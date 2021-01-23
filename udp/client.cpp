#include "main.hpp"

// 对于客户端程序而言，命令行参数当中的ip和port需要指定为服务端的ip和端口
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./cli [svr_ip] [svr_port]\n");
        return 0;
    }

    std::string svr_ip = argv[1];
    uint16_t svr_port = atoi(argv[2]);

    UdpSvr us;
    if(!us.create_socket())
    {
        return 0;
    }

    struct sockaddr_in destaddr;
    destaddr.sin_family = AF_INET;
    destaddr.sin_addr.s_addr = inet_addr(svr_ip.c_str());
    destaddr.sin_port = htons(svr_port);
    
    while(1)
    {
        // 发送数据
        printf("Client: ");
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        us.send_data(buf, &destaddr);

        // 接收数据
        struct sockaddr_in peeraddr;
        us.recv_data(&buf, &peeraddr);
        printf("Server: %s\n", buf.c_str());
    }

    us.Close();
    return 0;
}
