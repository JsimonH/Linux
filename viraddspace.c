#include <stdio.h>
#include <unistd.h>

int a = 10; //定义全局变量a,并初始化为10.
int main()
{
    pid_t rpid = fork();
    if(rpid < 0)
    {
        perror("fork");
        return 0;
    }
    else if(rpid == 0)
    {
        //child
        printf("child: a = %d\n", a);
    }
    else
    {
        //parent
        a = 0; //父进程将a的值修改为0
        printf("parent: a = %d\n", a);
    }
    sleep(1);
    return 0;
}
