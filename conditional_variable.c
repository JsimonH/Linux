#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int g_bowl = 0; // 0表示"碗中没有饭", 1表示"碗中有饭"

pthread_mutex_t lock; // 互斥锁
pthread_cond_t  cond; // 条件变量

void* producer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        if(g_bowl == 1)
        {
            pthread_cond_wait(&cond, &lock);
        }
        g_bowl++;
        printf("I am producer, I made a meal: %d\n", g_bowl);
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

void* consumer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        if(g_bowl == 0)
        {
            pthread_cond_wait(&cond, &lock);
        }
        g_bowl--;
        printf("I am consumer, I had a meal: %d\n", g_bowl);
        sleep(1);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t tid[2];
    int ret = pthread_create(&tid[0], NULL, producer, NULL);
    if(ret < 0)
    {
        perror("create thread failed");
        return 0;
    }

    ret = pthread_create(&tid[1], NULL, consumer, NULL);
    if(ret < 0)
    {
        perror("create thread failed");
        return 0;
    }

    for(int i = 0; i < 2; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
