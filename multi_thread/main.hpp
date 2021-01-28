#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TcpSvr
{
    public:
        TcpSvr()
        {
            _sockfd = -1;
        }

        ~TcpSvr()
        {}

        // 创建套接字
        bool CreateSocket()
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(_sockfd < 0)
            {
                perror("socket");
                return false;
            }
            return true;
        }

        // 绑定地址信息
        bool Bind(std::string& ip, uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(port);

            int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
            if(ret < 0)
            {
                perror("bind");
                return false;
            }
            return true;
        }

        // 监听
        bool Listen(int backlog = 5)
        {
            int ret = listen(_sockfd, backlog);
            if(ret < 0)
            {
                perror("listen");
                return false;
            }
            return true;
        }

        // 获取连接
        bool Accept(struct sockaddr_in* cli_addr, TcpSvr* ts)
        {
            socklen_t addrlen = sizeof(struct sockaddr_in);
            int serverfd = accept(_sockfd, (struct sockaddr*)cli_addr, &addrlen);
            if(serverfd < 0)
            {
                perror("accept");
                return false;
            }
            ts->_sockfd = serverfd;
            return true;
        }

        // 发起连接
        bool Connect(std::string& ip, uint16_t port)
        {
            struct sockaddr_in destaddr;
            destaddr.sin_family = AF_INET;
            destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
            destaddr.sin_port = htons(port);
            
            int ret = connect(_sockfd, (struct sockaddr*)&destaddr, sizeof(destaddr));
            if(ret < 0)
            {
                perror("connet");
                return false;
            }
            return true;
        }

        // 发送数据
        bool Send(std::string& data)
        {
            int sendsize = send(_sockfd, data.c_str(), data.size(), 0);
            if(sendsize < 0)
            {
                perror("send");
                return false;
            }
            return true;
        }

        // 接收数据
        bool Recv(std::string* data)
        {
            char buf[1024] = {0};
            int recvsize = recv(_sockfd, buf, sizeof(buf) - 1, 0);
            if(recvsize < 0)
            {
                perror("recv");
                return false;
            }
            else if(recvsize == 0)
            {
                printf("disconnected...\n");
                return false;
            }
            (*data).assign(buf, recvsize);
            return true;
        }

        // 关闭套接字
        void Close()
        {
            close(_sockfd);
            _sockfd = -1;
        }
    
    private:
        int _sockfd;
};
