#ifndef __WQS_SEM_H__
#define __WQS_SEM_H__

extern int semaphore_init(int *semid, char *path, int sem_num);
extern int P(int semid,int semnum);
extern int V(int semid,int semnum);

#endif
