/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/04/05 15:45:20 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_print_status(t_philos *philos, char *str)
{
	size_t	start_time;

	start_time = philos->data->start_time;
	sem_wait(&philos->data->print_lock);
	if (philos->data->stop)
	{
		sem_post(&philos->data->print_lock);
		return ;
	}
	printf("%ld %d %s\n", ft_current_time() - start_time, philos->n, str);
	sem_post(&philos->data->print_lock);
}

void	ft_usleep(size_t time, t_philos *philos)
{
	size_t	start;

	start = ft_current_time();
	while (ft_current_time() - start < time)
	{
		sem_wait(&philos->data->stop_mutex);
		if (philos->data->stop)
		{	
			sem_post(&philos->data->stop_mutex);
			return ;
		}
		sem_post(&philos->data->stop_mutex);
		usleep(1000);
	}
}

int	ft_eating(t_philos *philos)
{
	sem_t	*first_fork;
	sem_t	*second_fork;

	if (philos->l_fork < philos->r_fork)
	{
		first_fork = philos->l_fork;
		second_fork = philos->r_fork;
	}
	else
	{
		first_fork = philos->r_fork;
		second_fork = philos->l_fork;
	}
	sem_wait(first_fork);
	ft_print_status(philos, "has taken a fork");
	sem_wait(&philos->data->stop_mutex);
	if (philos->data->stop)
	{
		sem_post(first_fork);
		sem_post(&philos->data->stop_mutex);
		(ft_clean(philos->data), exit(0));
	}
	sem_post(&philos->data->stop_mutex);
	sem_wait(second_fork);
	ft_print_status(philos, "has taken a fork");
	sem_wait(&philos->data->stop_mutex);
	if (philos->data->stop)
	{
		sem_post(second_fork);
		sem_post(first_fork);
		sem_post(&philos->data->stop_mutex);
		(ft_clean(philos->data), exit(0));
	}
	sem_post(&philos->data->stop_mutex);
	ft_print_status(philos, "is eating");
	sem_wait(&philos->data->meals_mutex);
	philos->last_meal_time = ft_current_time();
	philos->meals_eaten++;
	sem_post(&philos->data->meals_mutex);
	ft_usleep((size_t)philos->data->time_eat, philos);
	if (philos->meals_eaten == philos->data->n_meals)
	{
		sem_post(second_fork);
		sem_post(first_fork);
		(ft_clean(philos->data), exit(0));
	}
	sem_post(second_fork);
	sem_post(first_fork);
	return (1);
}

void	*ft_routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->n % 2 == 0)
		usleep(100);
	while (1)
	{
		sem_wait(&philos->data->stop_mutex);
		if (philos->data->stop)
			(ft_clean(philos->data), sem_post(&philos->data->stop_mutex), exit(0));
		sem_post(&philos->data->stop_mutex);
		if (!ft_eating(philos))
			return (NULL);
		sem_wait(&philos->data->stop_mutex);
		ft_print_status(philos, "is sleeping");
		sem_post(&philos->data->stop_mutex);
		ft_usleep((size_t)philos->data->time_sleep, philos);
		sem_wait(&philos->data->stop_mutex);
		ft_print_status(philos, "is thinking");
		sem_post(&philos->data->stop_mutex);
	}
}

void	*ft_one_philo(void	*arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	while (1)
	{
		sem_wait(&philos->data->stop_mutex);
		if (philos->data->stop)
			(sem_post(&philos->data->stop_mutex), ft_clean(philos->data), exit(0));
		sem_post(&philos->data->stop_mutex);
		ft_print_status(philos, "has taken a fork");
		ft_print_status(philos, "is eating");
		sem_wait(&philos->data->meals_mutex);
		philos->last_meal_time = ft_current_time();
		philos->meals_eaten++;
		sem_post(&philos->data->meals_mutex);
		ft_usleep((size_t)philos->data->time_eat, philos);
		if (philos->meals_eaten == philos->data->n_meals)
			return (NULL);
		ft_print_status(philos, "is sleeping");
		ft_usleep((size_t)philos->data->time_sleep, philos);
		ft_print_status(philos, "is thinking");
	}
}

void	*ft_monitor(void *arg)
{
	t_data	*data;
	size_t	l_meal_time;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			sem_wait(&data->meals_mutex);
			if (data->stop || data->philos[i].meals_eaten == data->n_meals)
				return (sem_post(&data->meals_mutex), NULL);
			l_meal_time = data->philos[i].last_meal_time;
			sem_post(&data->meals_mutex);
			if (ft_current_time() - l_meal_time > (size_t)data->time_die
				&& data->time_eat != data->time_die)
			{
				return (ft_print_status(&data->philos[i], "died"),
					sem_wait(&data->stop_mutex), data->stop = 1,
					sem_post(&data->stop_mutex), NULL);
			}
		}
	}
	return (NULL);
}

void	ft_more_philos(t_data *data)
{
	int	i;
	int	m_pid;

	i = -1;
	while (++i < data->n_philo)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
			(ft_clean(data), write(2, "Error in fork()\n", 16), exit(1));
		else if (data->philos[i].pid == 0)
			ft_routine(&data->philos[i]);
	}
	m_pid = fork();
	if (m_pid < 0)
		(ft_clean(data), write(2, "Error in fork()\n", 16), exit(1));
	else if (m_pid == 0)
		ft_monitor(data);
	waitpid(m_pid, NULL, 0);
	i = -1;
	while (++i < data->n_philo)
		waitpid(data->philos[i].pid, NULL, 0);
	
}

int	ft_start_simulation(t_data *data)
{
	int	m_pid;

	if (data->n_philo == 1)
	{
		data->philos[0].pid = fork();
		if (data->philos[0].pid < 0)
			(ft_clean(data), write(2, "Error in fork()\n", 16), exit(1));
		else if (data->philos[0].pid == 0)
			ft_one_philo(&data->philos[0]);
		m_pid = fork();
		if (m_pid < 0)
			(ft_clean(data), write(2, "Error in fork()\n", 16), exit(1));
		else if (m_pid == 0)
			ft_monitor(data);
		waitpid(m_pid, NULL, 0);
		waitpid(data->philos[0].pid, NULL, 0);
	}
	else
		ft_more_philos(data);
	return (1);
}
