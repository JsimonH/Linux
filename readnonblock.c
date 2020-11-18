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

    //设置读端描述符为非阻塞属性
    int readflag = fcntl(fd[0], F_GETFL);
    readflag |= O_NONBLOCK;
    fcntl(fd[0], F_SETFL, readflag);
    
    close(fd[1]); //关闭写端,read正常返回
    
    char buf[1024] = {0};
    int readsize = read(fd[0], buf, sizeof(buf) - 1);
    printf("readsize = %d\n", readsize); //写端不关闭时,读端进行读,read返回-1,错误提示:资源暂时不可用
    perror("read");

    return 0;
}
