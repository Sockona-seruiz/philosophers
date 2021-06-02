#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DEAD 4
# define FREE 5
# define LOCK 6

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}	                t_list;

typedef struct	s_struct
{
	int				tteat;
	int				ttdie;
	int				ttsleep;
	int				total_eat;
	int				philo_nb;
    int             *eat_count;
	uint64_t		start_time;
	uint64_t		actual_time;
	pthread_mutex_t	*speak;
	pthread_mutex_t	*forks;
	int				*forks_status;
}				t_struct;

typedef struct	s_philo
{
	int				state;
	int				eat_count;
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