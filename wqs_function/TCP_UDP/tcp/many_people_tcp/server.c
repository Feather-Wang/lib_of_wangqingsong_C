#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  N  64

typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
	int listenfd, connfd, n;
	char buf[N];
	pid_t pid;
	struct sockaddr_in myaddr;

	if (argc < 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		return -1;
	}

	//int socket(int domain, int type, int protocol);
	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = PF_INET;
	myaddr.sin_port = htons(atoi(argv[2]));
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(listenfd, (SA *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("fail to bind");
		exit(-1);
	}

	if (listen(listenfd, 5) < 0)
	{
		perror("fail to listen");
		exit(-1);
	}

	signal(SIGCHLD, SIG_IGN);
	while ( 1 )
	{
		if ((connfd = accept(listenfd, NULL, NULL)) < 0)
		{
			perror("fail to accept");
			exit(-1);
		}
		if ((pid = fork()) < 0)
		{
			perror("fail to fork");
			exit(-1);
		}
		else if (pid == 0)
		{
			while ((n = recv(connfd, buf, N, 0)) > 0)
			{
                printf("recv : %s\n", buf);
				send(connfd, buf, n, 0);
			}
			printf("client is closed\n");
			exit(0);
		}
		else
		{
			close(connfd);
		}
	}

	return 0;
}

