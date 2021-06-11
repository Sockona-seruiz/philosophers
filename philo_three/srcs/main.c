/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:10 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/11 14:37:30 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

//Philo with forks and semaphores

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int			exit_status;
	int			i;
	int			ret;

	i = 0;
	ret = 0;
	s = NULL;
	philos = NULL;
	s = malloc(sizeof(t_struct));
	if (set_shared_var(argc, argv, s) == 1)
		return (1);
	philos = malloc(sizeof(t_philo) * (s->philo_nb));
	if (init_struct(s, philos) == 1)
		return (1);
	i = 0;
	while (i < s->philo_nb)
	{
		s->start_time = get_time();
		philos[i].pid = fork();
		if (philos[i].pid == 0)
		{
			routine_loop(&philos[i]);//lancer la routine du philo qui lancera la routine de son monitoring
			exit (1);
		}
		//pthread_create(&(philos[i].th_id), NULL, routine_loop, &philos[i]);
		usleep(50);
		i++;
	}
	while (1)
	{
		waitpid(-1, &exit_status, 0);
		if (WEXITSTATUS(exit_status) == 1)
			break;
	}
	printf("DEAD===============");
	//ret = monitoring_loop(s);
	return (ft_exit(s, ret, philos));
}
