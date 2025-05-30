/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:08:56 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/05 11:47:59 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_initial_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		sem_init(&data->forks[i], 0, 1);
		data->philos[i].n = i + 1;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[(i + 1) % data->n_philo];
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
		return (0);
	data->stop = 0;
	data->start_time = ft_current_time();
	data->forks = malloc(sizeof(sem_t) * data->n_philo);
	data->philos = malloc(sizeof(t_philos) * data->n_philo);
	if (!data->forks || !data->philos)
		return (1);
	sem_init(&data->print_lock, 0, 1);
	sem_init(&data->meals_mutex, 0, 1);
	sem_init(&data->stop_mutex, 0, 1);
	return (1);
}

int main(int ac, char *av[])
{
	t_data	data;
	if (ac < 5 || ac > 6)
		return (write(2, "Should be 5 or 6 arguments\n", 27), 1);
	if (!ft_initial_data(&data, av))
		return (write(2, "Invalid arguments\n", 18), 1);
	ft_initial_philos(&data);
	if (!ft_start_simulation(&data))
		return (ft_clean(&data), 1);
	//ft_monitor(&data);
	ft_clean(&data);
	return (0);
}
