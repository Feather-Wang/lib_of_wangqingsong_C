#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;

#define  N  512
#define SERVER_IP ("192.168.25.233")
#define SERVER_PORT 137

char buf[] = {0x8c, 0x94, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x43, 0x4b, 0x41,\
    0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,\
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00, 0x21,\
        0x00, 0x01};

int main(int argc, char *argv[])
{
    int ret = -1;
    int server_fd = -1;
    uint8_t recv_buf[N];
    struct sockaddr_in server_addr;
    struct sockaddr_in local_addr;

    do{
        if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("fail to socket");
            break;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = PF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = PF_INET;
        local_addr.sin_port = htons(SERVER_PORT);
        local_addr.sin_addr.s_addr = inet_addr("192.168.25.177");
        bind(server_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));

        memset(recv_buf, 0, sizeof(recv_buf));

        sendto(server_fd, buf, sizeof(buf), 0, (SA *)&server_addr, sizeof(server_addr));

        memset(recv_buf, 0, sizeof(recv_buf));
        int len = recvfrom(server_fd, recv_buf, N, 0, NULL, NULL);

        printf("[%s:%d] recv_buf len = %d, %s\n", __FUNCTION__, __LINE__, len, recv_buf);

        ret = 0;

    }while(0);

    if( 0 < server_fd )
        close(server_fd);

    return ret;
}
