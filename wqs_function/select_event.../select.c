/*
 *单纯使用select机制，虽然可以实现多路复用，但也有其缺点
 1、当一个用户的请求需要处理的时间过长时，不能及时响应其他用户的请求
 2、不适用于TCP服务器端
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

typedef struct sockaddr SA;

#define  N  64
#define SERVER_IP ("192.168.91.44")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

int main(int argc, char *argv[])
{
    int ret = 0;
    int server_fd = -1;
    char recv_buf[N];
    struct sockaddr_in server_addr,
                       client_addr;
    socklen_t clientaddr_len = 0;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (bind(server_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("fail to bind");
        exit(-1);
    }

    fd_set rfds; //用于存放读的socket描述符
    struct timeval tv;  //设置select超时时间
    int maxfd=server_fd;    //设置需要监听的最大的socket描述符中的最大值
    clientaddr_len = sizeof(client_addr);
    while ( 1 )
    {

        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(&rfds); // 清空文件描述符集合
        FD_SET( server_fd, &rfds);  // 将需要监听的socket描述符加入到集合中
        //ret = select(maxfd + 1, &rfds, NULL, NULL, &tv);    // 扫描集合，看看有哪个有数据传来，如果没有数据且设置了超时时间，超时后就会返回-1
        ret = select(maxfd + 1, &rfds, NULL, NULL, NULL);    // 扫描集合，看看有哪个有数据传来，如果没有数据且设置了超时时间，超时后就会返回-1
        printf("select ret = %d\n", ret);
        switch(ret)
        {
            case -1:
                perror("select error");
                return -1;
            case 0:
                puts("select timeout");
                continue;
            default:
                break;
        }

        /* 判断出现响应的描述符是否为server_fd */
        if (FD_ISSET(server_fd, &rfds))
        {
            memset(&client_addr, 0, sizeof(client_addr));
            memset(recv_buf, 0, sizeof(recv_buf));
            recvfrom(server_fd, recv_buf, N, 0, (SA *)&client_addr, &clientaddr_len);
            printf("recv from [%s:%d] : %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_buf);

            while(1)
            {
                puts("sleeping...");
                sleep(1);
            }

            memset(recv_buf, 0, sizeof(recv_buf));
            strcpy(recv_buf, "Welcome to Server");
            sendto(server_fd, recv_buf, N, 0, (SA *)&client_addr, sizeof(client_addr));
        }
    }

    return 0;
}
