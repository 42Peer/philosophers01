#include "philo.h"

int main()
{
	pid_t	pid = 1;

	// printf("hi\n");
	int i = 0;
	while (i < 5)
	{
		if (pid != 0)
			pid = fork();
		if (pid == 0)
		{
			printf("child : %d\n", i);
			break;
		}
		++i;
	}
	return (0);
}