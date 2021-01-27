#include "main.hpp"
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signo)
{
    (void)signo;
    while(1)
    {
        waitpid(-1, NULL, WNOHANG);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("start with: ./svr [ip] [port]\n");
        return 0;
    }

    signal(SIGCHLD, sigcb);

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    
    TcpSvr ts;
    if(!ts.CreateSocket())
    {
        return 0;
    }

    if(!ts.Bind(ip, port))
    {
        return 0;
    }

    if(!ts.Listen())
    {
        return 0;
    }

    while(1)
    {
        TcpSvr newts;
        struct sockaddr_in peeraddr;
        if(!ts.Accept(&peeraddr, &newts))
        {
            continue;
        }
        printf("A new connection: ip= %s, port= %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        int pid = fork(); // 创建子进程
        if(pid < 0)
        {
            perror("fork");
            return 0;
        }
        else if(pid == 0)
        {
            while(1)
            {
                std::string buf;
                newts.Recv(&buf);
                printf("Client: %s\n", buf.c_str());

                printf("reply message: ");
                fflush(stdout);
                std::cin >> buf;
                newts.Send(buf);
            }
            newts.Close();
            exit(1);
        }
        else
        {
            newts.Close();
        }
    }
    return 0;
}
