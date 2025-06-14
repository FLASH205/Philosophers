/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:03:00 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/06/14 09:30:03 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitoring_death_philos(void *arg)
{
	t_philos	*philo;
	size_t		l_meal_time;

	philo = (t_philos *)arg;
	while (1)
	{
		sem_wait(philo->data->meals_sem);
		l_meal_time = philo->last_meal_time;
		sem_post(philo->data->meals_sem);
		if (ft_current_time() - l_meal_time >= (size_t)philo->data->time_die)
		{
			ft_print_status(philo, "died", 1);
			sem_post(philo->data->kill_sem);	//^________________________
			// sem_wait(philo->data->stop_sem);
			// philo->data->stop = 1;
			// sem_post(philo->data->stop_sem);
			exit (0);
		}
	}
	return (NULL);
}

int	philos_routine(t_data *data, t_philos *philo)
{
	pthread_t	death;

	if (pthread_create(&death, NULL, monitoring_death_philos, philo))
		return (write(2, "Failed pthread_create\n", 22), 0);
	if (pthread_detach(death))
		return (write(2, "Failed pthread_detach\n", 22), 0);
	if (philo->n % 2 == 0)
		usleep(500);
	while (1)
	{
		//sem_wait(data->stop_sem);
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	return (1);
		//}
		//sem_post(data->stop_sem);
		//*	Taken a fork_________________________
		sem_wait(data->forks);
		//sem_wait(data->stop_sem);
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	sem_post(data->forks);
		//	return (1);
		//}
		//sem_post(data->stop_sem);
		ft_print_status(philo, "has taken a fork", 0);

		sem_wait(data->forks);
		
		//sem_wait(data->stop_sem);
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	sem_post(data->forks);
		//	sem_post(data->forks);
		//	return (1);
		//}
		//sem_post(data->stop_sem);
		ft_print_status(philo, "has taken a fork", 0);
		
		//*	Eating_______________________________
		//sem_wait(data->stop_sem);//?______________________________
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	sem_post(data->forks);
		//	sem_post(data->forks);
		//	return (1);
		//}
		//sem_post(data->stop_sem);//?______________________________
		ft_print_status(philo, "is eating", 0);
		sem_wait(data->meals_sem);
		philo->last_meal_time = ft_current_time();
		philo->meals_eaten++;
		sem_post(data->meals_sem);
		ft_usleep((size_t)data->time_eat, philo);
		
		//*	Release a fork_______________________
		sem_post(data->forks);
		sem_post(data->forks);
		
		sem_wait(data->meals_sem);	//~	Compare number of meals____________________
		if (philo->meals_eaten == data->n_meals)
		{
			// printf("[%zu] Philosopher %d has completed all meals.\n", ft_current_time(), philo->n);
			sem_post(data->meals_sem);
			sem_post(data->done_meals);
			sem_post(data->kill_sem);
			return (1);
			//exit(1);
		}
		sem_post(data->meals_sem);	//~_________________________________________

		//*	Sleeping_____________________________
		//sem_wait(data->stop_sem);//?______________________________
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	return (1);
		//}
		//sem_post(data->stop_sem);//?______________________________
		ft_print_status(philo, "is sleeping", 0);
		ft_usleep((size_t)data->time_sleep, philo);
		
		//*	Thinking_____________________________
		//sem_wait(data->stop_sem);//?______________________________
		//if (data->stop)
		//{
		//	sem_post(data->stop_sem);
		//	return (1);
		//}
		//sem_post(data->stop_sem);//?______________________________
		ft_print_status(philo, "is thinking", 0);
		// printf("hello\n");
		// usleep(200);
	}
	return (1);
}

void	kill_processes(t_data *data, int index, int pid)
{
	int	i;

	if (pid != -1)
		kill(pid, SIGKILL);
	i = 0;
	while (i < index)
	{
		kill(data->philos[i].pid, SIGKILL);
		i++;
	}
	
}

int	ft_start_simulation(t_data *data)
{
	int	i;
	int	pid;
	int	n;

	i = 0;
	while (i < data->n_philo)
	{
		pid = fork();
		if (pid < 0)
			kill_processes(data, i, -1);
		else if (pid == 0)
		{
			n = philos_routine(data, &data->philos[i]);
			if (!n)
				return (0);
			exit(0);
		}
		data->philos[i].pid = pid;
		i++;
	}
	return (1);
}
