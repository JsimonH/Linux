#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct thread
{
    int thread_No;
    thread()
    {
        thread_No = -1;
    }
}THREAD;

void* thread_start(void* arg)
{
    THREAD* th = (THREAD*)arg;
    while(1)
    {
        printf("this is work thread: %d\n", th->thread_No);
        sleep(1);
    }
    // 最后要在线程入口函数结束的时候，释放堆上开辟的内存，否则会内存泄漏
    delete th;
    return NULL;
}

int main()
{
    pthread_t tid;
    int i = 0;
    for(; i < 4; i++)
    {
        THREAD* threadinfo = new thread(); // 堆上申请内存
        threadinfo->thread_No = i;
        
		int ret = pthread_create(&tid, NULL, thread_start, (void*)threadinfo);
        if(ret < 0)
        {
            perror("create thread failed.");
            return 0; 
        }
    }

    while(1)
    {
        printf("this is main thread.\n");
        sleep(1);
    }
    return 0;
}
