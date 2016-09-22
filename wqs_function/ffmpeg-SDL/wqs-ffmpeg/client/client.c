#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;

#include "../wqs_sdk.h"
#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

int main(int argc, char *argv[])
{
    int server_fd = -1;
    unsigned char recv_buf[TS_SIZE];
    struct sockaddr_in server_addr;
    int res = 0;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    FILE *fp = fopen(argv[1], "rb");
    if( NULL == fp )
    {
        fprintf(stderr, "fopen failed");
        return 1;
    }

    int i = 0;
    while( 1 )
    {
        memset(recv_buf, 0, sizeof(recv_buf));
        printf("send : %d\n", i++);
        res = fread(recv_buf, 1, TS_SIZE, fp);
        sendto(server_fd, recv_buf, res, 0, (SA *)&server_addr, sizeof(server_addr));
        usleep(500);
    }
    close(server_fd);

    return 0;
}
