#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int ret = mkfifo("./myfifo", 0664); //创建命名管道
    if(ret < 0)
    {
        perror("mkfifo");
        return 0;
    }
    
    int fd = open("./myfifo", O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        return 0;
    }
    
    char buf[1024] = {0};
    read(fd, buf, sizeof(buf) - 1);
    printf("buf: %s\n", buf);
    close(fd);
    return 0;
}
