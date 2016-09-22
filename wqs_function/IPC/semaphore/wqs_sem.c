#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/sem.h>
//初始化信号灯集
//信号灯0其值初始化为0
//信号灯1其值初始化为1
int semaphore_init(int *semid, char *path, int sem_num)
{
	key_t key;
	int value = 0,num = 0;
	
	//获得键值
	if((key = ftok(path,'k')) < 0)
	{
		perror("Fail to ftok");
		exit(EXIT_FAILURE);
	}
	
	//创建信号灯集，sem_num个信号灯
	if((*semid = semget(key, sem_num, IPC_CREAT | 0666)) < 0)
	{
		perror("Fail to semget");
        return -1;
	}
	
	//初始化信号灯的值
	for(value = 0; value < sem_num; value ++, num ++)
	{
		if(semctl(*semid, num, SETVAL, value) < 0)	
		{
			perror("Fail to semctl");
		    return -1;
        }
	}
	
	return 0;
}

//申请资源
int P(int semid,int semnum)
{
	struct sembuf sem;

	sem.sem_num = semnum;
	sem.sem_op  = -1;
	sem.sem_flg = 0;

	if(semop(semid,&sem,1) < 0)
	{
		perror("Fail to semop");
        return -1;
	}
	
	return 0;
}

//申请资源
int V(int semid,int semnum)
{
	struct sembuf sem;

	sem.sem_num = semnum;
	sem.sem_op  = 1;
	sem.sem_flg = 0;

	if(semop(semid,&sem,1) < 0)
	{
		perror("Fail to semop");
        return -1;
	}
	
	return 0;
}

