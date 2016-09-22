#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 64

typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
	int sockfd;
	char buf[N] = "Hello Server";
	struct sockaddr_in servaddr;
	
	if (argc < 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		return -1;
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}
		
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("fail to connect");
		exit(-1);
	}
	while ( 1 )
	{
		printf("client > ");
		fgets(buf, N, stdin);
		if (strncmp(buf, "quit\n", 5) == 0) break;
		send(sockfd, buf, strlen(buf)+1, 0);
		bzero(buf, N);
		recv(sockfd, buf, N, 0);
		printf("recv from server : %s\n", buf);
	}
	close(sockfd);

	return 0;
}
