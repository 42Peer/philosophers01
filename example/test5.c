#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "philosophers.h"


void *count(void *arg){
    int i;
    t_main	*name = (t_main *)arg;

	name->thread = pthread_self();
	printf("tid %d's right_lock || mutex_address : %p\n",name->tid, name->r_mutex);
	printf("tid %d's left_lock || mutex_address : %p\n",name->tid, name->l_mutex);
	//pthread_mutex_lock(name->m_mutex);
	while (1)
	{
		if (name->tid % 2 == 0)
		{
			pthread_mutex_lock(name->r_mutex);
			pthread_mutex_lock(name->l_mutex);
		}
		else
		{
			pthread_mutex_lock(name->l_mutex);
			pthread_mutex_lock(name->r_mutex);
		}
		printf("tid %d is eating \n", name->tid);
		//======== critical section =============
		for (i = 1; i <= 5; i++)
		{
			//printf("tid %d's cnt: %d\n", name->tid, i);
			usleep(1000);
		}
		//========= critical section ============

		pthread_mutex_unlock(name->l_mutex);
		pthread_mutex_unlock(name->r_mutex);
		printf("tid %d is sleeping \n", name->tid);
		usleep(10);
		//pthread_mutex_unlock(name->m_mutex);
	}
}


int main(int argc, char **argv)
{
	int					p_num;
	int					idx;
	t_main				*philo;
	pthread_mutex_t		*mutex_set;
	pthread_mutex_t		mutex_for_lock;

	idx = 0;
	if (argc < 2)
		return (0);
	p_num = ft_atoi(argv[1]);
	mutex_set = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * ft_atoi(argv[1]));
	philo = (t_main *)malloc(sizeof(t_main) * p_num);
	if (philo == NULL || mutex_set == NULL)
		return (0);
	pthread_mutex_init(&mutex_for_lock, NULL);
	while (idx < p_num)
	{
		pthread_mutex_init(&mutex_set[idx], NULL);
		pthread_create(&philo[idx].thread, NULL, count, (void *) &philo[idx]);
		philo[idx].tid = idx;
		philo[idx].l_mutex = &mutex_set[(p_num - 1 + idx) % p_num];
		philo[idx].r_mutex = &mutex_set[idx];
		philo[idx].m_mutex = &mutex_for_lock;
		idx++;
	}
	idx = 0;
	while(idx < p_num)
		pthread_join(philo[idx++].thread, NULL);
	
	idx = 0;
	while(idx < p_num)
		pthread_mutex_destroy(&mutex_set[idx++]);

	// doubly circular linked list 
	printf("main() 종료\n");

	return (0);	
}
