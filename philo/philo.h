#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define INT_MAX			2147483647
# define INT_MIN			-2147483648

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
	t_arg			arg;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	int				flag;
}	t_info;

enum e_action
{
	ERROR = -1,
	SUCCESS,
	FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

#endif
