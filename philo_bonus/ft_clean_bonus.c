/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:20 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/05 10:55:29 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_clean(t_data *data)
{
	int	i;

	i = -1;
	if (data->forks)
	{
		while (++i < data->n_philo)
			sem_destroy(&data->forks[i]);
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	sem_destroy(&data->print_lock);
	sem_destroy(&data->meals_mutex);
	sem_destroy(&data->stop_mutex);
}
