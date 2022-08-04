#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *p_function(void * data)
{
	pthread_t tid; // thread id
	char	*thread_name;
	int		i;

	tid = pthread_self();
	i = 0;
	thread_name = (char *)data;

	while(i < 3)
	{
		printf("%s, tid : %x\n", thread_name, (unsigned int)tid);
		i++;
		usleep(1000 * 1000);
	}
	return (tid);
}

int main(void)
{
	pthread_t t1, t2, t3;
	int thr_id1;
	int thr_id2;
	int thr_id3;
	char p0[] = "thread_main";
	char p1[] = "thread_created1";
	char p2[] = "thread_created2";
	char p3[] = "thread_created3";

	thr_id1 = pthread_create(&t1, NULL, p_function, (void *)p1);
	thr_id2 = pthread_create(&t2, NULL, p_function, (void *)p2);
	thr_id3 = pthread_create(&t3, NULL, p_function, (void *)p3);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	p_function((void *)p0);

	printf("end\n");
	return 0;
}
