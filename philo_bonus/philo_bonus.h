#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <pthread.h>

# define INT_MAX			2147483647
# define INT_MIN			-2147483648

typedef struct s_status
{
	int	end;
	int	n_full;
}	t_status;

typedef struct s_sem
{
	sem_t	*fork;
	sem_t	*print;
}	t_sem;

typedef struct s_arg
{
	size_t	die_time;
	int		n_philo;
	int		eat_time;
	int		sleep_time;
	int		must_eat;
}	t_arg;

typedef struct s_info
{
	t_arg		arg;
	t_sem		sema;
	t_status 	stat;
	size_t		birth_t;
}	t_info;

typedef struct s_philo
{
	int		pid;
	int 	idx;
	t_info	info;
	size_t	last_eat_t;
}	t_philo;

enum e_enum
{
	ERROR = -1,
	SUCCESS
};

long long	ft_atoi(const	char	*str);
#endif