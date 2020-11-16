#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    //flags: O_RDONLY 只读打开   O_WRONLY 只写打开  O_RDWR 可读可写 以上三者必选且只能三选一
    //可选项: O_CREAT 创建文件   O_TRUNC  截断(清空)文件  O_APPEND 以追加方式打开文件
    int fd = open("./test.txt", O_RDWR|O_CREAT, 0664);
    if(fd < 0)
    {
        perror("open error");
        return -1;
    }
    
    lseek(fd, 0, SEEK_END);  //读写位置跳转到文件末尾
    
    const char* ptr = "Hello World!";
    int ret = write(fd, ptr, strlen(ptr));
    if (ret < 0)
    {
        perror("write error");
        return -1;
    }
    printf("ret: %d\n", ret);

    lseek(fd, 0, SEEK_SET);  //读写位置跳转到文件开头
    
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
