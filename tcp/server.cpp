#include "main.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Start with: ./svr [ip] [port]\n");
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

    TcpSvr ser_ts;

    while(1)
    {
        struct sockaddr_in clientaddr;
        if(!ts.Accept(&clientaddr, &ser_ts))
        {
            return 0;
        }

        std::string buf;
        ser_ts.Recv(&buf);
        printf("Client: %s\n", buf.c_str());

        printf("reply message: ");
        fflush(stdout);
        std::cin >> buf;
        ser_ts.Send(buf);
    }
    ser_ts.Close();
    ts.Close();
    return 0;
}
