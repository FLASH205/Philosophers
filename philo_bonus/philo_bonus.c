/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:08:56 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/03 12:43:22 by ybahmaz          ###   ########.fr       */
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
	data->num = 0;
	data->start_time = ft_current_time();
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meals");
	sem_unlink("/stop");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->n_philo);
	data->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	data->meals_sem = sem_open("/meals", O_CREAT, 0644, 1);
	data->stop_sem = sem_open("/stop", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->print_sem == SEM_FAILED
		|| data->meals_sem == SEM_FAILED || data->stop_sem == SEM_FAILED)
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

	if (ac < 5 || ac > 6)
		return (write(2, "Should be 5 or 6 arguments\n", 27), 1);
	if (!ft_initial_data(&data, av))
		return (1);
	ft_initial_philos(&data);
	if (!ft_start_simulation(&data))
		return (ft_clean(&data), 1);
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break;
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			kill_processes(&data);
			break;
		}
	}
	
	ft_clean(&data);
	return (0);
}
