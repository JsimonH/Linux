#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        perror("pipe");
        return 0;
    }
    
    //设置写端描述符为非阻塞属性
    int writeflag = fcntl(fd[1], F_GETFL);
    writeflag |= O_NONBLOCK;
    fcntl(fd[1], F_SETFL, writeflag);
    
    //close(fd[0]); //当关闭读端时,非阻塞情况下还一直写,进程会收到SIGPIPE信号,管道破裂,写端进程终止

    int count = 0;
    //一直向管道中写但不进行读;直到管道写满后write会返回-1,错误提示:资源暂时不可用
    while(1)
    {
        int writesize = write(fd[1], "1", 1);
        if(writesize < 0)
        {
            printf("writesize = %d\n", writesize);
            perror("write");
            break;
        }
        count++;
    }
    printf("count = %d\n", count);
    return 0;
}
