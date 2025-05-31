/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:09:29 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/05/31 18:04:53 by ybahmaz          ###   ########.fr       */
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
# include <fcntl.h>
# include <signal.h>

typedef struct s_philos
{
	int				n;
	int				limit;
	size_t			last_meal_time;
	int				meals_eaten;
	int				pid;
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
	sem_t	*print_sem;
	sem_t	*meals_sem;
	sem_t	*stop_sem;
}	t_data;

int		ft_atoi(char *str);
size_t	ft_current_time(void);
void	ft_print_status(t_philos *philos, char *str);
void	ft_usleep(size_t time, t_philos *philos);
int		ft_start_simulation(t_data *data);
void	ft_clean(t_data *data);
void	kill_processes(t_data *data);

#endif