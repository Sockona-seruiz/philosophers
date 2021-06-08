/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:16 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 16:07:14 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	check_args(t_struct *s)
{
	if (s->philo_nb <= 0)
		return (ft_error("Invalid number of philosophers"));
	if (s->ttdie <= 0)
		return (ft_error("Invalid time to die"));
	if (s->tteat <= 0)
		return (ft_error("Invalid time to eat"));
	if (s->ttsleep <= 0)
		return (ft_error("Invalid time to sleep"));
	if (s->total_eat < 0)
		return (ft_error("Invalid number of time each philosophers must eat"));
	return (0);
}

int	set_shared_var(int argc, char **argv, t_struct *s)
{
	if (argc != 5 && argc != 6)
		return (ft_error("Invalid number of arguments"));
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
	s->done = 0;
	s->eat_count = wrmalloc(sizeof(int) * (s->philo_nb));
	s->last_meal_t = wrmalloc(sizeof(uint64_t) * (s->philo_nb));
	if (s->eat_count == NULL || s->last_meal_t == NULL)
		return (ft_error("Malloc faillure\n"));
	return (0);
}

int	init_struct(t_struct *s, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < s->philo_nb)
	{
		philos[i].s = s;
		philos[i].id = i + 1;
		philos[i].state = THINK;
		s->eat_count[i] = 0;
		i++;
	}
	sem_unlink(SEM_FORKS_NAME);
	sem_unlink(SEM_SPEAK_NAME);
	sem_unlink(SEM_WRITE_NAME);
	s->sem_forks = sem_open(SEM_FORKS_NAME, O_CREAT, 0660, s->philo_nb);
	s->sem_speak = sem_open(SEM_SPEAK_NAME, O_CREAT, 0660, 1);
	s->sem_write = sem_open(SEM_WRITE_NAME, O_CREAT, 0660, 1);
	if (s->sem_forks == SEM_FAILED || s->sem_speak == SEM_FAILED
		|| s->sem_write == SEM_FAILED)
		return (ft_error("Semaphore faillure"));
	return (0);
}
