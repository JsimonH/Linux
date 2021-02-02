#include "select_svr.hpp"
#define CHECK(p) if(p != true){return -1;}

int main()
{
    TcpSvr ts;
    CHECK(ts.CreateSocket());
    CHECK(ts.Bind("0.0.0.0", 19999));
    CHECK(ts.Listen());

    SelectSvr ss;
    ss.addFd(ts.getFd());

    while(1)
    {
        // 1.监控
        std::vector<TcpSvr> v;
        if(!ss.selectWait(&v))
        {
            continue;
        }

        for(size_t i = 0; i < v.size(); i++)
        {
            // 2.接受新的连接
            if(ts.getFd() == v[i].getFd())
            {
                struct sockaddr_in peeraddr;
                TcpSvr newts;
                ts.Accept(&peeraddr, &newts);
                printf("A new connection: [ip] %s, [port] %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

                // 将新创建出来的套接字添加到select监听的事件集合中
                ss.addFd(newts.getFd());
            }
            // 3.当连接上有数据时
            else
            {
                std::string data;
                bool ret = v[i].Recv(&data);
                if(!ret)
                {
                    ss.deleteFd(v[i].getFd());
                    v[i].Close();
                }
                printf("client: %s\n", data.c_str());
            }
        }
    }
    return 0;
}
