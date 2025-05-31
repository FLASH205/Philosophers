/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:39:49 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/05/31 08:31:13 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_take_fork(t_philos *philos)
{
	pthread_mutex_lock(philos->l_fork);
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
		return (pthread_mutex_unlock(&philos->data->stop_mutex),
			pthread_mutex_unlock(philos->l_fork), 0);
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_unlock(&philos->data->stop_mutex);
	pthread_mutex_lock(philos->r_fork);
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
		return (pthread_mutex_unlock(&philos->data->stop_mutex),
			pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(philos->l_fork), 0);
	ft_print_status(philos, "has taken a fork");
	pthread_mutex_unlock(&philos->data->stop_mutex);
	return (1);
}

int	ft_eating(t_philos *philos)
{
	if (!ft_take_fork(philos))
		return (0);
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->data->stop)
		return (pthread_mutex_unlock(&philos->data->stop_mutex),
			pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(philos->l_fork), 0);
	ft_print_status(philos, "is eating");
	pthread_mutex_unlock(&philos->data->stop_mutex);
	pthread_mutex_lock(&philos->data->meals_mutex);
	philos->last_meal_time = ft_current_time();
	pthread_mutex_unlock(&philos->data->meals_mutex);
	ft_usleep((size_t)philos->data->time_eat, philos);
	pthread_mutex_lock(&philos->data->meals_mutex);
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->data->meals_mutex);
	pthread_mutex_lock(&philos->data->stop_mutex);
	if (philos->meals_eaten == philos->data->n_meals || philos->data->stop == 1)
		return (pthread_mutex_unlock(&philos->data->stop_mutex),
			pthread_mutex_unlock(philos->r_fork),
			pthread_mutex_unlock(philos->l_fork), 0);
	pthread_mutex_unlock(&philos->data->stop_mutex);
	return (pthread_mutex_unlock(philos->r_fork),
		pthread_mutex_unlock(philos->l_fork), 1);
}

void	*ft_philo_routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->n % 2 == 0)
		usleep(500);
	while (1)
	{
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->stop_mutex), NULL);
		pthread_mutex_unlock(&philos->data->stop_mutex);
		if (!ft_eating(philos))
			return (NULL);
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->stop_mutex), NULL);
		ft_print_status(philos, "is sleeping");
		pthread_mutex_unlock(&philos->data->stop_mutex);
		ft_usleep((size_t)philos->data->time_sleep, philos);
		pthread_mutex_lock(&philos->data->stop_mutex);
		if (philos->data->stop)
			return (pthread_mutex_unlock(&philos->data->stop_mutex), NULL);
		ft_print_status(philos, "is thinking");
		pthread_mutex_unlock(&philos->data->stop_mutex);
	}
}
