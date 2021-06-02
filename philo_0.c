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

//Eat -> Sleep -> Think
//Pour manger un philo doit passer dans la fonction pour lock ses deux fourchettes
//Si celle-ci est déjà lock => un autre utilise ces même fourchettes pour manger
//il attend puis les lock à son tours dés que possible

typedef struct	s_struct
{
	int				tteat;
	int				ttdie;
	int				ttsleep;
	int				total_eat;
	int				philo_nb;
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

void	custom_usleep(t_philo *philo, int delay)
{
	uint64_t	i;
	uint64_t	j;
	int			true_delay;

	i = get_time();
	j = 0;
	while (j < delay)
	{
		usleep(10);
		j = get_time();
		j = j - i;
	}
}

void	speak(t_philo *philo, int state)
{
	pthread_mutex_lock(philo->s->speak);
	printf("%llu %d ", get_time() - philo->s->start_time, philo->id);
	if (state == FORK)
		printf("has taken a fork\n");
	else if (state == EAT)
		printf("is eating\n");
	else if (state == SLEEP)
		printf("is sleeping\n");
	else if (state == THINK)
		printf("is thinking\n");
	else
		printf("died\n");
	pthread_mutex_unlock(philo->s->speak);
}

int	pick_fork(t_philo *philo, int i)
{
	int	second_fork;

	pthread_mutex_lock(&(philo->s->forks[i]));
	philo->s->forks_status[i] = LOCK;
	speak(philo, FORK);
	if (i + 1 == philo->s->philo_nb)
		second_fork = 0;
	else
		second_fork = i + 1;
	while (philo->s->forks_status[second_fork] == LOCK)
	{
		if ((philo->end_think_time - philo->begin_think_time) > philo->s->ttdie)
					speak(philo, DEAD);
	}
	pthread_mutex_lock(&(philo->s->forks[second_fork]));
	philo->s->forks_status[second_fork] = LOCK;
	speak(philo, FORK);
	philo->end_think_time = get_time();
	//printf("philo %d think delay = %llu\n", philo->id, philo->end_think_time - philo->begin_think_time);
	philo->eat_count++;
	philo->state = EAT;
	speak(philo, EAT);
	//printf("philo %d is eating\n", i + 1);
	philo->begin_think_time = get_time();
	custom_usleep(philo, philo->s->tteat);
	philo->state = SLEEP;
	pthread_mutex_unlock(&(philo->s->forks[second_fork]));
	philo->s->forks_status[second_fork] = FREE;
	pthread_mutex_unlock(&(philo->s->forks[i]));
	philo->s->forks_status[i] = FREE;
	return (1);
}

void	*test_loop(void	*arg)
{
	t_philo		*philo;

	philo = arg;
	philo->begin_think_time = get_time();
	while (philo->state != DEAD)
	{
		if (philo->state == THINK)
		{
			while (philo->s->forks_status[philo->id] == LOCK)
			{
				philo->end_think_time = get_time();
				if ((philo->end_think_time - philo->begin_think_time) > philo->s->ttdie)
				{
					philo->state = DEAD;
					speak(philo, DEAD);
					exit(1);
				}
			}
			if (pick_fork(philo, philo->id - 1) == -1)
			{
				philo->state = DEAD;
				speak(philo, DEAD);
			}
		}
		if (philo->state == SLEEP)
		{
			speak(philo, SLEEP);
			//printf("philo %d is sleeping\n", philo->id);
			custom_usleep(philo, philo->s->ttsleep);
			philo->state = THINK;
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int			i;
	uint64_t	time;

	i = 0;
	s = malloc(sizeof(t_struct));
	s->philo_nb = ft_atoi(argv[1]);
	s->ttdie = ft_atoi(argv[2]);
	s->tteat = ft_atoi(argv[3]);
	s->ttsleep = ft_atoi(argv[4]);
	if (argv[5])
		s->total_eat = ft_atoi(argv[5]);
	printf("there is %d philosophers\n", s->philo_nb);
	s->forks_status = malloc(sizeof(int) * (s->philo_nb));
	s->forks = malloc(sizeof(pthread_mutex_t) * (s->philo_nb));
	s->speak = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(s->speak, NULL);
	philos = malloc(sizeof(t_philo) * (s->philo_nb));
	while (i < s->philo_nb)
	{
		philos[i].s = s;
		philos[i].id = i + 1;
		philos[i].state = THINK;
		philos[i].eat_count = 0;
		s->forks_status[i] = FREE;
		pthread_mutex_init(&(s->forks[i]), NULL);
		i++;
	}

	i = 0;
	s->start_time =  get_time();
	printf("time = %llu\n", time);
	while (i < s->philo_nb)
	{
		philos[i].last_action_time = get_time();
		pthread_create(&(philos[i].th_id), NULL, test_loop, &philos[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (i < s->philo_nb)
	{
		pthread_join(philos[i].th_id, 0);
		i++;
	}
}