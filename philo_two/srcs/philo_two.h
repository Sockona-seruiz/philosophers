/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seruiz <seruiz@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 17:27:16 by seruiz            #+#    #+#             */
/*   Updated: 2021/06/08 16:03:56 by seruiz           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>

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

# define SEM_FORKS_NAME "/sem_forks"
# define SEM_SPEAK_NAME "/sem_speak"
# define SEM_WRITE_NAME "/sem_write"

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
	int				done;
	int				*eat_count;
	uint64_t		start_time;
	uint64_t		actual_time;
	uint64_t		*last_meal_t;
	sem_t			*sem_forks;
	sem_t			*sem_speak;
	sem_t			*sem_write;
}				t_struct;

typedef struct s_philo
{
	int				state;
	int				id;
	pthread_t		th_id;
	t_struct		*s;
}				t_philo;

void			*wrmalloc(unsigned long size);
int				wrfree(void *ptr);
void			wrdestroy(void);
int				ft_exit(t_struct *s, int ret, t_philo *philos);
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
int				init_struct(t_struct *s, t_philo *philos);

#endif