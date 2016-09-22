#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>

#include "wqs_msgqueue.h"

/*
 * return:
 * -1:error
 *  0:msgid
 *  */
int init_msg_queue(char *path)
{
	key_t key;
	int msgid;

	if((key = ftok(path,'k')) < 0)
	{
		perror("Fail to ftok");
        return -1;
	}
	
	//如果不存在，则创建，如果存在，直接返回msgid
	if((msgid = msgget(key,IPC_CREAT | 0666)) < 0)
	{
		perror("Fail to msgget");
        return -1;
	}

	return msgid;
}

//发送消息
int send_msg(int msgid,void *buf,int size)
{
	if(msgsnd(msgid,buf,size,0) < 0)
	{
		perror("Fail to msgsend");
        return -1;
	}

	return 0;
}

//接收消息
int recv_msg(int msgid,void *buf,int size,int type)
{
	if(msgrcv(msgid,buf,size,type,0) < 0)
	{
		perror("Fail to msgrecv");
        return -1;
	}

	return 0;
}


