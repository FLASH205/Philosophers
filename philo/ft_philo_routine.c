/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:39:49 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/11 10:16:44 by ybahmaz          ###   ########.fr       */
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
		usleep(time / 1000);
	}
}

int	ft_take_fork(t_philos *philos)
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
		return (pthread_mutex_unlock(first_fork),
			pthread_mutex_unlock(&philos->data->stop_mutex), 0);
	pthread_mutex_unlock(&philos->data->stop_mutex);
	pthread_mutex_lock(second_fork);
	ft_print_status(philos, "has taken a fork");
	return (1);
}

int	ft_eating(t_philos *philos)
{
	if (!ft_take_fork(philos))
		return (0);
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
		return (pthread_mutex_unlock(philos->l_fork),
			pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(&philos->data->stop_mutex), 0);
	pthread_mutex_unlock(&philos->data->stop_mutex);
	ft_print_status(philos, "is eating");
	pthread_mutex_lock(&philos->data->meals_mutex);
	philos->last_meal_time = ft_current_time();
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->data->meals_mutex);
	ft_usleep((size_t)philos->data->time_eat, philos);
	if (philos->meals_eaten == philos->data->n_meals || philos->data->stop == 1)
	{
		pthread_mutex_unlock(philos->r_fork);
		pthread_mutex_unlock(philos->l_fork);
		return (0);
	}
	pthread_mutex_unlock(philos->r_fork);
	pthread_mutex_unlock(philos->l_fork);
	return (1);
}

void	*ft_philo_routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->n % 2 == 0)
		usleep(1000);
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
