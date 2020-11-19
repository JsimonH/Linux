#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

#define shm_key 0x12345678
int main()
{
    //1.创建共享内存
    int shmid = shmget(shm_key, 1024, IPC_CREAT | 0664);
    if(shmid < 0)
    {
        perror("shmget");
        return 0;
    }

    //2.将进程附加到共享内存上
    void* addr = shmat(shmid, NULL, 0); //传递NULL值,默认操作系统为我们分配地址
    if(!addr)
    {
        perror("shmat");
        return 0;
    }

    //while(1)
    {
        printf("readshm: \"%s\"\n", (char*)addr);
        sleep(2);
    }

    //3.从共享内存中分离进程
    shmdt(addr);
    
    //4.销毁共享内存
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
