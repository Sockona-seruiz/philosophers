/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:15 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/11 14:40:36 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

uint64_t	set_get_last_meal_time(t_struct *s, int	sw)
{
	uint64_t	ret;

	sem_wait(s->sem_write);
	if (sw != -1)
	{
		s->last_meal_t = get_time();
		sem_post(s->sem_write);
		return (0);
	}
	s->actual_time = get_time();
	ret = s->last_meal_t;
	sem_post(s->sem_write);
	return (ret);
}

void	*routine_loop(void	*arg)
{
	t_philo		*philo;
	pthread_t	th_id;

	philo = arg;
	philo->s->philo_id = philo->id;
	philo->s->last_meal_t = get_time();
	pthread_create(&(th_id), NULL, monitoring_loop, &(philo->s));
	while (philo->state != DEAD)
	{
		if (philo->state == THINK)
			pick_fork(philo);
		if (philo->state == SLEEP)
		{
			speak(philo->s, SLEEP, philo->id);
			custom_usleep(philo->s->ttsleep);
			philo->state = THINK;
		}
	}
	return (0);
}

void	*monitoring_loop(void *arg)
{
	int			i;
	int			done_eating;
	uint64_t	time;
	t_struct	*s;

	s = arg;
	i = s->philo_id;
	done_eating = 1;
	while (42)
	{
		time = set_get_last_meal_time(s, -1);
		s->actual_time = get_time();
		if (s->actual_time - time > (uint64_t)s->ttdie)
		{
			speak(s, DEAD, i + 1);
			exit (1);
		}
		if (s->eat_count < s->total_eat)
			done_eating = 0;
		if (done_eating == 1 && s->total_eat != 0)
		{
			speak(s, DONE, i + 1);
			exit (0);
		}
		done_eating = 1;
	}
}
