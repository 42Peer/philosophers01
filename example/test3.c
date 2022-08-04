#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_main
{
	pthread_mutex_t *mutex;
	char			*str;
}					t_main;


void *t_function(void *param)
{
	t_main	*philo;
	philo = (t_main*)param;
    printf("\n%s : 똑똑 식사 가능한가요? \n", philo->str);
	/* pthread_mutex_lock(philo->mutex); */
	usleep(1000);
    printf("\n%s : 주먹고기 1인분이요~\n", philo->str);
    for (int i = 1; i <= 5; i++)
    {
        printf("%s : 밥 먹는 중..(%d / 5)\n", philo->str, i);
    }
	/* pthread_mutex_unlock(philo->mutex); */
    printf("\n%s : 아 배부르다 잠이나 잘까\n", philo->str);
	return (void *)2147483647;  // result에 담길 값.
}

int main()
{
    pthread_t p_thread1;
	pthread_t p_thread2;


	//정적할당
	/* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
	pthread_mutex_t mutex;
    int result;

	t_main p0;
	t_main p1;
	t_main p2;

	p0.mutex = &mutex;
	p1.mutex = &mutex;
	p2.mutex = &mutex;
	p0.str = "thread_main";
	p1.str = "thread_created1";
	p2.str = "thread_created2";

	pthread_mutex_init(&mutex, NULL);

    pthread_create(&p_thread1, NULL, t_function, (void *)&p1);
	pthread_create(&p_thread2, NULL, t_function, (void *)&p2);

    // 쓰레드 식별자 p_thread 가 종료되길 기다렸다가 
    // 종료후에 리턴값을 받아온다.

	/* pthread_join(p_thread2, (void *)&result); */
	/* pthread_join(p_thread1, (void *)&result); */
	t_function((void *)&p0);

    printf("\nthread join 실행됨: %d\n", result);

    printf("main() 종료\n");
	pthread_mutex_destroy(&mutex);
    return 0;
}
