# ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>

typedef struct s_main 
{
	int				tid;
	pthread_t		thread;
	pthread_mutex_t *r_mutex;
	pthread_mutex_t *m_mutex;
	pthread_mutex_t *l_mutex;
}					t_main;

int	ft_atoi(const char *str);


# endif