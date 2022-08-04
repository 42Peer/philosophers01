#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_arg
{
	int	philo_n;
	int	life_t;
	int	eat_t;
	int	sleep_t;
	int	eat_cnt;
}	t_arg;

typedef struct s_info
{
	t_arg		arg;
	pthread_t	*p;
	t_mutex		*mutex;
	int		flag;
}	t_info;

enum e_action
{
	FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

#endif
