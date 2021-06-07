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

int	speak(t_struct *s, int state, int id)
{
	pthread_mutex_lock(s->speak);
	if (state == DONE)
	{
		printf("Max eat count reached\n");
		return (2);
	}
	printf("%llu %d ", get_time() - s->start_time, id);
	if (state == FORK)
		printf("has taken a fork\n");
	else if (state == EAT)
		printf("is eating\n");
	else if (state == SLEEP)
		printf("is sleeping\n");
	else if (state == THINK)
		printf("is thinking\n");
	else if (state == DEAD)
	{
			printf("died\n");
			return (1);
	}
	pthread_mutex_unlock(s->speak);
	return (0);
}

uint64_t	set_get_last_meal_time(t_struct *s, int	sw, int id)
{
	uint64_t	ret;

	pthread_mutex_lock(s->write);
	if (sw != -1)
	{
		s->last_meal_t[id] = get_time();
		pthread_mutex_unlock(s->write);
		return (0);
	}
	s->actual_time = get_time();
	ret = s->last_meal_t[id];
	pthread_mutex_unlock(s->write);
	return (ret);
}

int	pick_fork(t_philo *philo, int i)
{
	int	second_fork;

	pthread_mutex_lock(&(philo->s->forks[i]));
	speak(philo->s, FORK, philo->id);
	if (i + 1 == philo->s->philo_nb)
		second_fork = 0;
	else
		second_fork = i + 1;
	pthread_mutex_lock(&(philo->s->forks[second_fork]));
	speak(philo->s, FORK, philo->id);
	speak(philo->s, EAT, philo->id);
	set_get_last_meal_time(philo->s, 1, i);
	//philo->s->last_meal_t[i] = get_time();
	custom_usleep(philo, philo->s->tteat);
	philo->s->eat_count[i]++;
	philo->state = SLEEP;
	pthread_mutex_unlock(&(philo->s->forks[second_fork]));
	pthread_mutex_unlock(&(philo->s->forks[i]));
	return (0);
}

void	*test_loop(void	*arg)
{
	t_philo		*philo;
	int			ret;

	philo = arg;
	//philo->begin_think_time = get_time();
	while (philo->state != DEAD)
	{
		if (philo->state == THINK)
			pick_fork(philo, philo->id - 1);
		if (philo->state == SLEEP)
		{
			speak(philo->s, SLEEP, philo->id);
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

int	monitoring_loop(t_struct *s)
{
	int			i;
	int			done_eating;
	uint64_t	time;

	i = 0;
	done_eating = 1;
	/*
	time = get_time();
	printf("Monitoring launched\n");
	printf("start time = %llu\n      time = %llu", s->start_time, time);
	*/
	while (42)
	{
		while (i < s->philo_nb)
		{
			s->actual_time = get_time();
			time = set_get_last_meal_time(s, -1, i);
			if (s->actual_time - time > s->ttdie)
			{
				if (speak(s, DEAD, i + 1) == 1)
					return (1);
			}
			pthread_mutex_unlock(s->write);
			//printf("eat_count[%d] = %d\n", i, s->eat_count[i]);
			if (s->eat_count[i] < s->total_eat)
				done_eating = 0;
			i++;
		}
		if (done_eating == 1 && s->total_eat != 0)
			return (speak(s, DONE, i + 1));
		done_eating = 1;
		i = 0;
		usleep(50);
	}
}

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int			i;
	uint64_t	time;
	pthread_t	monitor_id;
	int			ret;

	i = 0;
	ret = 0;
	s = wrmalloc(sizeof(t_struct));
	if (set_shared_var(argc, argv, s) == 1)
	{
		wrdestroy();
		return (1);
	}
	s->forks_status = wrmalloc(sizeof(int) * (s->philo_nb));
	s->eat_count = wrmalloc(sizeof(int) * (s->philo_nb));
	s->last_meal_t = wrmalloc(sizeof(uint64_t) * (s->philo_nb));
	s->forks = wrmalloc(sizeof(pthread_mutex_t) * (s->philo_nb));
	s->speak = wrmalloc(sizeof(pthread_mutex_t));
	s->write = wrmalloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(s->speak, NULL);
	pthread_mutex_init(s->write, NULL);
	philos = wrmalloc(sizeof(t_philo) * (s->philo_nb));
	while (i < s->philo_nb)
	{
		philos[i].s = s;
		philos[i].id = i + 1;
		philos[i].state = THINK;
		s->eat_count[i] = 0;
		s->forks_status[i] = FREE;
		pthread_mutex_init(&(s->forks[i]), NULL);
		i++;
	}

	i = 0;
	s->start_time =  get_time();
	while (i < s->philo_nb)
	{
		s->last_meal_t[i] = s->start_time;
		i++;
	}
	
	//usleep(50);
	i = 0;
	while (i < s->philo_nb)
	{
		pthread_create(&(philos[i].th_id), NULL, test_loop, &philos[i]);
		usleep(50);
		i++;
	}
	ret = monitoring_loop(s);
	if (ret != 0)
		return (ret);
	/*
	i = 0;
	while (i < s->philo_nb)
	{
		pthread_join(philos[i].th_id, 0);
		i++;
	}
	*/
}