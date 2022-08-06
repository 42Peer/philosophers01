# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

size_t get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

int main()
{
	size_t	start;
	size_t	end;
	size_t	end_time_check;

	start = get_time();
	while (1)
	{
		printf("%zu\n", get_time() - start);
		usleep(557);
	}

	return (0);
}