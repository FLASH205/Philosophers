/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:05:17 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/03/22 11:48:43 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_init_philo(t_data *data)
{
	t_philos	*philo;
	int			i;

	philo = data->philos;
	i = -1;
	while (++i < data->n_philo)
	{
		philo[i].n = i + 1;
		philo[i].last_meal_time = -1;
		philo[i].meals_eaten = 0;
		philo[i].l_fork = &data->forks[i];
		philo[i].r_fork = &data->forks[(i + 1) % data->n_philo];
		philo[i].data = data;
	}
}

int	ft_init_data(t_data *data, char **av)
{
	data->n_philo = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	data->n_meals = -1;
	if (av[5])
		data->n_meals = ft_atoi(av[5]);
	if (!data->philos || !data->time_die || !data->time_eat
		|| !data->time_sleep || !data->n_meals)
		return (write(2, "Invalid arguments\n", 18), 0);
	data->stop = 0;
	data->t_start = ft_current_time();
	if (!data->t_start)
		return (0);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	data->philos = malloc(sizeof(t_philos) * data->n_philo);
	if (!data->forks || !data->philos)
		return (0);
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (0);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (write(2, "Should be 5 or 6 arguments\n", 27), 1);
	if (!ft_init_data(&data, argv))
		return (1);
	ft_init_philo(&data);
	if (!ft_start_simulation(&data))
		return (1);
	return 0;
}
