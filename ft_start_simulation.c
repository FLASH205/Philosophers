/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start_simulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybahmaz <ybahmaz@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:50:10 by ybahmaz           #+#    #+#             */
/*   Updated: 2025/03/22 11:53:13 by ybahmaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_print_status(t_philos *philo, char *messg)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->print_lock);
		return ;
	}
	printf("%zu %d %s\n", ft_current_time() - philo->data->t_start, philo->n, messg);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	ft_apply_usleep(int	time)
{
	size_t	start;

	start = ft_current_time();
	while (ft_current_time() - start < (size_t)time)
		usleep(500);
	return (0);
}

void	*ft_philo_routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	while (1)
	{
		//pthread_mutex_lock(&philo->data->death_check);
		if (philo->data->stop)
		{
			//pthread_mutex_unlock(&philo->data->death_check);
			break;
		}
		//pthread_mutex_unlock(&philo->data->death_check);
		if (philo->n % 2 == 0)			//!____________________waiting for philo whose number is even____________________
			usleep(1000);
		pthread_mutex_lock(philo->l_fork);			//?___________________lock left fork___________________
		ft_print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);			//?___________________lock right fork___________________
		ft_print_status(philo, "has taken a fork");
		ft_print_status(philo, "is eating");
		//pthread_mutex_lock(&philo->data->death_check);
		philo->last_meal_time = ft_current_time() - philo->data->t_start;
		printf("last meal in vpid function == %ld \n", philo->last_meal_time);
		philo->meals_eaten++;
		//pthread_mutex_unlock(&philo->data->death_check);
		ft_apply_usleep(philo->data->time_eat);		//!__________________time to eat__________________
		pthread_mutex_unlock(philo->r_fork);		//?___________________unlock right fork___________________
		pthread_mutex_unlock(philo->l_fork);		//?___________________unlock left fork___________________
		ft_print_status(philo, "is sleeping");
		ft_apply_usleep(philo->data->time_sleep);	//!__________________time to sleep__________________
		ft_print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*ft_check_death(void *arg)
{
	t_data		*data;
	t_philos	*philo;
	int			i;

	data = (t_data *)arg;
	philo = data->philos;
	printf("hiiii\n\n\n");
	while (1)
	{
		i = -1;
		while (++i < data->n_philo)
		{
			//pthread_mutex_lock(&data->death_check);
			//printf("last meal == %ld \n", philo[i].last_meal_time);
			//printf("ft_current_time() == %ld \n", ft_current_time() - philo[i].data->t_start);
			if ((ft_current_time() - philo[i].data->t_start)  - philo[i].last_meal_time > (size_t)data->time_die && philo[i].last_meal_time != (size_t)-1 && (philo[i].last_meal_time != ft_current_time() - philo->data->t_start + philo->data->time_eat) && philo[i].last_meal_time != 0)
			{
				ft_print_status(&philo[i], "is died");		//*	<---------------	check this
				data->stop = 1;
				pthread_mutex_unlock(&data->death_check);
				return (NULL);
			}
			//else
			//	pthread_mutex_unlock(&data->death_check);
			if (i == data->n_philo)
				i = -1;
		}
	}
	return (NULL);
}

int	ft_start_simulation(t_data *data)
{
	int			i;
	pthread_t	death;

	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL, ft_philo_routine, &data->philos[i]))	//!_______________create thread for every philo _______________
			return (write(2, "Error in simulation\n", 20), 0);
		if (i == data->n_philo - 1)
		{
			if (pthread_create(&death, NULL, ft_check_death, data))		//!_______________create thread for check if philo die_______________
				return (write(2, "Error in simulation\n", 20), 0);
		}
	}
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (write(2, "Error in simulation\n", 20), 0);
	}
	pthread_join(death, NULL);
	return (1);
}
