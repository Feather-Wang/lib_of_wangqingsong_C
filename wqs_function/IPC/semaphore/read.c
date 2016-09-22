#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/sem.h>

#include "wqs_sem.h"

#define READRS 0
#define WRITERS 1

#define NSEMS 2

#define SIZE 1024

//如果不存在，创建，存在直接打开
//
//./a.out ./
int main(int argc,char *argv[])
{
    int shmid = 0;
    char *shm_addr = NULL;
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
		P(semid,READRS);
        
        printf("Read : %s.\n", shm_addr);

        if( 0 == strncmp( shm_addr, "quit", 4) )
        {
            shm_del(&shmid, shm_addr);
            break;
        }
		
        V(semid,WRITERS);
    }

    exit(EXIT_SUCCESS);
}
