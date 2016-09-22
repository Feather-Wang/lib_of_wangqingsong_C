/*æ÷”ÚÕ¯¡ƒÃÏ “≥Ã–Ú*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

typedef struct _node_
{ 
	struct sockaddr_in cliaddr;
	struct _node_ *next;
} linknode, *linklist;

void add_usr(int sockfd, MSG buf, struct sockaddr_in peeraddr, linklist h)
{
	linklist p;

	p = (linklist)malloc(sizeof(linknode));
	p->cliaddr = peeraddr;
	p->next = h->next;
	h->next = p;
	sprintf(buf.text, "%s is online\n", buf.name);
	strcpy(buf.name, "system");
	p = p->next;
	while (p != NULL)
	{
		sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&p->cliaddr, sizeof(peeraddr));
		p = p->next;
	}
	strcpy(buf.text, "Welcome to Farsight Chat Room\n");
	sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&peeraddr, sizeof(peeraddr));
}

void del_usr(int sockfd, MSG buf, struct sockaddr_in peeraddr, linklist h)
{
	linklist p = h->next;

	sprintf(buf.text, "%s is offline\n", buf.name);
	strcpy(buf.name, "system");
	while (p != NULL)
	{
		if (memcmp(&p->cliaddr, &peeraddr, sizeof(peeraddr)) == 0)
		{
			h->next = p->next;
			free(p);
		}
		else
		{
			sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&p->cliaddr, sizeof(peeraddr));
			h = h->next;
		}
		p = h->next;
	}
	strcpy(buf.text, "See You Later\n");
	sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&peeraddr, sizeof(peeraddr));
}

void broadcast(int sockfd, MSG buf, linklist h)
{
	h = h->next;
	while (h != NULL)
	{
		sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&h->cliaddr, sizeof(h->cliaddr));
		h = h->next;
	}
}

int main(int argc, char *argv[])
{
	int sockfd;
    MSG buf;
	pid_t pid;
	struct sockaddr_in myaddr, peeraddr;
	socklen_t peerlen;

	if (argc < 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		return -1;
	}

	//int socket(int domain, int type, int protocol);
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = PF_INET;
	myaddr.sin_port = htons(atoi(argv[2]));
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(sockfd, (SA *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("fail to bind");
		exit(-1);
	}

	if ((pid = fork()) < 0)
	{
		perror("fail to fork");
		exit(-1);
	}
	else if (pid == 0)
	{
		peerlen = sizeof(peeraddr);
		linklist h = (linklist)malloc(sizeof(linknode));
		h->next = NULL;
		while ( 1 )
		{
			recvfrom(sockfd, &buf, sizeof(buf), 0, (SA *)&peeraddr, &peerlen);
			switch ( buf.type )
			{
			case R :
				add_usr(sockfd, buf, peeraddr, h);
				break;
			case U :
				del_usr(sockfd, buf, peeraddr, h);
				break;
			case B :
			case E :
				broadcast(sockfd, buf, h);
				break;
			}
			if (buf.type == E) exit(0);
		}
	}
	else
	{
		strcpy(buf.name, "system");
		buf.type = B;
		while ( 1 )
		{
			printf("server > ");
			fgets(buf.text, N, stdin);
			if (strncmp(buf.text, "quit\n", 5) == 0)
			{
				buf.type = E;
				sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&myaddr, sizeof(myaddr));
				usleep(200000);
				exit(0);
			}
			sendto(sockfd, &buf, sizeof(buf), 0, (SA *)&myaddr, sizeof(myaddr));
		}
	}

	return 0;
}

