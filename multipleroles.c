#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADSIZE 2
pthread_mutex_t lock;
pthread_cond_t producer;
pthread_cond_t consumer;

int g_bowl = 0; // 0表示"碗中没有食物", 1表示"碗中有食物"

void* producer_start(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_bowl == 1)
        {
            pthread_cond_wait(&producer, &lock);
        }
        g_bowl++;
        printf("I am producer[%p]: I made a meal: %d\n", pthread_self(), g_bowl);
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&consumer);
    }
    return NULL;
}

void* consumer_start(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(g_bowl == 0)
        {
            pthread_cond_wait(&consumer, &lock);
        }
        g_bowl--;
        printf("I am consumer[%p]: I had a meal: %d\n", pthread_self(), g_bowl);
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&producer);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&producer, NULL);
    pthread_cond_init(&consumer, NULL);

    pthread_t producer_tid[THREADSIZE];
    pthread_t consumer_tid[THREADSIZE];
    
    int i = 0;
    for(; i < THREADSIZE; i++)
    {
        int ret1 = pthread_create(&producer_tid[i], NULL, producer_start, NULL);
        if(ret1 < 0)
        {
            perror("create thread failed");
            return 0;
        }
    }
    
    for(i = 0; i < THREADSIZE; i++)
    {
        int ret2 = pthread_create(&consumer_tid[i], NULL, consumer_start, NULL);
        if(ret2 < 0)
        {
            perror("create thread failed");
            return 0;
        }
    }

    for(i = 0; i < THREADSIZE; i++)
    {
        pthread_join(producer_tid[i], NULL);
        pthread_join(consumer_tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&producer);
    pthread_cond_destroy(&consumer);
    return 0;
}
