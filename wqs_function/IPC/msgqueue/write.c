#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "wqs_msgqueue.h"

//./a.out ./
int main(int argc,char *argv[])
{
	int msgid;
	MSG buf;

    if( 1 == argc ) 
    {
        fprintf(stderr, "Run : ./w ./\n");
        return -1;
    }

	msgid = init_msg_queue(argv[1]);

	while(1)
	{
		printf(">");
		
		buf.type = MSGTYPE;
		buf.pid  = getpid();
		
		fgets(buf.mtext,sizeof(buf.mtext),stdin);
		buf.mtext[strlen(buf.mtext) - 1] = '\0';
		
		send_msg(msgid,&buf,MSGLEN);
	}

	exit(EXIT_SUCCESS);
}
