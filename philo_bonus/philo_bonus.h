/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:09:29 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/05 14:00:03 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <semaphore.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <signal.h>

typedef struct s_philos
{
	int				n;
	size_t			last_meal_time;
	int				meals_eaten;
	int				pid;
	sem_t			*l_fork;
	sem_t			*r_fork;
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
	t_philos		*philos;
	sem_t			*forks;
	sem_t	print_lock;
	sem_t	meals_mutex;
	sem_t	stop_mutex;
}	t_data;

int		ft_atoi(char *str);
size_t	ft_current_time(void);
void	ft_print_status(t_philos *philos, char *str);
int		ft_start_simulation(t_data *data);
void	ft_clean(t_data *data);

#endif