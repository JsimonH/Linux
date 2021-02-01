#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{   
    while(1)
    {
        // 监控0号文件描述符的可读事件
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);

        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(1, &readfds, NULL, NULL, &tv);
        if(ret < 0)
        {
            perror("select");
            return 0;
        }
        else if(ret == 0)
        {
            printf("select timeout....\n");

            // select只会返回已经就绪的文件描述符,因此当监控超时,0号文件描述符应该不在fd_set中
            ret = FD_ISSET(0, &readfds);
            if(ret == 0)
            {
                printf("fd 0 is not in readfds\n");
            }
        }
        else
        {
            printf("fd 0 is in readfds\n");
        }

        if(FD_ISSET(0, &readfds) != 0)
        {
            char buf[1024] = {0};
            read(0, buf, sizeof(buf) - 1);
            printf("buf: %s\n", buf);
        }
        FD_CLR(0, &readfds);
    }
    return 0;
}
