#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd[2];
    int ret = pipe(fd); //创建匿名管道返回的文件描述符默认是阻塞的
    if(ret < 0)
    {
        perror("pipe");
        return 0;
    }

    int readflag = fcntl(fd[0], F_GETFL); //获取管道读端描述符当前属性
    printf("readflag = %d\n", readflag);
    int writeflag = fcntl(fd[1], F_GETFL); //获取管道写端描述符当前属性
    printf("writeflag = %d\n", writeflag);
    
    //设置管道读端描述符为非阻塞
    readflag |= O_NONBLOCK; 
    fcntl(fd[0], F_SETFL, readflag);
    printf("O_NONBLOCK: readflag = %d\n", readflag);
    
    //设置管道写端描述符为非阻塞
    writeflag |= O_NONBLOCK; 
    fcntl(fd[1], F_SETFL, writeflag);
    printf("O_NONBLOCK: writeflag = %d\n", writeflag);
    return 0;
}
