#include <stdio.h>    
#include <unistd.h>    
    
int main()    
{    
    pid_t rpid = fork();    
    if(rpid < 0)    
    {    
        perror("fork");    
        return 0;    
    }    
    else if(rpid == 0)    
    {    
        //子进程        
        printf("this is child process: %d\n", getpid()); //getpid()用来获取当前进程的标识符,即进程id
		        
        //当父进程先于子进程退出,子进程会成为"孤儿进程"; 被1号进程领养,目的是当子进程退出时回收系统资源
        //1号进程是操作系统启动的第一个进程,很多进程都是被1号进程创建的
		while(1)    
        {    
            sleep(1);    
            printf("parent process exited..\n");   
        }    
    }        
    else    
    {    
        //父进程
        printf("this is parent process: %d\n", getpid());
        sleep(1);
	} 
    return 0;    
}
