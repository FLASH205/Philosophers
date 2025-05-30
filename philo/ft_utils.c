/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:29:54 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/05/30 15:35:33 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	ft_print_status(t_philos *philos, char *str)
{
	size_t	start_time;

	start_time = philos->data->start_time;
	pthread_mutex_lock(&philos->data->print_lock);
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
		usleep(200);
	}
}
