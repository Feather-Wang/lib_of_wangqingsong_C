#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define  N  1024*512
#define SERVER_IP ("192.168.91.44")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 500000

typedef struct sockaddr SA;

static int epoll_control(int epfd, int sockfd, int type, int *count_fd)
{
    struct epoll_event ev;

    ev.events = EPOLLIN | EPOLLET;//读入,边缘触发方式
    //ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if( epoll_ctl(epfd, type, sockfd, &ev) < 0 )
        return -1;

    *count_fd += 1;
    return 0;
}

static int epoll_change(int epfd, int sockfd, int type)
{
    struct epoll_event ev;

    if( 0 == type )
    {
        ev.events = EPOLLIN | EPOLLET;//读入,边缘触发方式
    }
    else if( 1 == type )
    {
        ev.events = EPOLLOUT | EPOLLET;//写入,边缘触发方式
    }
    ev.data.fd = sockfd;
    if( epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev) < 0 )
        return -1;

    return 0;
}


//函数
//功能:设置socket为非阻塞的
    static int
make_socket_non_blocking (int sfd)
{
    int flags, s;

    //得到文件状态标志
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    //设置文件状态标志
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

static int socket_init(int *server_fd)
{
    int ret = -1;
    struct sockaddr_in server_addr;

    do{
        //int socket(int domain, int type, int protocol);
        if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("fail to socket");
            ret = -1;
            break;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
        if (bind(*server_fd, (SA *)&server_addr, sizeof(struct sockaddr_in)) < 0)
        {
            perror("fail to bind");
            ret = -1;
            break;
        }
        ret = make_socket_non_blocking (*server_fd);
        if (ret == -1)
        {
            perror("fail to set socket");
            break;
        }
        if (listen(*server_fd, 5) < 0)
        {
            perror("fail to listen");
            ret = -1;
            break;
        }

        ret = 0;

    }while(0);

    if( -1 == ret && 0 < *server_fd)
    {
        close(*server_fd);
    }

    return ret;
}

//函数:
//功能:创建和绑定一个TCP socket
//参数:端口
//返回值:创建的socket
    static int
create_and_bind (char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            /* We managed to bind successfully! */
            break;
        }

        close (sfd);
    }

    if (rp == NULL)
    {
        fprintf (stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo (result);

    return sfd;
}

int main(int argc, char *argv[])
{
    int server_fd = -1;
    char recv_buf[N];
    int ret = 0;

    /*****************   socket init   ******************/
#if 1
    ret = socket_init(&server_fd);
    if( -1 == ret )
    {
        perror("fail to init socket");
        return -1;
    }

#else 
    server_fd = create_and_bind (argv[1]);
    if (server_fd == -1)
        abort ();

    ret = make_socket_non_blocking (server_fd);
    if (ret == -1)
        abort ();

    ret = listen (server_fd, 5);
    if (ret == -1)
    {
        perror ("listen");
        abort ();
    }

#endif
    /********************    socket init End     ************************/

    /***********************    epoll init     ****************************/
    struct epoll_event events[MAXEPOLLSIZE];
    int epfd = 0,
        count_fd = 0;
    if( (epfd = epoll_create(MAXEPOLLSIZE)) <= 0 )
    {
        perror("failed to epoll_create");
        return -1;
    }
    if( epoll_control(epfd, server_fd, EPOLL_CTL_ADD, &count_fd) < 0 )
    {
        perror("failed to epoll_ctl");
        return -1;
    }

    printf("count_fd = %d\n", count_fd);

    while ( 1 )
    {
        // 判断epoll监听的描述符没有超过最大范围
        if( count_fd <= 0 && count_fd > MAXEPOLLSIZE )
            break;

        puts("epoll_wait start");
        int index = 0;
        int nfds = 0;
        if( (nfds = epoll_wait(epfd, events, count_fd, WAITTIME)) <= 0 )
        {
            puts("11");
            perror("epoll wait timeout");
            continue;
        }

        puts("epoll wait end");
        for( index = 0; index < nfds; ++index )
        {
            if ( (events[index].events & EPOLLERR) ||
                    (events[index].events & EPOLLHUP) )
            {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                perror("epoll error");
                close (events[index].data.fd);
                continue;
            }
            if( events[index].events & EPOLLIN )
            {
                if( events[index].data.fd == server_fd )
                {
                    puts("server_fd ----------------------");
                    while(1)
                    {
                        int client_fd = -1;
                        struct sockaddr_in client_addr;
                        socklen_t clientaddr_len = 0;

                        clientaddr_len = sizeof(client_addr);
                        memset(&client_addr, 0, sizeof(client_addr));

                        if ((client_fd = accept(server_fd, (SA *)&client_addr, &clientaddr_len)) < 0)
                        {
                            if ((errno == EAGAIN) ||
                                    (errno == EWOULDBLOCK))
                            {
                                /* We have processed all incoming
                                   connections. */
                                break;
                            }
                            else
                            {
                                perror("fail to accept");
                                break;
                            }
                        }
#if 1 
                        printf("%d...connection from [%s:%d]\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                        if( epoll_control(epfd, client_fd, EPOLL_CTL_ADD, &count_fd) < 0 )
                        {
                            perror("failed to epoll_ctl");
                            return -1;
                        }
#else
                        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                        //将地址转化为主机名或者服务名
                        ret = getnameinfo ((SA*)&client_addr, clientaddr_len,
                                hbuf, sizeof(hbuf),
                                sbuf, sizeof(sbuf),
                                NI_NUMERICHOST | NI_NUMERICSERV);//flag参数:以数字名返回
                        //主机地址和服务地址

                        if (ret == 0)
                        {
                            printf("Accepted connection on descriptor %d "
                                    "(host=%s, port=%s)\n", client_fd, hbuf, sbuf);
                        }

                        /* Make the incoming socket non-blocking and add it to the
                           list of fds to monitor. */
                        ret = make_socket_non_blocking (client_fd);
                        if (ret == -1)
                            abort ();

                        if( epoll_control(epfd, client_fd, EPOLL_CTL_ADD, &count_fd) < 0 )
                        {
                            perror("failed to epoll_ctl");
                            return -1;
                        }

#endif
                    }
                    continue;
                }
                else
                {
                    /* We have data on the fd waiting to be read. Read and
                       display it. We must read whatever data is available
                       completely, as we are running in edge-triggered mode
                       and won't get a notification again for the same
                       data. */
                    int done = 0;
                    puts("client_fd ----------------------");

                    while (1)
                    {
                        ssize_t count;

                        memset( recv_buf, 0, sizeof(recv_buf) );
                        count = recv(events[index].data.fd, recv_buf, N, 0);
                        if ( -1 == count )
                        {
                            /* If errno == EAGAIN, that means we have read all
                               data. So go back to the main loop. */
                            if ( EAGAIN == errno )
                            {
                                perror ("recv");
                                done = 1;
                            }
                            break;
                        }
                        else if ( 0 == count )
                        {
                            /* End of file. The remote has closed the
                               connection. */
                            done = 1;
                            break;
                        }
                        printf("recv buf --> %s\n", recv_buf);

                        //while(1)
                        //    sleep(1);
                        /* Write the buffer to standard output */

                        memset(recv_buf, 0, sizeof(recv_buf));
                        sprintf(recv_buf, "Welcome to China");
                        count = send(events[index].data.fd, recv_buf, strlen(recv_buf), 0);
                        if ( -1 == count )
                        {
                            perror ("send");
                        }
                    }

                    if (done)
                    {
                        printf ("Closed connection on descriptor %d\n",
                                events[index].data.fd);

                        /* Closing the descriptor will make epoll remove it
                           from the set of descriptors which are monitored. */
                        close (events[index].data.fd);
                    }

                }
            }
            if( events[index].events & EPOLLOUT )
            {
                printf("EPOLLOUT..........fd = %d\n", events[index].data.fd);
            }
        }
    }
    close(epfd);
    close(server_fd);

    return 0;
}

