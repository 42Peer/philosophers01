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

typedef	struct s_arg
{
	int	philo_n;
	int	life_t;
	int	eat_t;
	int	sleep_t;
	int	eat_cnt;
}		t_arg;

typedef struct s_info
{
	t_arg			arg;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t *prt_mutex;
	int				eat_flag;
	int				die_flag;
}					t_info;

typedef struct s_philo
{
	int			idx;
	int			p_eat_cnt;
	int			life_time;
	t_info		*info;
	pthread_t	tid;
}				t_philo;

enum	e_action
{
	ERROR = -1,
	SUCCESS,
	FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

long long	ft_atoi(const	char	*str);

#endif