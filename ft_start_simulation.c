/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/03/28 16:03:01 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_print_status(t_philos *philos, char *str)
{
	size_t	start_time;

	start_time = philos->data->start_time;
	pthread_mutex_lock(&philos->data->print_lock);
	if (philos->data->stop)
	{
		pthread_mutex_unlock(&philos->data->print_lock);
		return ;
	}
	printf("%ld %d %s\n", ft_current_time() - start_time, philos->n, str);
	pthread_mutex_unlock(&philos->data->print_lock);
}

void	ft_smart_sleep(size_t time, t_philos *philos)
{
	size_t	start;

	start = ft_current_time();
	while (ft_current_time() - start < time)
	{
		if (philos->data->stop)
			return ;
		usleep(1000);
	}
}

int	ft_eating(t_philos *philos)
{
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_lock(philos->l_fork);
	pthread_mutex_lock(&philos->data->print_lock);
	if (philos->data->stop)
		return (pthread_mutex_unlock(philos->l_fork),
			pthread_mutex_unlock(&philos->data->print_lock), 0);
	pthread_mutex_unlock(&philos->data->print_lock);
	pthread_mutex_lock(philos->r_fork);
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_lock(&philos->data->print_lock);
	if (philos->data->stop)
		return (pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(philos->l_fork),
			pthread_mutex_unlock(&philos->data->print_lock), 0);
	pthread_mutex_unlock(&philos->data->print_lock);
	ft_print_status(philos, "is eating");
	pthread_mutex_lock(&philos->data->print_lock);
	philos->last_meal_time = ft_current_time();
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->data->print_lock);
	if (philos->meals_eaten == philos->data->n_meals)
		return (pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(philos->l_fork), 0);
	ft_smart_sleep((size_t)philos->data->time_eat, philos);
	pthread_mutex_unlock(philos->r_fork);
	pthread_mutex_unlock(philos->l_fork);
	return (1);
}

void	*ft_routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->n % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&philos->data->print_lock);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->print_lock), NULL);
		pthread_mutex_unlock(&philos->data->print_lock);
		if (!ft_eating(philos))
			return (NULL);
		pthread_mutex_lock(&philos->data->print_lock);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->print_lock), NULL);
		pthread_mutex_unlock(&philos->data->print_lock);
		ft_print_status(philos, "is sleeping");
		pthread_mutex_lock(&philos->data->print_lock);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->print_lock), NULL);
		pthread_mutex_unlock(&philos->data->print_lock);
		ft_smart_sleep((size_t)philos->data->time_sleep, philos);
		ft_print_status(philos, "is thinking");
	}
}

void	*ft_one_philo(void	*arg)
{
	t_philos	*philos;
	
	philos = (t_philos *)arg;
	while (1)
	{
		if (philos->data->stop)
			return (NULL);
		ft_print_status(philos, "has taken a fork");
		ft_print_status(philos, "is eating");
		philos->last_meal_time = ft_current_time();
		philos->meals_eaten++;
		ft_smart_sleep(philos->data->time_eat, philos);
		if (philos->meals_eaten == philos->data->n_meals)
			return (NULL);
		ft_print_status(philos, "is sleeping");
		ft_smart_sleep(philos->data->time_sleep, philos);
		ft_print_status(philos, "is thinking");
	}
	
}

void	*ft_monitor(t_data *data)
{
	size_t	l_meal_time;
	int		i;

	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			pthread_mutex_lock(&data->print_lock);
			if (data->stop || data->philos[i].meals_eaten == data->n_meals)
				return (pthread_mutex_unlock(&data->print_lock), NULL);
			l_meal_time = data->philos[i].last_meal_time;
			if (ft_current_time() - l_meal_time > (size_t)data->time_die
				&& data->time_eat != data->time_die)
			{			
				pthread_mutex_unlock(&data->print_lock);
				ft_print_status(&data->philos[i], "died");
				data->stop = 1;
				return (NULL);
			}
			pthread_mutex_unlock(&data->print_lock);
		}
	}
	return (0);
}

int	ft_start_simulation(t_data *data)
{
	int			i;
	//pthread_t	monitor_thread;

	i = 0;
	if (data->n_philo == 1)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			ft_one_philo, &data->philos[i]))
		return (write(2, "Error in simulation\n", 20), 0);
	}
	else
	{
		while (i < data->n_philo)
		{
			if (pthread_create(&data->philos[i].thread, NULL,
					ft_routine, &data->philos[i]) != 0)
				return (write(2, "Error in simulation\n", 20), 0);
			i++;
		}
	}
	ft_monitor(data);
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (write(2, "Error joining threads\n", 20), 0);
		i++;
	}
	return (1);
}
