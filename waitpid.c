#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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
        //child process
        printf("I am child process\n");
        sleep(3);
        exit(65);
    }
    else
    {
        //parent process
        pid_t status;
        while(waitpid(rpid, &status, WNOHANG) == 0);
        if((status & 0x7F) == 0)
        {
            printf("正常退出\n");
            printf("exitcode = %d\n", (status >> 8) & 0xFF);
        }
        else
        {
            printf("异常退出\n");
            printf("codeflag = %d\n", (status >> 7) & 0x1);
        }
        while(1)
        {
            sleep(2);
            printf("I am parent process, I am going to exit\n");
        }
    }
    return 0;
}
