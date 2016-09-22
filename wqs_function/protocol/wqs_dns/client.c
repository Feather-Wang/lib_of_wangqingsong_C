#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "wqs_dns.h"

#define  N  512
#define SERVER_IP ("192.168.25.23")
#define SERVER_PORT 53

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

int main(int argc, char *argv[])
{
	if( argc != 2 )
	{
		printf("Run this demo like that : ./a.out www.baidu.com\n");
		return -1;
	}
	int ret = -1;

	uint8_t *request_url = argv[1];
    uint8_t ip_list[1024] = {0};

    int server_fd = -1;
    uint8_t recv_buf[N];
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
    
    int len = dns_build(recv_buf, request_url);

    sendto(server_fd, recv_buf, len, 0, (SA *)&server_addr, sizeof(server_addr));

    memset(recv_buf, 0, sizeof(recv_buf));
    len = recvfrom(server_fd, recv_buf, N, 0, NULL, NULL);
    close(server_fd);

    printf("[%s:%d] recv_buf len = %d\n", __FUNCTION__, __LINE__, len);

	memset(ip_list, 0x00, sizeof(ip_list));
    ret = dns_solve(recv_buf, len, ip_list);
	if( 0 != ret )
	{
		printf("[%s:%d] dns_solve error, code = %d, msg = %s\n", ret, wqs_dns_strerror(ret));
		return -1;
	}

    printf("[%s:%d] ip_list = %s\n", __FUNCTION__, __LINE__, ip_list);

    return 0;
}
