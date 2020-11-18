#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        perror("pipe");
        return 0;
    }

    int pid = fork(); //创建子进程
    if(pid < 0)
    {
        perror("fork");
        return 0;
    }
    else if(pid == 0)
    {
        //child
        close(fd[0]); //关闭子进程读端
        
        //设置子进程写端描述符为非阻塞属性
        int writeflag = fcntl(fd[1], F_GETFL);
        writeflag |= O_NONBLOCK;
        fcntl(fd[1], F_SETFL, writeflag);
        
        printf("I am child process\n");
        write(fd[1], "hello", 5);
    }
    else
    {
        //parent
        close(fd[0]); //关闭父进程读端
        
        //进程等待,获取子进程退出终止信号
        int status;
        wait(&status);
        printf("termination signal is: %d\n", status & 0x7f);
    }
    return 0;
}
