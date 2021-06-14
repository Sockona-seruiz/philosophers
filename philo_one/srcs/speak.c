/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speak.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:13 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/14 11:00:59 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	ft_putnbr_space(int nb, int fd)
{
	int	i;
	int	tab[10];

	i = 0;
	while (nb >= 10)
	{
		tab[i] = nb % 10 + '0';
		nb = nb / 10;
		i++;
	}
	tab[i] = nb + '0';
	while (i >= 0)
	{
		write(fd, &tab[i], 1);
		i--;
	}
	write(fd, " ", 1);
}

int	speak(t_struct *s, int state, int id)
{
	pthread_mutex_lock(s->speak);
	if (state == DONE)
	{
		ft_putstr("Max eat count reached\n");
		return (0);
	}
	ft_putnbr_space(get_time() - s->start_time, 1);
	ft_putnbr_space(id, 1);
	if (state == FORK)
		ft_putstr(" has taken a fork\n");
	else if (state == EAT)
		ft_putstr(" is eating\n");
	else if (state == SLEEP)
		ft_putstr(" is sleeping\n");
	else if (state == THINK)
		ft_putstr(" is thinking\n");
	else if (state == DEAD)
	{
		ft_putstr(" died\n");
		return (1);
	}
	pthread_mutex_unlock(s->speak);
	return (0);
}
