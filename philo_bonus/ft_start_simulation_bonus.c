/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/05/31 18:13:00 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philos_routine(t_data *data, t_philos *philo)
{
	while (1)
	{
		//*	Taken a fork_________________________
		sem_wait(data->forks);
		ft_print_status(philo, "has taken a fork");
		sem_wait(data->forks);
		ft_print_status(philo, "has taken a fork");
		
		//*	Eating_______________________________
		ft_print_status(philo, "is eating");
		sem_wait(data->meals_sem);
		philo->last_meal_time = ft_current_time();
		philo->meals_eaten++;
		sem_post(data->meals_sem);
		ft_usleep((size_t)data->time_eat, philo);
		
		//*	Release a fork_______________________
		sem_post(data->forks);
		sem_post(data->forks);
		
		//*	Sleeping_____________________________
		ft_print_status(philo, "is sleeping");
		ft_usleep((size_t)data->time_sleep, philo);
		
		//*	Thinking_____________________________
		ft_print_status(philo, "is thinking");
		usleep(200);
	}
}

void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}

int	ft_start_simulation(t_data *data)
{
	int	i;
	int	pid;

	i = 0;
	while (i < data->n_philo)
	{
		pid = fork();
		if (pid < 0)
			kill_processes(data);
		else if (pid == 0)
			philos_routine(data, &data->philos[i]);
		data->philos[i].pid = pid;
		i++;
	}
	return (1);
}
