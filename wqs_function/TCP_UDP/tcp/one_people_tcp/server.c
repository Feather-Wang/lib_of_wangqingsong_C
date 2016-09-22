#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#define  N  1024*512
#define SERVER_IP ("192.168.91.44")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

typedef struct sockaddr SA;

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
    int server_fd = -1,
        client_fd = -1;
    struct sockaddr_in server_addr,
                       client_addr;
    socklen_t clientaddr_len = 0;
    char recv_buf[N];
    int ret = 0;
    struct epoll_event events[MAXEPOLLSIZE];
    int epfd = 0,
        efd = 0,
        nfds = 0,
        i = 0,
        curfds = 0;
    int connect_flag = 0;

    //int socket(int domain, int type, int protocol);
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
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

    if (listen(server_fd, 5) < 0)
    {
        perror("fail to listen");
        exit(-1);
    }

    if( (epfd = epoll_create(MAXEPOLLSIZE)) <= 0 )
        return -1;
    if( insert_epoll_event(epfd, server_fd, &curfds) < 0 )
        return -1;

    clientaddr_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    while ( 1 )
    {
        if( curfds <= 0 && curfds > MAXEPOLLSIZE )
            break;

        if ((client_fd = accept(server_fd, (SA *)&client_addr, &clientaddr_len)) < 0)
        {
            perror("fail to accept");
            exit(-1);
        }
        printf("%d...connection from [%s:%d]\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if( insert_epoll_event(epfd, client_fd, &curfds) < 0 )
            return -1;

        while( 1 )
        {
            if( (nfds = epoll_wait(epfd, events, curfds, WAITTIME)) <= 0 )
                continue;

            for( i = 0; i < nfds; ++i )
            {
                efd = events[i].data.fd;

                if( efd == client_fd )
                {
                    memset( recv_buf, 0, sizeof(recv_buf) );
                    ret = recv(client_fd, recv_buf, N, 0);
                    if( ret <= 0 )
                    {
                        ret < 0 ? perror("recv:") : printf("disconnect\n");
                        connect_flag = 1;
                        break;
                    }
                    else
                    {
                        printf("recv:%s\n", recv_buf);

                        memset(recv_buf, 0, sizeof(recv_buf));
                        sprintf(recv_buf, "Welcome to China");
                        ret = send(client_fd, recv_buf, strlen(recv_buf), 0);
                        if( ret <= 0 )
                        {
                            perror("send error");
                            connect_flag = 1;
                            break;
                        }
                    }
                }
                if( efd == server_fd )
                {
                    printf("server_fd...\n");
                }
            }
            if( connect_flag )
            {
                connect_flag = 0;
                break;
            }
        }
        close(client_fd);
    }

    return 0;
}

