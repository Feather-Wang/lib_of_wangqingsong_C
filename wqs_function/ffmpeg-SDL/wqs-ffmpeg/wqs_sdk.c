#include "wqs_sdk.h"

int init_semaphore(sem_t *prs,int value)
{
	if(sem_init(prs,0,value) < 0)
	{
		perror("Fail to sem init");
		exit(EXIT_FAILURE);
	}

	return 0;
}

void P(sem_t *prs)
{
	if(sem_wait(prs) < 0)
	{
		perror("Sem wait failed");
		exit(EXIT_FAILURE);
	}
	
	return;
}

void V(sem_t *prs)
{
	if(sem_post(prs) < 0)
	{
		perror("Sem post failed");
		exit(EXIT_FAILURE);
	}

	return;
}

