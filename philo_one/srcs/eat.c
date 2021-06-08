/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:14 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/07 17:32:23 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	pick_fork(t_philo *philo, int i)
{
	int	second_fork;

	pthread_mutex_lock(&(philo->s->forks[i]));
	speak(philo->s, FORK, philo->id);
	if (i + 1 == philo->s->philo_nb)
		second_fork = 0;
	else
		second_fork = i + 1;
	pthread_mutex_lock(&(philo->s->forks[second_fork]));
	speak(philo->s, FORK, philo->id);
	speak(philo->s, EAT, philo->id);
	set_get_last_meal_time(philo->s, 1, i);
	custom_usleep(philo->s->tteat);
	philo->s->eat_count[i]++;
	philo->state = SLEEP;
	pthread_mutex_unlock(&(philo->s->forks[second_fork]));
	pthread_mutex_unlock(&(philo->s->forks[i]));
	return (0);
}
