#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UdpSvr
{
    public:
        UdpSvr()
        {
            _sock = -1;
        }

        ~UdpSvr()
        {}

        // 1.socket
        bool create_socket()
        {
            _sock = socket(AF_INET, SOCK_DGRAM, 0);
            if(_sock < 0)
            {
                perror("socket");
                return false;
            }
            return true;
        }

        // 2.bind
        bool Bind(std::string& ip, uint16_t port)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(port);
            
            int ret = bind(_sock, (struct sockaddr*)&addr, sizeof(addr));
            if(ret < 0)
            {
                perror("bind");
                return false;
            }
            return true;
        }

        // 3.sendto
        bool send_data(std::string& data, struct sockaddr_in* destaddr)
        {
            int sendsize = sendto(_sock, data.c_str(), data.size(), 0, (struct sockaddr*)destaddr, sizeof(struct sockaddr_in));
            if(sendsize < 0)
            {
                perror("sendto");
                return false;
            }
            return true;
        }

        // 4.recvfrom
        bool recv_data(std::string* buf, struct sockaddr_in* srcaddr)
        {
            char tmp[1024];
            memset(tmp, '\0', sizeof(tmp));
            socklen_t len = sizeof(struct sockaddr_in);

            int recvsize = recvfrom(_sock, tmp, sizeof(tmp) - 1, 0, (struct sockaddr*)srcaddr, &len);
            if(recvsize < 0)
            {
                perror("recvfrom");
                return false;
            }
            (*buf).assign(tmp, recvsize);
            return true;
        }

        // 5.close
        void Close()
        {
            close(_sock);
            _sock = -1;
        }
    
    private:
        int _sock;
};
