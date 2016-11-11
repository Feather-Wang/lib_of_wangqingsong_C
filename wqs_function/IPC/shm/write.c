#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wqs_shm.h"

#define SIZE 1024

//如果不存在，创建，存在直接打开
//
//./a.out ./
int main(int argc,char *argv[])
{
    int shmid = 0;
    char *shm_addr = NULL;
	
    if( -1 == shm_init( &shmid, &shm_addr, argv[1], SIZE, 0666) )
    {
        perror("Failed to shm_init");
        return -1;
    }

	while(1)
	{
		printf(">");
		fgets(shm_addr,1024,stdin);
		shm_addr[strlen(shm_addr) - 1] = '\0';

		if( 0 == strncmp(shm_addr,"quit",4) )
		{
            shm_del(&shmid, shm_addr);
			break;
		}
	}
	
	exit(EXIT_SUCCESS);
}
