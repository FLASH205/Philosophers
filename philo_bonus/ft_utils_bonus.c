/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:29:54 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/04 11:34:34 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi(char *str)
{
	int		i;
	long	r;

	i = 0;
	r = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		r = r * 10 + str[i] - '0';
		if (r > 2147483647)
			return (0);
		i++;
	}
	return (r);
}

size_t	ft_current_time(void)
{
	struct timeval	tv;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_print_status(t_philos *philos, char *str, int oky)
{
	size_t	start_time;

	start_time = philos->data->start_time;
	sem_wait(philos->data->print_sem);
	printf("%ld %d %s\n", ft_current_time() - start_time, philos->n, str);
	if (!oky)
		sem_post(philos->data->print_sem);
}

void	ft_usleep(size_t time, t_philos *philos)
{
	size_t	start;

	start = ft_current_time();
	while (ft_current_time() - start < time)
	{
		sem_wait(philos->data->stop_sem);
		if (philos->data->stop)
		{	
			sem_post(philos->data->stop_sem);
			return ;
		}
		sem_post(philos->data->stop_sem);
		usleep(200);
	}
}
