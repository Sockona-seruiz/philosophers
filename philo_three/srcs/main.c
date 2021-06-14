/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:10 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/14 13:03:52 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_to_kill(t_struct *s, t_philo *philos)
{
	int	exit_status;
	int	i;

	while (1)
	{
		waitpid(-1, &exit_status, 0);
		if (WEXITSTATUS(exit_status) == 1 || WEXITSTATUS(exit_status) == 0)
		{
			i = 0;
			while (i < s->philo_nb)
				kill(philos[i++].pid, SIGKILL);
			break ;
		}
	}
}

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
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
	while (i < s->philo_nb)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			routine_loop(&philos[i]);
		usleep(50);
		i++;
	}
	wait_to_kill(s, philos);
	return (ft_exit(s, ret, philos));
}
