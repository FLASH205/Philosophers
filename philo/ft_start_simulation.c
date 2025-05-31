/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/05/31 08:44:40 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_one_philo(void	*arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	while (1)
	{
		ft_print_status(philos, "has taken a fork");
		ft_usleep(philos->data->time_die, philos);
		ft_print_status(philos, "died");
		return (NULL);
	}
}

int	ft_check_meals_death(t_data *data, int i, int *num)
{
	size_t	l_meal_time;

	pthread_mutex_lock(&data->meals_mutex);
	if (!data->philos[i].limit
		&& data->n_meals == data->philos[i].meals_eaten)
	{
		data->philos[i].limit = 1;
		(*num)++;
	}
	if (*num == data->n_philo)
		return (pthread_mutex_unlock(&data->meals_mutex), 0);
	if (data->stop)
		return (pthread_mutex_unlock(&data->meals_mutex), 0);
	l_meal_time = data->philos[i].last_meal_time;
	pthread_mutex_unlock(&data->meals_mutex);
	if (!data->philos[i].limit
		&& (ft_current_time() - l_meal_time >= (size_t)data->time_die))
	{
		pthread_mutex_lock(&data->stop_mutex);
		ft_print_status(&data->philos[i], "died");
		data->stop = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		return (0);
	}
	return (1);
}

void	*ft_monitore(void *arg)
{
	t_data	*data;
	int		i;
	int		num;

	data = (t_data *)arg;
	num = 0;
	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			if (!ft_check_meals_death(data, i, &num))
				return (NULL);
		}
	}
}

int	ft_more_philos(t_data *data)
{
	int			i;
	pthread_t	monitore;

	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				ft_philo_routine, &data->philos[i]) != 0)
			return (write(2, "Error in simulation\n", 20), 0);
	}
	if (pthread_create(&monitore, NULL, ft_monitore, data) != 0)
		return (write(2, "Error in simulation\n", 20), 0);
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (write(2, "Error joining threads\n", 22), 0);
	}
	if (pthread_join(monitore, NULL) != 0)
		return (write(2, "Error joining threads\n", 22), 0);
	return (1);
}

int	ft_start_simulation(t_data *data)
{
	if (data->n_philo == 1)
	{
		if (pthread_create(&data->philos[0].thread, NULL,
				ft_one_philo, &data->philos[0]))
			return (write(2, "Error in simulation\n", 20), 0);
		if (pthread_join(data->philos[0].thread, NULL) != 0)
			return (write(2, "Error joining threads\n", 22), 0);
	}
	else
	{
		if (!ft_more_philos(data))
			return (0);
	}
	return (1);
}
