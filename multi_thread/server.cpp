#include "main.hpp"
#include <pthread.h>

void* thread_start(void* arg)
{
    pthread_detach(pthread_self());
    TcpSvr* ts = (TcpSvr*)arg;
    while(1)
    {
        std::string buf;
        ts->Recv(&buf);
        printf("Client: %s\n", buf.c_str());

        printf("reply message: ");
        fflush(stdout);
        std::cin >> buf;
        ts->Send(buf);
    }
    ts->Close();
    delete ts;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("start with: ./svr [ip] [port]\n");
        return 0;
    }

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
        TcpSvr* newts = new TcpSvr();
        struct sockaddr_in peeraddr;
        if(!ts.Accept(&peeraddr, newts))
        {
            continue;
        }
        printf("A new connection: ip= %s, port= %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thread_start, (void*)newts);
        if(ret < 0)
        {
            perror("pthread_create");
            return 0;
        }
    }
    return 0;
}
