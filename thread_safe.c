#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADSIZE 4

int g_tickets = 20;
pthread_mutex_t lock;

void* buyTickets(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock); // 加锁
        if(g_tickets <= 0)
        {
            pthread_mutex_unlock(&lock); // 在所有可能退出的地方都要解锁
            break;
        }
        printf("I am thread[%p], I got a ticket: %d\n", pthread_self(), g_tickets);
        sleep(1);
        g_tickets--;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock, NULL); // 初始化互斥锁
    
    pthread_t tid[THREADSIZE];
    int i = 0;
    for(; i < THREADSIZE; i++)
    {
        int ret = pthread_create(&tid[i], NULL, buyTickets, NULL);
        if(ret < 0)
        {
            perror("create thread failed.");
            return 0;
        }
    }
    
    // 线程等待
    for(i = 0; i < THREADSIZE; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    // 销毁互斥锁
    pthread_mutex_destroy(&lock);
    return 0;
}
