/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:20 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/12 09:21:12 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_clean(t_data *data, int n, int index)
{
	int	i;

	i = -1;
	if (data->forks)
	{
		while (++i < index)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	if (n != 0 || n == 6)
		pthread_mutex_destroy(&data->print_lock);
	if ((n != 0 && n != 1) || n == 6)
		pthread_mutex_destroy(&data->meals_mutex);
	if ((n != 0 && n != 1 && n != 2) || n == 6)
		pthread_mutex_destroy(&data->stop_mutex);
}
