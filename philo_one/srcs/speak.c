/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speak.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:13 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 11:35:14 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	speak(t_struct *s, int state, int id)
{
	pthread_mutex_lock(s->speak);
	if (state == DONE)
	{
		printf("Max eat count reached\n");
		return (0);
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
	pthread_mutex_unlock(s->speak);
	return (0);
}
