/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:19 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/07 17:32:53 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

	sign = 1;
	result = 0;
	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (48 <= str[i] && str[i] <= 57)
	{
		result = (str[i] - 48) + (result * 10);
		i++;
	}
	return (result * sign);
}

uint64_t	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	custom_usleep(int delay)
{
	uint64_t	i;
	uint64_t	j;

	i = get_time();
	j = 0;
	while (j < (uint64_t)delay)
	{
		usleep(10);
		j = get_time();
		j = j - i;
	}
}

int	ft_error(char *message)
{
	wrdestroy();
	printf("Error : %s\n", message);
	return (1);
}

void	ft_exit(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->s->philo_nb)
	{
		pthread_mutex_destroy(&(philo->s->forks[i]));
		i++;
	}
	wrdestroy();
	exit (0);
}
