/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 10:53:01 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/09 14:25:08 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	ft_free(t_struct *s, t_philo *philos)
{
	if (s->eat_count != NULL)
		free(s->eat_count);
	if (s->last_meal_t != NULL)
		free(s->last_meal_t);
	if (s->speak != NULL)
		free(s->speak);
	if (s->write != NULL)
		free(s->write);
	if (s->forks != NULL)
		free(s->forks);
	if (s != NULL)
		free(s);
	if (philos != NULL)
		free(philos);
}

int	ft_error(char *message, t_struct *s)
{
	ft_free(s, NULL);
	printf("Error : %s\n", message);
	return (1);
}

int	ft_exit(t_struct *s, int ret, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < s->philo_nb)
	{
		pthread_mutex_destroy(&(s->forks[i]));
		pthread_detach(philos[i].th_id);
		i++;
	}
	ft_free(s, philos);
	pthread_mutex_destroy(s->write);
	pthread_mutex_destroy(s->speak);
	return (ret);
}
