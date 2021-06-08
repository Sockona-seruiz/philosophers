/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:16 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 17:22:34 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	check_args(t_struct *s)
{
	if (s->philo_nb <= 0)
		return (ft_error("Invalid number of philosophers", s));
	if (s->ttdie <= 0)
		return (ft_error("Invalid time to die", s));
	if (s->tteat <= 0)
		return (ft_error("Invalid time to eat", s));
	if (s->ttsleep <= 0)
		return (ft_error("Invalid time to sleep", s));
	if (s->total_eat < 0)
		return (ft_error(
				"Invalid number of time each philosophers must eat",
				s));
	return (0);
}

int	set_shared_var(int argc, char **argv, t_struct *s)
{
	s->eat_count = NULL;
	s->last_meal_t = NULL;
	if (argc != 5 && argc != 6)
		return (ft_error("Invalid number of arguments", s));
	s->philo_nb = ft_atoi(argv[1]);
	s->ttdie = ft_atoi(argv[2]);
	s->tteat = ft_atoi(argv[3]);
	s->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		s->total_eat = ft_atoi(argv[5]);
	else
		s->total_eat = 0;
	if (check_args(s) == 1)
		return (1);
	s->eat_count = malloc(sizeof(int) * (s->philo_nb));
	s->last_meal_t = malloc(sizeof(uint64_t) * (s->philo_nb));
	s->forks = wrmalloc(sizeof(pthread_mutex_t) * (s->philo_nb));
	s->speak = wrmalloc(sizeof(pthread_mutex_t));
	s->write = wrmalloc(sizeof(pthread_mutex_t));
	if (s->eat_count == NULL
		|| s->last_meal_t == NULL || s->forks == NULL
		|| s->speak == NULL || s->write == NULL)
		return (ft_error("Malloc faillure\n", s));
	return (0);
}

void	init_struct(t_struct *s, t_philo *philos)
{
	int	i;

	i = 0;
	pthread_mutex_init(s->speak, NULL);
	pthread_mutex_init(s->write, NULL);
	while (i < s->philo_nb)
	{
		philos[i].s = s;
		philos[i].id = i + 1;
		philos[i].state = THINK;
		s->eat_count[i] = 0;
		pthread_mutex_init(&(s->forks[i]), NULL);
		i++;
	}
}
