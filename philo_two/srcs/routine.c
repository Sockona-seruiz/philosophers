/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:15 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 16:14:48 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

uint64_t	set_get_last_meal_time(t_struct *s, int	sw, int id)
{
	uint64_t	ret;

	sem_wait(s->sem_write);
	if (sw != -1)
	{
		s->last_meal_t[id] = get_time();
		sem_post(s->sem_write);
		return (0);
	}
	s->actual_time = get_time();
	ret = s->last_meal_t[id];
	sem_post(s->sem_write);
	return (ret);
}

void	*routine_loop(void	*arg)
{
	t_philo		*philo;

	philo = arg;
	philo->s->last_meal_t[philo->id] = get_time();
	while (philo->state != DEAD)
	{
		if (philo->state == THINK)
			pick_fork(philo, philo->id - 1);
		if (philo->state == SLEEP)
		{
			speak(philo->s, SLEEP, philo->id);
			custom_usleep(philo->s->ttsleep);
			philo->state = THINK;
		}
	}
	return (0);
}

int	monitoring_loop(t_struct *s)
{
	int			i;
	int			done_eating;
	uint64_t	time;

	i = 0;
	done_eating = 1;
	while (42)
	{
		while (i < s->philo_nb)
		{
			s->actual_time = get_time();
			time = set_get_last_meal_time(s, -1, i);
			if (s->actual_time - time > (uint64_t)s->ttdie)
				return (speak(s, DEAD, i + 1));
			if (s->eat_count[i] < s->total_eat)
				done_eating = 0;
			i++;
		}
		if (done_eating == 1 && s->total_eat != 0)
			return (speak(s, DONE, i + 1));
		done_eating = 1;
		i = 0;
		usleep(50);
	}
}
