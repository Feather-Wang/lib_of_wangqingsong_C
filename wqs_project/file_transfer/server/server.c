/*该程序主要用于从服务器指定目录下下载文件和上传文件*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  N  256

typedef struct sockaddr SA;

void do_list(int connfd)
{
	DIR *mydir;
	struct dirent *dirp;

	mydir = opendir(".");
	while ((dirp = readdir(mydir)) != NULL)
	{
		if (dirp->d_name[0] == '.') continue;
		send(connfd, dirp->d_name, N, 0);
	}
	closedir(mydir);
}

void do_get(int connfd, char fname[])
{
	int fd, nbyte;
	char buf[N];

	if ((fd = open(fname, O_RDONLY)) < 0)
	{
		send(connfd, "N", 1, 0);
		return;
	}
	send(connfd, "Y", 1, 0);
	while ((nbyte = read(fd, buf, N)) > 0)
	{
		send(connfd, buf, nbyte, 0);
	}
	close(fd);
}

void do_put()
{
}

int main(int argc, char *argv[])
{
	int listenfd, connfd;
	char command[N];
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

	while ( 1 )
	{
		if ((connfd = accept(listenfd, NULL, NULL)) < 0)
		{
			perror("fail to accept");
			exit(-1);
		}
		recv(connfd, command, N, 0);  // receive command from client
		switch ( command[0] )
		{
		case 'L' :
			printf("do_list\n");
			do_list(connfd);
			break;
		case 'G' :
			printf("do_get\n");
			do_get(connfd, command+1);
			break;
		case 'P' :
			printf("do_put\n");
			do_put();
			break;
		}
		close(connfd);
	}

	return 0;
}

