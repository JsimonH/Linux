#pragma once
#include "main.hpp"
#include <sys/select.h>
#include <vector>

class SelectSvr
{
    public:
        SelectSvr()
        {
            max_fd = -1;
            FD_ZERO(&readfds);
        }
        
        ~SelectSvr()
        {}
        
        void addFd(int fd)
        {
            // 1.添加
            FD_SET(fd, &readfds);
            // 2.更新最大文件描述符
            if(fd > max_fd)
            {
                max_fd = fd;
            }
        }

        void deleteFd(int fd)
        {
            // 1.删除
            FD_CLR(fd, &readfds);
            // 2.从max_fd位置往前找, 找到第一个存在于readfds中的文件描述符即为最大描述符
            for(int i = max_fd; i >= 0; i--)
            {
                if(FD_ISSET(i, &readfds) == 0)
                {
                    continue;
                }
                max_fd = i;
                break;
            }
        }

        bool selectWait(std::vector<TcpSvr>* v)
        {
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 3000;

            fd_set tmp = readfds;
            int ret = select(max_fd + 1, &tmp, NULL, NULL, &tv);
            // 监控出错
            if(ret < 0)
            {
                perror("select");
                return false;
            }
            // 监控超时
            else if(ret == 0)
            {
                return false;
            }
            // 正常监控
            for(int i = 0; i <= max_fd; i++)
            {
                if(FD_ISSET(i, &tmp))
                {
                    TcpSvr ts;
                    ts.setFd(i);
                    v->push_back(ts);
                }
            }
            return true;
        }

    private:
        int max_fd; // 最大文件描述符
        fd_set readfds; // 可读事件集合
};
