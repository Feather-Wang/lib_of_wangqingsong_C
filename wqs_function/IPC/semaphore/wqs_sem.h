#ifndef __WQS_SEM_H__
#define __WQS_SEM_H__

int semaphore_init(int *semid, char *path, int sem_num);
int P(int semid,int semnum);
int V(int semid,int semnum);

#endif
