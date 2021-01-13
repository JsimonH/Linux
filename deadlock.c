#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* thread_start1(void* arg)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock1);
    sleep(3);
    pthread_mutex_lock(&lock2);
    return NULL;
}

void* thread_start2(void* arg)
{
    pthread_detach(pthread_self());
    pthread_mutex_lock(&lock2);
    sleep(3);
    pthread_mutex_lock(&lock1);
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_t tid[2];
    int ret = pthread_create(&tid[0], NULL, thread_start1, NULL);
    if(ret < 0)
    {
        perror("create thread failed");
        return 0;
    }

    ret = pthread_create(&tid[1], NULL, thread_start2, NULL);
    if(ret < 0)
    {
        perror("create thread failed");
        return 0;
    }

	while(1)
    {
        sleep(1);
        printf("this is main thread.\n");
    }

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    return 0;
}
