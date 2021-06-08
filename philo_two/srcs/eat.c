/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:14 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 14:08:56 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	pick_fork(t_philo *philo, int i)
{
	sem_wait(philo->s->sem_forks);
	speak(philo->s, FORK, philo->id);
	sem_wait(philo->s->sem_forks);
	speak(philo->s, FORK, philo->id);
	speak(philo->s, EAT, philo->id);
	set_get_last_meal_time(philo->s, 1, i);
	custom_usleep(philo->s->tteat);
	philo->s->eat_count[i]++;
	philo->state = SLEEP;
	sem_post(philo->s->sem_forks);
	sem_post(philo->s->sem_forks);
	return (0);
}
