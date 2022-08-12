#include "philo.h"

void action()
{

    exit(1);
}

int main()
{
	int i = 0;
	pid_t	pid;

	while(i < 5)
	{
		pid = fork();
		if (pid == 0)
            action();
        printf("pid : %d\n", pid);
        printf("idx : %d\n", i);
		++i;
	}
	return (0);
}
/*
부모0
ㄴ 자식0

부모1
ㄴ 자식1
*/