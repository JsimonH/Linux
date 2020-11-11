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
    }    
    else    
    {    
        //父进程    
        printf("this is parent process: %d\n", getpid());
		
        //当子进程先于父进程退出,但父进程没有对子进程的资源进行回收管理,此时子进程将会成为"僵尸进程"
        while(1)
        {
            sleep(1);
            printf("child process exited..\n");
        }    
    }                                                                                                                                                               
    return 0;    
}
