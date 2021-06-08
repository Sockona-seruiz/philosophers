/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:53:01 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 11:02:25 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	ft_error(char *message)
{
	wrdestroy();
	printf("Error : %s\n", message);
	return (1);
}

int	ft_exit(t_struct *s, int ret)
{
	int	i;

	i = 0;
	while (i < s->philo_nb)
	{
		pthread_mutex_destroy(&(s->forks[i]));
		i++;
	}
	pthread_mutex_destroy(s->write);
	pthread_mutex_destroy(s->speak);
	wrdestroy();
	if (ret == 2)
		return (0);
	return (ret);
}
