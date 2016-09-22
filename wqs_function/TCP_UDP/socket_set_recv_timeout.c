#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

typedef struct sockaddr SA;

#define  N  64
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777

int main(int argc, char *argv[])
{
    int ret = -1;
    int server_fd = -1;
    char recv_buf[N];
    struct sockaddr_in server_addr;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    memset(recv_buf, 0, sizeof(recv_buf));
    sprintf(recv_buf, "Hello World");
    ret = sendto(server_fd, recv_buf, N, 0, (SA *)&server_addr, sizeof(server_addr));
    if( 0 >= ret )
    {
        printf("sendto failed : %s\n", strerror(errno));
        return -1;
    }

    fd_set rfds; //用于存放读的socket描述符
    struct timeval tv;  //设置select超时时间
    int maxfd=server_fd;    //设置需要监听的最大的socket描述符中的最大值

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&rfds); // 清空文件描述符集合
    FD_SET( server_fd, &rfds);  // 将需要监听的socket描述符加入到集合中

    ret = select(maxfd + 1, &rfds, NULL, NULL, &tv);    // 扫描集合，看看有哪个有数据传来，如果没有数据且设置了超时时间，超时后就会返回-1
    switch(ret)
    {
    case -1:
        perror("select error");
        break;
    case 0:
        puts("select timeout");
        break;
    default:
        memset(recv_buf, 0, sizeof(recv_buf));
        ret = recvfrom(server_fd, recv_buf, N, 0, NULL, NULL);
        if( 0 >= ret )
        {
            printf("recvfrom failed : %s\n", strerror(errno));
            if( errno == EAGAIN )
            {
                puts("11");
            }
            return -1;
        }
        printf("recv from server : %s\n", recv_buf);
        break;
    }

    close(server_fd);

    return 0;
}
