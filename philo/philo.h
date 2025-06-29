/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:42:08 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/12 09:20:56 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philos
{
	int				limit;
	int				n;
	size_t			last_meal_time;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	struct s_data	*data;
}	t_philos;

typedef struct s_data
{
	int				n_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				n_meals;
	size_t			start_time;
	int				stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meals_mutex;
	pthread_mutex_t	stop_mutex;
	t_philos		*philos;
}	t_data;

int		ft_atoi(char *str);
size_t	ft_current_time(void);
void	ft_print_status(t_philos *philos, char *str);
int		ft_start_simulation(t_data *data);
void	*ft_philo_routine(void *arg);
void	ft_usleep(size_t time, t_philos *philos);
void	ft_clean(t_data *data, int n, int index);

#endif