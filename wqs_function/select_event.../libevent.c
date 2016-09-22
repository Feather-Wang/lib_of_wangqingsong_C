#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <event.h>

#define SERVER_IP ("192.168.91.44")
#define SERVER_PORT 7777

#define  MEM_SIZE  1024*512

typedef struct sockaddr SA;

struct event_base* base;
struct sock_ev {
    struct event* read_ev;
    struct event* write_ev;
    char* buffer;
};

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

void release_sock_event(struct sock_ev* ev)
{
    event_del(ev->read_ev);
    free(ev->read_ev);
    free(ev->write_ev);
    free(ev->buffer);
    free(ev);
}

void on_write(int sock, short event, void* arg)
{
    char* buffer = (char*)arg;
    send(sock, buffer, strlen(buffer), 0);

    free(buffer);
}

void on_read(int sock, short event, void* arg)
{
    struct event* write_ev;
    int size;
    struct sock_ev* ev = (struct sock_ev*)arg;

    ev->buffer = (char*)malloc(MEM_SIZE);
    bzero(ev->buffer, MEM_SIZE);
    
    size = recv(sock, ev->buffer, MEM_SIZE, 0);
    printf("receive data:%s, size:%d\n", ev->buffer, size);
    if (size == 0) {
        release_sock_event(ev);
        close(sock);
        return;
    }
    
    event_set(ev->write_ev, sock, EV_WRITE, on_write, ev->buffer);
    event_base_set(base, ev->write_ev);
    event_add(ev->write_ev, NULL);
}

void on_accept(int sock, short event, void* arg)
{
    struct sockaddr_in cli_addr;
    int client_fd, sin_size;
    struct sock_ev* ev = (struct sock_ev*)malloc(sizeof(struct sock_ev));
    
    ev->read_ev = (struct event*)malloc(sizeof(struct event));
    ev->write_ev = (struct event*)malloc(sizeof(struct event));
    sin_size = sizeof(struct sockaddr_in);
    
    client_fd = accept(sock, (struct sockaddr*)&cli_addr, &sin_size);
    
    event_set(ev->read_ev, client_fd, EV_READ|EV_PERSIST, on_read, ev);
    event_base_set(base, ev->read_ev);
    event_add(ev->read_ev, NULL);
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

int main(int argc, char* argv[])
{

    int ret = 0;
    int server_fd = -1;

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

    struct event listen_ev;
    base = event_base_new();
    event_set(&listen_ev, server_fd, EV_READ|EV_PERSIST, on_accept, NULL);
    event_base_set(base, &listen_ev);
    event_add(&listen_ev, NULL);
    event_base_dispatch(base);

    return 0;
}
