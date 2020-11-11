#include <stdio.h>
#include <unistd.h>

int main()
{
    //使用系统调用fork创建进程. 成功:父进程返回子进程的pid,子进程返回0; 失败:返回<0的数
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
    }
    sleep(1);
    return 0;
}
