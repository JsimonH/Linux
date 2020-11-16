#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
    printf("[minishell@localhost Linux]$ ");
    fflush(stdout);
    while(1)
    {
        //1.从标准输入获取命令
        char buf[1024] = {0};
        fgets(buf, 1023, stdin);
        buf[strlen(buf)-1] = '\0';
        
        //2.对命令行参数进行解析
        char* argv[32] = {NULL};
        int argc = 0;
        char* ptr = buf;
        // [   ls   -a    -l   ]
        while(*ptr != '\0')
        {    
            if(*ptr != ' ')
            {
                argv[argc] = ptr;
                argc++;
                while(*ptr != ' ' && *ptr != '\0')
                {
                    ptr++;
                }
                *ptr = '\0';
            }
            ptr++;
        }
        argv[argc] = NULL;  //最后一个参数的下一个位置设置为NULL
        
        //3.创建子进程
        //4.在子进程中程序替换
        pid_t pid = fork();
        if(pid == 0)
        {
            execvp(argv[0], (char**)argv);
            perror("execvp error");
            exit(0);
        }
        
        //5.进程等待
        wait(NULL);
        return 0;
    }    
}
