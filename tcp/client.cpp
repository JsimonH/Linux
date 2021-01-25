#include "main.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Start with: ./cli [svr_ip] [svr_port]\n");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSvr ts;
    if(!ts.CreateSocket())
    {
        return 0;
    }

    if(!ts.Connect(ip, port))
    {
       return 0; 
    }

    while(1)
    {
        printf("please enter msg: ");
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        ts.Send(buf);

        ts.Recv(&buf);
        printf("Server: %s\n", buf.c_str());
    }
    ts.Close();
    return 0;
}
