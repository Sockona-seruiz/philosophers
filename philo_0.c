#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define DEAD 4

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
	pthread_mutex_t	*forks;
}				t_struct;

typedef struct	s_philo
{
	int				state;
	int				eat_count;
	pthread_t		id;
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

void	*loop(void	*arg)
{
	t_struct	*s;
	int			i;

	i = 0;
	s = arg;
	while (42)
	{
		while (i < s->philo_nb)
		{
			//change_state(s);
			i--;
		}
		i = 0;
	}
}

void	*test_loop(void	*arg)
{
	t_philo	*philo;
	static	int nbr = 0;


	philo = arg;
	nbr++;
	int i = nbr;
	//pthread_mutex_lock(&(philo->s->forks[0]));
	while (1)
	{
		printf("nbr : %d 000015433\n", i);
	}
	//pthread_mutex_unlock(&(philo->s->forks[0]));
	/*
	pthread_mutex_lock(&(s->forks[0]));
	i = 50;
	while (i > 0)
	{
		printf("%d\n", i);
		i--;
	}
	pthread_mutex_unlock(&(s->forks[0]));
	*/
	return (0);
}

//comment identifier un philosopher en foction de son thread?

int	main(int argc, char **argv)
{
	t_struct	*s;
	t_philo		*philos;
	int		i;

	i = 0;
	s = malloc(sizeof(t_struct));
	s->philo_nb = ft_atoi(argv[1]);
	printf("there is %d philosophers\n", s->philo_nb);
	s->forks = malloc(sizeof(pthread_mutex_t) * (s->philo_nb));
	philos = malloc(sizeof(t_philo) * (s->philo_nb));
	while (i < s->philo_nb)
	{
		philos[i].s = s;
		pthread_mutex_init(&(s->forks[i]), NULL);
		i++;
	}

	i = 0;

	while (i < s->philo_nb)
	{
		pthread_create(&(philos[i].id), NULL, test_loop, &philos[i]);
		i++;
	}
	i = 0;
	while (i < s->philo_nb)
	{
		pthread_join(philos[i].id, 0);
		i++;
	}
}