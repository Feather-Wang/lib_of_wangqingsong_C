#include "wqs_udp.h"

void *wqs_udp(void *arg)
{
    int server_fd = -1;
    unsigned char recv_buf[TS_SIZE];
    struct sockaddr_in server_addr,
                       client_addr;
    socklen_t clientaddr_len = 0;
    int res = 0;
    TSBUF *tsbuf = (TSBUF *)arg;

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

    clientaddr_len = sizeof(client_addr);
    while ( 1 )
    {
        P(&w_rs);

        memset(&client_addr, 0, sizeof(client_addr));
        memset(recv_buf, 0, sizeof(recv_buf));
        res = recvfrom(server_fd, recv_buf, TS_SIZE, 0, (SA *)&client_addr, &clientaddr_len);
        //printf("recv from [%s:%d] : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), res);

        memcpy(tsbuf->tsbuf, recv_buf, res);
        tsbuf->tslen = res;

        V(&r_rs);
    }

    return 0;
}

void *wqs_readfile(void *arg)
{
    TSBUF *tsbuf = (TSBUF *)arg;
    FILE *fp = NULL;
    int res = 0;

    fp = fopen("1-1shell.flv", "rb");
    if( NULL == fp )
    {
        fprintf(stderr, "wqs_readfile ---> fopen video file failed...");
        return ;
    }

    while(1)
    {
        P(&w_rs);

        res = fread(tsbuf->tsbuf, 1, TS_SIZE, fp); 
        tsbuf->tslen = res;

        V(&r_rs);
        sleep(1);
    }
}
