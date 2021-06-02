#include "philo_0.h"

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
	{
		printf("died\n");
		ft_exit(philo);
	}
	pthread_mutex_unlock(philo->s->speak);
}

int	check_eat_count(t_philo *philo)
{
	int	i;
	int	ret;

	i = 0;
	ret = 1;
	while (i < philo->s->philo_nb)
	{
		if (&(philo->s->eat_count[i]) < philo->s->eat_count)
			ret = 0;
		i++;
	}
	return (ret);
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
		philo->end_think_time = get_time();
		if ((philo->end_think_time - philo->begin_think_time) > philo->s->ttdie)
		{
			speak(philo, DEAD);
			exit (1);
		}
	}
	pthread_mutex_lock(&(philo->s->forks[second_fork]));
	philo->s->forks_status[second_fork] = LOCK;
	speak(philo, FORK);
	philo->end_think_time = get_time();

	philo->s->eat_count[i]++;
	/*
	if (check_eat_count(philo) == 1)
		ft_exit(philo);
	*/
	speak(philo, EAT);

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
					speak(philo, DEAD);
			}
			if (pick_fork(philo, philo->id - 1) == -1)
				speak(philo, DEAD);
		}
		if (philo->state == SLEEP)
		{
			speak(philo, SLEEP);
			custom_usleep(philo, philo->s->ttsleep);
			philo->state = THINK;
		}
	}
	return (0);
}

int	ft_error(char *message)
{
	printf("Error : %s\n", message);
	return (1);
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
	return (0);
}

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int			i;
	uint64_t	time;

	i = 0;
	s = wrmalloc(sizeof(t_struct));
	if (set_shared_var(argc, argv, s) == 1)
	{
		wrdestroy();
		return (1);
	}
	s->forks_status = wrmalloc(sizeof(int) * (s->philo_nb));
	s->eat_count = wrmalloc(sizeof(int) * (s->philo_nb));
	s->forks = wrmalloc(sizeof(pthread_mutex_t) * (s->philo_nb));
	s->speak = wrmalloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(s->speak, NULL);
	philos = wrmalloc(sizeof(t_philo) * (s->philo_nb));
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
	while (i < s->philo_nb)
	{
		philos[i].last_action_time = get_time();
		pthread_create(&(philos[i].th_id), NULL, test_loop, &philos[i]);
		usleep(50);
		i++;
	}
	i = 0;
	while (i < s->philo_nb)
	{
		pthread_join(philos[i].th_id, 0);
		i++;
	}
}