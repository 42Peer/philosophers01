#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "philosophers.h"


int cnt = 0;

void *count(void *arg){
    int i;
    t_main	*name = (t_main *)arg;
	printf("WHATTT %s\n",name->str);
	pthread_mutex_lock(name->mutex);
    //======== critical section =============
    cnt = 0;
    for (i = 0; i < 10; i++)
    {
        printf("%s cnt: %d\n", name->str, cnt);
        cnt++;
        //usleep(1);
    }
    //========= critical section ============
	pthread_mutex_unlock(name->mutex);
}

int main()
{
    pthread_t thread1,thread2;
	pthread_mutex_t mutex;
    
	t_main philo0;
	t_main philo1;
	t_main philo2;
	philo0.mutex = &mutex;
	philo1.mutex = &mutex;
	philo2.mutex = &mutex;
	philo0.str = "thread_main";
	philo1.str = "thread_created1";
	philo2.str = "thread_created2";


    pthread_mutex_init(&mutex,NULL);

    pthread_create(&thread1, NULL, count, (void *)&philo1);
    pthread_create(&thread2, NULL, count, (void *)&philo2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

	count((void *) &philo0);
    pthread_mutex_destroy(&mutex);
}