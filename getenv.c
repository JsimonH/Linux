#include <stdio.h>
#include <unistd.h>

//三种方式获取环境变量:
#if 0
//1.main函数的参数获取环境变量
int main(int argc, char* argv[], char* env[])
{
    //argc -> 命令行参数个数
    //argv -> 命令行参数的具体内容
    //env  -> 环境变量
    int i;
    for(i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    for(i = 0; env[i]; i++)
    {
        printf("%s\n", env[i]);
    }
    return 0;
}
#endif

#if 0
//2.libc库中提供的变量来获取
int main()
{
    extern char** environ;
    int i;
    for(i = 0; environ[i]; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}
#endif

#include <stdlib.h>
//3.使用libc库中的函数getenv
int main()
{
    //char *getenv(const char *name);
    //name:环境变量的名称        返回值:具体环境变量对应的值
    char* PATH = getenv("PATH");
    printf("%s\n", PATH);
    return 0;
}
