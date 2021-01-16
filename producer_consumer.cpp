#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>

#define THREADSIZE 2

class BlockQueue
{
    public:
        BlockQueue(size_t capacity)
        {
            _capacity = capacity;
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&producer, NULL);
            pthread_cond_init(&consumer, NULL);
        }
        
        void push(int& data)
        {
            pthread_mutex_lock(&lock);
            while(isFull())
            {
                pthread_cond_wait(&producer, &lock);
            }
            q.push(data);
            pthread_mutex_unlock(&lock);
            pthread_cond_signal(&consumer);
        }

        void pop(int* data)
        {
            pthread_mutex_lock(&lock);
            while(q.empty())
            {
                pthread_cond_wait(&consumer, &lock);
            }
            *data = q.front();
            q.pop();
            pthread_mutex_unlock(&lock);
            pthread_cond_signal(&producer);
        }

        bool isFull()
        {
            if(q.size() == _capacity)
            {
                return true;
            }
            return false;
        }

        ~BlockQueue()
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&producer);
            pthread_cond_destroy(&consumer);
        }

    private:
        std::queue<int> q;
        size_t _capacity;
        pthread_mutex_t lock;
        pthread_cond_t producer;
        pthread_cond_t consumer;
};

void* producer_start(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    int i = 0;
    while(1)
    {
        bq->push(i);
        printf("producer[%p]: %d\n", pthread_self(), i);
        sleep(1);
        i++;
    }
    return NULL;
}

void* consumer_start(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    while(1)
    {
        int data;
        bq->pop(&data);
        printf("consumer[%p]: %d\n", pthread_self(), data);
        sleep(1);
    }
    return NULL;
}

int main()
{
    BlockQueue* bq = new BlockQueue(10);

    pthread_t producer_tid[THREADSIZE];
    pthread_t consumer_tid[THREADSIZE];

    int i = 0;
    for(; i < THREADSIZE; i++)
    {
        int ret = pthread_create(&producer_tid[i], NULL, producer_start, (void*)bq);
        if(ret < 0)
        {
            perror("create thread failed");
            return 0;
        }

        ret = pthread_create(&consumer_tid[i], NULL, consumer_start, (void*)bq);
        if(ret < 0)
        {
            perror("create thread failed");
            return 0;
        }
    }
    
    for(i = 0 ; i < THREADSIZE; i++)
    {
        pthread_join(producer_tid[i], NULL);
        pthread_join(consumer_tid[i], NULL);
    }

    delete bq;
    bq = NULL;
    return 0;
}
