/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/02 16:09:07 by ybahmaz          ###   ########.fr       */
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

void	ft_usleep(size_t time, t_philos *philos)
{
	size_t	start;

	start = ft_current_time();
	while (ft_current_time() - start < time)
	{
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
		{	
			pthread_mutex_unlock(&philos->data->stop_mutex);
			return ;
		}
		pthread_mutex_unlock(&philos->data->stop_mutex);
		usleep(1000);
	}
}

int	ft_eating(t_philos *philos)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philos->l_fork < philos->r_fork)
	{
		first_fork = philos->l_fork;
		second_fork = philos->r_fork;
	}
	else
	{
		first_fork = philos->r_fork;
		second_fork = philos->l_fork;
	}
	pthread_mutex_lock(first_fork);
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(&philos->data->stop_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philos->data->stop_mutex);
	pthread_mutex_lock(second_fork);
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(&philos->data->stop_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philos->data->stop_mutex);
	ft_print_status(philos, "is eating");
	pthread_mutex_lock(&philos->data->meals_mutex);
	philos->last_meal_time = ft_current_time();
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->data->meals_mutex);
	ft_usleep((size_t)philos->data->time_eat, philos);
	if (philos->meals_eaten == philos->data->n_meals)
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
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
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->stop_mutex), NULL);
		pthread_mutex_unlock(&philos->data->stop_mutex);
		if (!ft_eating(philos))
			return (NULL);
		pthread_mutex_lock(&philos->data->stop_mutex);
		ft_print_status(philos, "is sleeping");
		pthread_mutex_unlock(&philos->data->stop_mutex);
		ft_usleep((size_t)philos->data->time_sleep, philos);
		pthread_mutex_lock(&philos->data->stop_mutex);
		ft_print_status(philos, "is thinking");
		pthread_mutex_unlock(&philos->data->stop_mutex);
	}
}

void	*ft_one_philo(void	*arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	while (1)
	{
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->stop_mutex), NULL);
		pthread_mutex_unlock(&philos->data->stop_mutex);
		ft_print_status(philos, "has taken a fork");
		ft_print_status(philos, "is eating");
		pthread_mutex_lock(&philos->data->meals_mutex);
		philos->last_meal_time = ft_current_time();
		philos->meals_eaten++;
		pthread_mutex_unlock(&philos->data->meals_mutex);
		ft_usleep((size_t)philos->data->time_eat, philos);
		if (philos->meals_eaten == philos->data->n_meals)
			return (NULL);
		ft_print_status(philos, "is sleeping");
		ft_usleep((size_t)philos->data->time_sleep, philos);
		ft_print_status(philos, "is thinking");
	}
}

void	*ft_monitor(void *arg)
{
	t_data	*data;
	size_t	l_meal_time;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			pthread_mutex_lock(&data->meals_mutex);
			if (data->stop || data->philos[i].meals_eaten == data->n_meals)
				return (pthread_mutex_unlock(&data->meals_mutex), NULL);
			l_meal_time = data->philos[i].last_meal_time;
			pthread_mutex_unlock(&data->meals_mutex);
			if (ft_current_time() - l_meal_time > (size_t)data->time_die
				&& data->time_eat != data->time_die)
			{
				return (ft_print_status(&data->philos[i], "died"),
					pthread_mutex_lock(&data->stop_mutex), data->stop = 1,
					pthread_mutex_unlock(&data->stop_mutex), NULL);
			}
		}
	}
	return (NULL);
}

int	ft_more_philos(t_data *data)
{
	int			i;
	pthread_t	monitor;

	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				ft_routine, &data->philos[i]) != 0)
			return (write(2, "Error in simulation\n", 20), 0);
	}
	if (pthread_create(&monitor, NULL, ft_monitor, data) != 0)
		return (write(2, "Error in simulation\n", 20), 0);
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (write(2, "Error joining threads\n", 22), 0);
	}
	if (pthread_join(monitor, NULL) != 0)
		return (write(2, "Error joining threads\n", 22), 0);
	return (1);
}

int	ft_start_simulation(t_data *data)
{
	pthread_t	monitor;

	if (data->n_philo == 1)
	{
		if (pthread_create(&data->philos[0].thread, NULL,
				ft_one_philo, &data->philos[0]))
			return (write(2, "Error in simulation\n", 20), 0);
		if (pthread_create(&monitor, NULL, ft_monitor, data) != 0)
			return (write(2, "Error in simulation\n", 20), 0);
		if (pthread_join(data->philos[0].thread, NULL) != 0)
			return (write(2, "Error joining threads\n", 22), 0);
		if (pthread_join(monitor, NULL) != 0)
			return (write(2, "Error joining threads\n", 22), 0);
	}
	else
	{
		if (!ft_more_philos(data))
			return (0);
	}
	return (1);
}
