#include "main.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svr [ip] [port]");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    
    UdpSvr us;
    if(!us.create_socket())
    {
        return 0;
    }

    if(!us.Bind(ip, port))
    {
        return 0;
    }

    while(1)
    {
        // 接收数据
        std::string buf;
        struct sockaddr_in peeraddr;
        us.recv_data(&buf, &peeraddr);
        printf("Client: %s\n", buf.c_str());

        // 发送数据
        printf("Server: ");
        fflush(stdout);
        std::cin >> buf;
        us.send_data(buf, &peeraddr);
    }

    us.Close();
    return 0;
}
