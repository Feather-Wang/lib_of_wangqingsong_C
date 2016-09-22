#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/sem.h>
#include "wqs_sem.h"
#include "../shm/wqs_shm.h"

#define READRS 0
#define WRITERS 1

#define NSEMS 2

#define SIZE 1024

//如果不存在，创建，存在直接打开
//
//./a.out ./
int main(int argc,char *argv[])
{
    char *shm_addr = NULL;
	int shmid = 0;
    int semid = 0;

    if( -1 == semaphore_init( &semid, argv[1], NSEMS) )
    {
        perror("Failed to semaphore_init");
        return -1;
    }

    if( -1 == shm_init( &shmid, &shm_addr, argv[1], SIZE, 0666) )
    {
        perror("Failed to shm_init");
        return -1;
    }
	
    while(1)
	{
		P(semid,WRITERS);

		printf(">");
		fgets(shm_addr,1024,stdin);
		shm_addr[strlen(shm_addr) - 1] = '\0';

		if( 0 == strncmp(shm_addr,"quit",4) )
		{
            shm_del(&shmid, shm_addr);
			break;
		}
		
        V(semid,READRS);
	}
	
	exit(EXIT_SUCCESS);
}
