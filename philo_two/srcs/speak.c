/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speak.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:13 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 16:18:34 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	speak_done(t_struct *s, int state)
{
	if (state == DONE)
	{
		s->done = 1;
		printf("Max eat count reached\n");
	}
	else if (s->done == 1)
		sem_post(s->sem_speak);
	return (0);
}

int	speak(t_struct *s, int state, int id)
{
	sem_wait(s->sem_speak);
	if (state == DONE || s->done == 1)
	{
		return (speak_done(s, state));
	}
	printf("%llu %d ", get_time() - s->start_time, id);
	if (state == FORK)
		printf("has taken a fork\n");
	else if (state == EAT)
		printf("is eating\n");
	else if (state == SLEEP)
		printf("is sleeping\n");
	else if (state == THINK)
		printf("is thinking\n");
	else if (state == DEAD)
	{
		printf("died\n");
		return (1);
	}
	sem_post(s->sem_speak);
	return (0);
}
