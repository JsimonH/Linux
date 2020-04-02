#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    int fd = -1;
    umask(0); //将当前进程的文件创建权限掩码设置为0
    fd = open("./test.txt", O_RDWR|O_CREAT,0777);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }
    lseek(fd, 0, SEEK_END);  //读写位置跳转到文件末尾
    char* ptr = "Hello World!!\n";
    int ret = write(fd, ptr, strlen(ptr));
    if (ret <0)
    {
        perror("write error");
        return -1;
    }
    printf("ret:%d\n", ret);

    lseek(fd, 0, SEEK_SET);  //跳转到文件的起始位置 
    char buf[1024] = {0};
    ret = read(fd, buf, 1024);
    if(ret < 0)
    {
       perror("read error");
       return -1;
    }
    printf("ret:%d-[%s]\n", ret, buf);
     
    close(fd);
    return 0;
}
