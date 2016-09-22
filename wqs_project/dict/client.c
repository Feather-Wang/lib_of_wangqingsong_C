#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  N  16
#define  R  1   //  user register
#define  L  2   //  user login
#define  Q  3   //  query word
#define  H  4   //  history record
#define  COL 50

#define DATABASE "my.db"

typedef struct sockaddr SA;

typedef struct 
{
	int type;
	char name[N];
	char data[256];   // password or word or remark
} MSG;

void do_register(int sockfd, MSG *pbuf)
{
	pbuf->type = R;
	printf("input name : ");
	scanf("%s", pbuf->name);
	printf("input password : ");
	scanf("%s", pbuf->data);
	send(sockfd, pbuf, sizeof(MSG), 0);
	recv(sockfd, pbuf, sizeof(MSG), 0);
	printf("register : %s\n", pbuf->data);

	return;
}

int do_login(int sockfd, MSG *pbuf)
{
	pbuf->type = L;
	printf("input name : ");
	scanf("%s", pbuf->name);
	printf("input password : ");
	scanf("%s", pbuf->data);
	send(sockfd, pbuf, sizeof(MSG), 0);
	recv(sockfd, pbuf, sizeof(MSG), 0);
	if (strncmp(pbuf->data, "OK", 3) == 0) 
	{
		printf("login : OK\n");
		return 1;
	}
	printf("login : %s\n", pbuf->data);

	return 0;
}

void print(char *p)
{
	int count = 0, len;
	char *q;
	
	printf("   ");
	while ( *p )
	{
		if (*p == ' ' && count == 0) p++;
		printf("%c", *p++);
		count++;

		/* if a new word will begin */
		if (*(p-1) == ' ' && *p != ' ')
		{
			q = p;
			len = 0;
			/* count the length of next word */
			while (*q != ' ' && *q != '\0') 
			{
				len++;
				q++;
			}
            if ((COL - count) < len) 
			{
				printf("\n   ");
				count = 0;
			}
		}
		if (count == 50)
		{
			count = 0;
			printf("\n   ");
		}
	}

	return;
}

void do_query(int sockfd, MSG *pbuf)
{
	pbuf->type = Q;
	while ( 1 )
	{
		printf("input word : ");
		scanf("%s", pbuf->data);
		if (strcmp(pbuf->data, "#") == 0) break;
		send(sockfd, pbuf, sizeof(MSG), 0);
		recv(sockfd, pbuf, sizeof(MSG), 0);
		print(pbuf->data);
		printf("\n");
	}

	return;
}

void do_history(int sockfd, MSG *pbuf)
{
	pbuf->type = H;
	send(sockfd, pbuf, sizeof(MSG), 0);
	while ( 1 )
	{
		recv(sockfd, pbuf, sizeof(MSG), 0);
		if (pbuf->data[0] == '\0') break;
		printf("%s\n", pbuf->data);
	}

	return;
}

int main(int argc, char *argv[])
{
	int sockfd, login = 0;
	struct sockaddr_in servaddr;
	MSG buf;
	char clean[64];

	if (argc < 3)
	{
		printf("Usage : %s <serv_ip> <serv_port>\n", argv[0]);
		exit(-1);
	}

	// XXX int socket(int domain, int type, int protocol);
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	// XXX int connect(int sockfd, const struct sockaddr *addr,
	//                 socklen_t addrlen);
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("fail to connect");
		exit(-1);
	}

	int n;
	while ( 1 )
	{
		printf("************************************\n");
		printf("* 1: register   2: login   3: quit *\n");
		printf("************************************\n");
		printf("please choose : ");
		if (scanf("%d", &n) == 0)
		{
			fgets(clean, 64, stdin);
			printf("\n");
			continue;
		}
		switch ( n )
		{
		case 1 :
			printf("\n");
			do_register(sockfd, &buf);
			printf("\n");
			break;
		case 2 :
			printf("\n");
			if (do_login(sockfd, &buf) == 1)
			{
				printf("\n");
				goto next;
			}
			printf("\n");
			break;
		case 3 :
			close(sockfd);
			exit(0);
		}
	}

next:

	while ( 1 )
	{
		printf("***********************************************\n");
		printf("* 1: query_word   2: history_record   3: quit *\n");
		printf("***********************************************\n");
		printf("please choose : ");
		
		if (scanf("%d", &n) == 0)
		{
			fgets(clean, 64, stdin);
			printf("\n");
			continue;
		}
		switch ( n )
		{
		case 1 :
			printf("\n");
			do_query(sockfd, &buf);
			printf("\n");
			break;
		case 2 :
			printf("\n");
			do_history(sockfd, &buf);
			printf("\n");
			break;
		case 3 :
			close(sockfd);
			exit(0);
		}
	}

	return 0;
}
