#include "philo.h"

void	*action(void *param)
{
	int i = 0;
	while(1)
	{
		printf("%d\n", i++);
		usleep(500000);
	}
	return (NULL);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

int main()
{
	pthread_t t1;
	size_t		s;
	size_t		e;
	s = get_time();
	pthread_create(&t1, NULL, &action, NULL);
	while (1)
	{
		if (get_time() - s >= 3000)
			break ;
	}
	printf("detach start\n");
	pthread_detach(t1);
	printf("detach end\n");
	system("leaks -q a.out");
	return (0);
}