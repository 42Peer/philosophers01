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
	int		philo_n;
	size_t	life_t;
	size_t	eat_t;
	size_t	sleep_t;
	int		eat_cnt;
}		t_arg;

typedef struct s_flag
{
	int				eat_f;
	int				die_f;
	int				err_f;
	int				start_f;
}					t_flag;

typedef struct s_info
{
	t_arg			arg;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t prt_mutex;
	pthread_mutex_t	t_mutex;
	t_flag			flags;
	size_t			start_time;
}					t_info;

typedef struct s_philo
{
	int			idx;
	int			p_eat_cnt;
	size_t		life_time;
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