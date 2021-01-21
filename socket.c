#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("sockfd = %d\n", sockfd);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.80.193");
    addr.sin_port = htons(22);
    
    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    while(1)
    {
        sleep(1);
    }
    return 0;
}
