#include <sys/_types.h>
#include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>

void action(int idx)
{
    printf("fork %d start!\n", idx);
	exit(1);
}

int	main(int argc, char **argv)
{
	//t_philo		philo;
	pid_t	pid;
	int		idx;

	idx = 0;
	while (idx < 3)
	{
		pid = fork();
		if (pid == 0)
			action(idx);
		++idx;
	}
	return (0);
}