#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#define BUF_SIZE 1024*512
#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

int read_data(void *opaque, uint8_t *buf, int buf_size);
int copyBuf(void *opaque, uint8_t *buf, int buf_size);
int jumpVideo(char *bufVideo);

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
    //char wp[BUF_SIZE];
    //char *recv_buf = wp;
    char recv_buf[BUF_SIZE];
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
                    ret = recv(client_fd, recv_buf, BUF_SIZE, 0);
                    if( ret <= 0 )
                    {
                        ret < 0 ? perror("recv:") : printf("disconnect\n");
                        connect_flag = 1;
                        break;
                    }
                    else
                    {
                        printf("recv->%d:%s\n", ret, recv_buf);
                        //jumpVideo(recv_buf);
                        memset(recv_buf, 0, sizeof(recv_buf));
                        sprintf(recv_buf, "Welcome to China");
                        ret = send(client_fd, recv_buf, BUF_SIZE, 0);
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

//注册av_read_frame的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
int copyBuf(void *opaque, uint8_t *buf, int buf_size) 
{
    char *p = (char *)opaque;

    memcpy(buf, p, buf_size);
    return buf_size;
}

int jumpVideo(char *bufVideo) 
{
	uint8_t *buf = (uint8_t*)malloc(sizeof(uint8_t)*BUF_SIZE);

	av_register_all();
	AVIOContext * pb = NULL;
	AVInputFormat *piFmt = NULL;
	AVFormatContext *pFmt = NULL;
    puts("111111111111111111");

	pb = avio_alloc_context(buf, BUF_SIZE, 0, bufVideo, copyBuf, NULL, NULL);
	if (!pb) {
		fprintf(stderr, "avio alloc failed!\n");
		return -1;
	}
    puts("2222222222222222222");
	//step2:探测流格式
	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 2048+1) < 0) {
	//if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0) {
		fprintf(stderr, "probe failed!\n");
		return -1;
	} else {
        puts("22222223333333");
		fprintf(stdout, "probe success!\n");
		fprintf(stdout, "format: %s[%s] --> ", piFmt->name, piFmt->long_name);
	}
    puts("252525252525252525");
	pFmt = avformat_alloc_context();
	pFmt->pb = pb; //step3:这一步很关键
    puts("33333333333333333333");
	//step4:打开流
	if (avformat_open_input(&pFmt, "", piFmt, NULL) < 0) {
		fprintf(stderr, "avformat open failed.\n");
		return -1;
	} else {
		fprintf(stdout, "open stream success!\n");
	}
    printf("444444444444444444\n");
	//以下就和文件处理一致了
#if 1
	if (avformat_find_stream_info(pFmt, NULL) < 0) {
		fprintf(stderr, "could not fine stream.\n");
		return -1;
	}
#endif
    puts("******************************************11111111111");
    printf("wangqingsong ------------->%\n");
    int wi = 0;
    char bufz[256] = {0};
    for(wi = 0; wi < pFmt->nb_streams; wi++ )
    {
        AVStream *st = pFmt->streams[wi];
        avcodec_string(bufz, sizeof(bufz), st->codec, 0);
        printf("-->%s\n", bufz);
    }
	av_dump_format(pFmt, 0, "", 0);
    puts("******************************************22222222222");

	free(buf);
	return 0;
}

