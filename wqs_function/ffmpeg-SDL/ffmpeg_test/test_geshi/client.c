#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 1442
//#define N 1024*512
#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
	int client_fd = -1;
	char recv_buf[N];
	struct sockaddr_in server_addr;
	int ret = 0;

	if ((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}
		
    printf("client -> %d...\n", client_fd);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	if (connect(client_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("fail to connect");
		exit(-1);
	}

    int readsize = 0;
    FILE *fp = fopen(argv[1], "rb");
    if( NULL == fp )
    {
        puts("video file open failed");
        return 1;
    }
	while ( 1 )
	{
        memset(recv_buf, 0, sizeof(recv_buf));
        printf("send : \n");
		//fgets(recv_buf, N, stdin);
        readsize = fread(recv_buf, 1, N, fp);
        printf("read file size --> %d\n", readsize);
		ret = send(client_fd, recv_buf, readsize, 0);
        if( ret <= 0 )
        {
            ret < 0 ? perror("recv:") : printf("disconnect\n");
            break;
        }
        printf("send size -->%d\n", ret);

        memset(recv_buf, 0, sizeof(recv_buf));
		ret = recv(client_fd, recv_buf, N, 0);
        if( ret <= 0 )
        {
            perror("recv error");
            break;
        }
        printf("recv:%s\n", recv_buf);
	}
	close(client_fd);

	return 0;
}
