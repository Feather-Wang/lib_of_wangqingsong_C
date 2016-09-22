#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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

    return 0;
}

int main(int argc, char *argv[])
{
    int server_fd = -1;
    char recv_buf[N];
    struct sockaddr_in server_addr;
    struct epoll_event events[MAXEPOLLSIZE];
    int epfd = 0,
        efd = 0,
        nfds = 0,
        i = 0,
        curfds = 0;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    epfd = epoll_create(MAXEPOLLSIZE);

    if( insert_epoll_event(epfd, server_fd, &curfds) < 0 )
        return -1;

    while( 1 )
    {
        if( curfds <= 0 && curfds >> MAXEPOLLSIZE )
            break;

        //if( (nfds = epoll_wait(epfd, events, curfds, WAITTIME)) <= 0 )
        //    continue;

        //for( i = 0; i < nfds; ++i )
        {
            efd = events[i].data.fd;

            //if( efd == server_fd )
            {
                memset(recv_buf, 0, sizeof(recv_buf));
                printf("send : ");
                fgets(recv_buf, N, stdin);
                sendto(server_fd, recv_buf, N, 0, (SA *)&server_addr, sizeof(server_addr));

                memset(recv_buf, 0, sizeof(recv_buf));
                recvfrom(server_fd, recv_buf, N, 0, NULL, NULL);
                printf("recv from server : %s\n", recv_buf);
            }
        }
    }
    close(server_fd);

    return 0;
}
