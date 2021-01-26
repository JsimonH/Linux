#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>

#define THREADSIZE 4

typedef void* (*handler_t)(int);
class thread_task
{
    public:
        thread_task(int data, handler_t handler)
        {
            _data = data;
            _handler = handler;
        }
        
        ~thread_task()
        {}

        void run()
        {
            _handler(_data);
        }
    
    private:
        int _data;
        handler_t _handler;
};

class threadpool
{
    public:
        threadpool()
        {
            max_capacity = 10;
            init_capacity = THREADSIZE;

            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&cond, NULL);

            // 创建线程
            bool create = create_thread();
            if(!create)
            {
                printf("create thread failed\n");
                exit(0);
            }

            IsExit = false;
            cur_threadcnt = THREADSIZE;
        }

        ~threadpool()
        {}

        bool push(thread_task* tt)
        {
            pthread_mutex_lock(&lock);
            if(IsExit)
            {
                pthread_mutex_unlock(&lock);
                return false;
            }
            q.push(tt);
            pthread_mutex_unlock(&lock);

            // 插入数据以后,通知线程池中的消费者
            pthread_cond_signal(&cond);
            return true;
        }

        bool pop(thread_task** tt)
        {
            *tt = q.front();
            q.pop();
            return true;
        }

        // 线程等待
        void thread_join()
        {
            for(int i = 0; i < THREADSIZE; i++)
            {
                pthread_join(tid[i], NULL);
            }
        }

        // 线程退出,如果直接退出,有可能队列当中还有数据没有处理完毕
        void thread_exit()
        {
            for(int i = 0; i < THREADSIZE; i++)
            {
                pthread_cancel(tid[i]);
            }
        }

        void threadpool_clear()
        {
            pthread_mutex_lock(&lock);
            IsExit = true;
            pthread_mutex_unlock(&lock);

            if(cur_threadcnt > 0)
            {
                pthread_cond_broadcast(&cond);
            }
        }

    private:
        static void* thread_start(void* arg)
        {
            threadpool* tp = (threadpool*)arg;
            while(1)
            {
                // 从队列当中获取数据进行消费,对于不同线程而言,在获取数据的时候是互斥的
               pthread_mutex_lock(&tp->lock);
               thread_task* tt;
               while(tp->q.empty())
               {
                   if(tp->IsExit)
                   {
                        tp->cur_threadcnt--;
                        pthread_mutex_unlock(&tp->lock);
                        pthread_exit(NULL);
                   }
                   pthread_cond_wait(&tp->cond, &tp->lock);
               }
               tp->pop(&tt);
               pthread_mutex_unlock(&tp->lock);
               // 处理数据
               tt->run();
               delete tt;
            }
        }

        bool create_thread()
        {
            for(int i = 0; i < THREADSIZE; i++)
            {
                int ret = pthread_create(&tid[i], NULL, thread_start, (void*)this);
                if(ret < 0)
                {
                    perror("pthread_create");
                    return false;
                }
            }
            return true;
        }

    private:
        std::queue<thread_task*> q;
        size_t max_capacity;
        pthread_mutex_t lock; // 互斥
        pthread_cond_t cond; // 同步,消费线程的条件变量,但是并没有生产线程的条件变量: 因为客户端的请求行为是我们无法控制的
        size_t init_capacity; // 线程池初始化的时候线程数量
        size_t cur_threadcnt; // 标识当前线程池中的线程数量
        pthread_t tid[THREADSIZE]; // 线程组
        bool IsExit; // 标志是否可以退出
};

void* deal_data(int data)
{
    printf("consume data: %d\n", data);
    return NULL;
}

int main()
{
    threadpool* tp = new threadpool();

    for(int i = 0; i <= 50; i++)
    {
        thread_task* tt = new thread_task(i, deal_data);
        tp->push(tt);
    }

    // 等待线程池中线程退出
    sleep(10);
    tp->threadpool_clear();
    tp->thread_join();
    delete tp;
    return 0;
}
