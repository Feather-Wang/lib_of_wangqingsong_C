#include <stdio.h>
#include <stdlib.h>

#include "wqs_msgqueue.h"

//./a.out ./
int main(int argc,char *argv[])
{
	int msgid;
	MSG buf;

    if( 1 == argc ) 
    {
        fprintf(stderr, "Run : ./r ./\n");
        return -1;
    }

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
