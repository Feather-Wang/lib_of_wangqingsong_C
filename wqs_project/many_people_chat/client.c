#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;

#define  N  64
#define  R  1
#define  B  2
#define  U  3
#define  E  4

typedef struct
{
	int type;
	char name[16];
	char text[N];
} MSG;

int main(int argc, char *argv[])
{
	int sockfd;
	MSG buf;
	pid_t pid;
	struct sockaddr_in servaddr;
	
	if (argc < 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		return -1;
	}

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}
		
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	printf("input name : ");
	fgets(buf.name, 16, stdin);
	buf.name[strlen(buf.name)-1] = '\0';
	buf.type = R;
	sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&servaddr, sizeof(servaddr));

	if ((pid = fork()) < 0)
	{
		perror("fail to fork");
		exit(-1);
	}
	else if (pid == 0)  // receive message
	{
		while ( 1 )
		{
			recvfrom(sockfd, &buf, sizeof(buf), 0, NULL, NULL);
			if (buf.type == E) break;
			printf("\n**** [%s] %s", buf.name, buf.text);
		}
		printf("Server is down, exit...\n");
		kill(getppid(), SIGKILL);
		exit(0);
	}
	else  // send message
	{
		buf.type = B;
		usleep(100000);
		while ( 1 )
		{
			printf("client > ");
			fgets(buf.text, N, stdin);
			if (strncmp(buf.text, "quit\n", 5) == 0)
			{
				buf.type = U;
				sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&servaddr, sizeof(servaddr));
				usleep(200000);
				kill(pid, SIGKILL);
				exit(0);
			}
			sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&servaddr, sizeof(servaddr));
			usleep(200000);
		}
	}

	return 0;
}
