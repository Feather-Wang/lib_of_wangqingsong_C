#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

typedef struct
{
	long type;
	char mtext[1024];
	int pid;
}MSG;

#define MSGLEN (sizeof(MSG) - sizeof(long))

#define MSGTYPE 100

int init_msg_queue(char *path)
{
	key_t key;
	int msgid;

	if((key = ftok(path,'k')) < 0)
	{
		perror("Fail to ftok");
		exit(EXIT_FAILURE);
	}
	
	//如果不存在，则创建，如果存在，直接返回msgid
	if((msgid = msgget(key,IPC_CREAT | 0666)) < 0)
	{
		perror("Fail to msgget");
		exit(EXIT_FAILURE);
	}

	return msgid;
}

//发送消息
int send_msg(int msgid,void *buf,int size)
{
	if(msgsnd(msgid,buf,size,0) < 0)
	{
		perror("Fail to msgsend");
		exit(EXIT_FAILURE);
	}

	return 0;
}

//接收消息
int recv_msg(int msgid,void *buf,int size,int type)
{
	if(msgrcv(msgid,buf,size,type,0) < 0)
	{
		perror("Fail to msgrecv");
		exit(EXIT_FAILURE);
	}

	return 0;
}

//./a.out ./
int main(int argc,char *argv[])
{
	int msgid;
	MSG buf;

	msgid = init_msg_queue(argv[1]);

	while(1)
	{
		recv_msg(msgid,&buf,MSGLEN,MSGTYPE);

		printf("\n***************************\n");
		printf("PID : %d\n",buf.pid);
		printf("MSG : %s\n",buf.mtext);
		printf("***************************\n");
	}

	exit(EXIT_SUCCESS);
}
