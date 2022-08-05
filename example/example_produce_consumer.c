// (출처) https://reakwon.tistory.com/m/99
//
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define Q_MAX       3 + 1

// ================ queue implementation =======================
typedef struct queue_t {
        int             data[Q_MAX];
        int             nitem;
        int             head;
        int             tail;

        pthread_mutex_t mutex;
        pthread_cond_t  c_cond;
        pthread_cond_t  p_cond;

}queue_t;

queue_t queue={
        .p_cond=PTHREAD_COND_INITIALIZER,
        .c_cond=PTHREAD_COND_INITIALIZER,
        .mutex=PTHREAD_MUTEX_INITIALIZER
};

int is_empty( ){
        return queue.nitem == 0;
}

int is_full( ){
        return queue.nitem ==  Q_MAX-1;
}

void put( int d ){

        queue.data[queue.head] = d;
        queue.head = (queue.head+1)%Q_MAX;
        queue.nitem++;
}

int get(){

        int temp =  queue.data[queue.tail];
        queue.tail = (queue.tail+1)%Q_MAX;
        queue.nitem--;
        return temp;
}

// ==============queue implementation end ====================

void * produce(void *arg)
{

        int i=0;
        while(1)
		{

                pthread_mutex_lock(&queue.mutex);

                if(is_full()){
                        pthread_cond_wait(&queue.c_cond,&queue.mutex);
                }

                printf("produce:%d\n",i);

                put(i);
                pthread_cond_signal(&queue.p_cond);

                pthread_mutex_unlock(&queue.mutex);
                i++;

                if(i==100) break;

				usleep(rand()%1000);
        }
        return 0;

}

void * consume(void *arg)
{
        while(1)
		{

                pthread_mutex_lock(&queue.mutex);

                if(is_empty()){
                        pthread_cond_wait(&queue.p_cond,&queue.mutex);
                }
                int item=get();

                pthread_cond_signal(&queue.c_cond);
                printf("\t\tconsume:%d\n",item);

                pthread_mutex_unlock(&queue.mutex);

				usleep(rand()%1000);
				if (item == 100) break ;
        }
        return 0;

}

int main(void)
{

        int     n;
        pthread_t producer, consumer;
        srand(time(0));

        pthread_create(&producer, 0, &produce, 0);
        pthread_create(&consumer, 0, &consume, 0);

        pthread_join(producer, 0);
        pthread_join(consumer, 0);


        return 0;
}
