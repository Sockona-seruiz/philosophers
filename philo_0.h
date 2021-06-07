/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_0.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:27:16 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/07 17:27:17 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_0_H
# define PHILO_0_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DEAD 4
# define FREE 5
# define LOCK 6
# define DONE 7

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_struct
{
	int				tteat;
	int				ttdie;
	int				ttsleep;
	int				total_eat;
	int				philo_nb;
	int				*eat_count;
	uint64_t		start_time;
	uint64_t		actual_time;
	uint64_t		*last_meal_t;
	pthread_mutex_t	*speak;
	pthread_mutex_t	*write;
	pthread_mutex_t	*forks;
	int				*forks_status;
}				t_struct;

typedef struct s_philo
{
	int				state;
	int				id;
	uint64_t		last_action_time;
	uint64_t		begin_think_time;
	uint64_t		end_think_time;
	pthread_t		th_id;
	t_struct		*s;
}				t_philo;

void			*wrmalloc(unsigned long size);
int				wrfree(void *ptr);
void			wrdestroy(void);
void			ft_exit(t_philo *philo);
void			custom_usleep(int delay);
uint64_t		get_time(void);
int				ft_atoi(const char *str);
int				monitoring_loop(t_struct *s);
int				ft_error(char *message);
void			*routine_loop(void	*arg);
int				pick_fork(t_philo *philo, int i);
uint64_t		set_get_last_meal_time(t_struct *s, int	sw, int id);
int				speak(t_struct *s, int state, int id);
int				set_shared_var(int argc, char **argv, t_struct *s);
void			init_struct(t_struct *s, t_philo *philos);

#endif