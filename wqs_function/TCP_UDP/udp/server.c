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

static int insert_epoll_event(int epfd, int sockfd, int *curfds)
{
    struct epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if( epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) < 0 )
        return -1;
    *curfds += 1;

    return 1;
}

int main(int argc, char *argv[])
{
    struct epoll_event events[MAXEPOLLSIZE];
    int epfd = 0,
        efd = 0,
        nfds = 0,
        i = 0,
        curfds = 0;
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
    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(server_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("fail to bind");
        exit(-1);
    }

    epfd = epoll_create(MAXEPOLLSIZE);

    if( insert_epoll_event(epfd, server_fd, &curfds) < 0 )
        return -1;

    clientaddr_len = sizeof(client_addr);
    while ( 1 )
    {
        if( curfds <= 0 && curfds > MAXEPOLLSIZE )
            break;

        if( (nfds = epoll_wait(epfd, events, curfds, WAITTIME)) <= 0 )
            continue;

        for( i = 0; i < nfds; ++i )
        {
            efd = events[i].data.fd;

            if(efd == server_fd)
            {
                memset(&client_addr, 0, sizeof(client_addr));
                memset(recv_buf, 0, sizeof(recv_buf));
                recvfrom(server_fd, recv_buf, N, 0, (SA *)&client_addr, &clientaddr_len);
                printf("recv from [%s:%d] : %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_buf);

                memset(recv_buf, 0, sizeof(recv_buf));
                strcpy(recv_buf, "Welcome to Server");
                sendto(server_fd, recv_buf, N, 0, (SA *)&client_addr, sizeof(client_addr));
            }
        }
    }

    return 0;
}
