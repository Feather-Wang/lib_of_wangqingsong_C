#ifndef __WQS_MSGQUEUE_H__

#define __WQS_MSGQUEUE_H__

typedef struct
{
	long type;
	char mtext[1024];
	int pid;
}MSG;

#define MSGLEN (sizeof(MSG) - sizeof(long))

#define MSGTYPE 100


extern int init_msg_queue(char *path);
extern int send_msg(int msgid,void *buf,int size);
extern int recv_msg(int msgid,void *buf,int size,int type);

#endif /* end of include guard: __WQS_MSGQUEUE_H__ */
