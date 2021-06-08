/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:13:10 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 11:15:01 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int			i;
	int			ret;

	i = 0;
	ret = 0;
	s = wrmalloc(sizeof(t_struct));
	if (set_shared_var(argc, argv, s) == 1)
		return (1);
	philos = wrmalloc(sizeof(t_philo) * (s->philo_nb));
	init_struct(s, philos);
	i = 0;
	s->start_time = get_time();
	while (i < s->philo_nb)
	{
		pthread_create(&(philos[i].th_id), NULL, routine_loop, &philos[i]);
		usleep(50);
		i++;
	}
	ret = monitoring_loop(s);
	return (ft_exit(s, ret));
}
