#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

typedef struct sockaddr SA;

#define  N  2580
#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

int main(int argc, char *argv[])
{
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

    int i = 0;
    char j = '1';
    int maxlen = argc == 1 ? 2580 : atoi(argv[1]);
    for(i = 0; i < maxlen; i++)
    {
        recv_buf[i] = i/100 + j;
    }

    puts(recv_buf);

    while( 1 )
    {
                //memset(recv_buf, 0, sizeof(recv_buf));
                printf("send : \n");
                sendto(server_fd, recv_buf, maxlen, 0, (SA *)&server_addr, sizeof(server_addr));
                char a;
                scanf("%c", &a);
#if 0
                memset(recv_buf, 0, sizeof(recv_buf));
                recvfrom(server_fd, recv_buf, N, 0, NULL, NULL);
                printf("recv from server : %s\n", recv_buf);
#endif 
    }
    close(server_fd);

    return 0;
}
