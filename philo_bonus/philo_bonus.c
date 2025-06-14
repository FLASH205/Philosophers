/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:08:56 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/14 09:34:22 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_initial_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		data->philos[i].n = i + 1;
		data->philos[i].last_meal_time = ft_current_time();
		data->philos[i].meals_eaten = 0;
		data->philos[i].limit = 0;
		data->philos[i].data = data;
		i++;
	}
}

int	ft_initial_data(t_data *data, char **av)
{
	data->n_philo = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	data->n_meals = -1;
	if (av[5])
		data->n_meals = ft_atoi(av[5]);
	if (!data->n_philo || !data->time_die || !data->time_eat
		|| !data->time_sleep || !data->n_meals)
		return (write(2, "Invalid arguments\n", 18), 0);
	data->stop = 0;
	data->start_time = ft_current_time();
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meals");
	sem_unlink("/stop");
	sem_unlink("/done_meals");
	sem_unlink("/kill_sem");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philo);
	data->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	data->meals_sem = sem_open("/meals", O_CREAT, 0644, 1);
	data->stop_sem = sem_open("/stop", O_CREAT, 0644, 1);
	data->done_meals = sem_open("/done_meals", O_CREAT, 0644, 0);
	data->kill_sem = sem_open("/kill_sem", O_CREAT, 0644, 0);
	if (data->forks == SEM_FAILED || data->print_sem == SEM_FAILED
		|| data->meals_sem == SEM_FAILED || data->stop_sem == SEM_FAILED
		|| data->done_meals == SEM_FAILED || data->kill_sem == SEM_FAILED)
		return (write(2, "Failed sem_open\n", 16), 0);
	data->philos = malloc(sizeof(t_philos) * data->n_philo);
	if (!data->philos)
		return (0);
	return (1);
}

int	main(int ac, char *av[])
{
	t_data	data;
	int		status;
	int		pid;
	int		pid_meals;
	int		i;

	pid_meals = -1;
	if (ac < 5 || ac > 6)
		return (write(2, "Should be 5 or 6 arguments\n", 27), 1);
	if (!ft_initial_data(&data, av))
		return (1);
	ft_initial_philos(&data);
	if (!ft_start_simulation(&data))
		return (ft_clean(&data), 1);
	sem_wait(data.kill_sem);	//^___________________________
	if (data.n_meals > 0)
	{
		pid_meals = fork();
		if (pid_meals == -1)
			return (ft_clean(&data), 1);
		sem_wait(data.kill_sem);
		if (pid_meals == 0)
		{
			i = 0;
			
			while (i < data.n_philo)
			{
				sem_wait(data.done_meals);
				i++;
			}
			exit(0);
		}
	}
	kill_processes(&data, data.n_philo, pid_meals);
	if (pid_meals != -1)
	{
		pid = waitpid(pid_meals, &status, 0);
		if (pid == -1)
			return (ft_clean(&data), 1);
	}
	i = 0;
	while (i < data.n_philo)
	{
		pid = waitpid(data.philos[i].pid, &status, 0);
		if (pid == -1)
			return (ft_clean(&data), 1);
	}
	// if (WEXITSTATUS(status) == 5 || WEXITSTATUS(status) == 0 || WEXITSTATUS(status) == 2)
	// 	kill_processes(&data, data.n_philo, pid_meals);
	return (ft_clean(&data), 0);
}
