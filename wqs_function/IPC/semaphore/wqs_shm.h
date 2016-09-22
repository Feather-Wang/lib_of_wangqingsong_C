#ifndef __WQS_SHM_H__
#define __WQS_SHM_H__

int shm_init(int *shmid, char **shm_addr, char *shm_file, int shm_size, int flag);
int shm_del(int *shmid, char *shm_addr);

#endif
