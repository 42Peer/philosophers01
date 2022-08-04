#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *routine() {
	printf("PTHREAD START!\n");
	sleep(3);
	printf("END PTHREAD!\n");
}

int main(int argc, char **argv)
{
	pthread_t t1, t2, t3;

	char *a;
	char c;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_create(&t3, NULL, &routine, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	routine();
	return (0);
}
