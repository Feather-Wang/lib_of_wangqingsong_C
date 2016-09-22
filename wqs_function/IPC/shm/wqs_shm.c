#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "wqs_shm.h"

/*
 * return:
 * -1:error
 *  0:ok
 *  */
int shm_init(int *shmid, char **shm_addr, char *shm_file, int shm_size, int flag)
{
    key_t key;
    int ret = -1;

    do{
        //产生键值
        if( 0 > (key = ftok(shm_file,'k')) )
        {
            perror("Fail to ftok");
            break;
        }
        

        //测试是否存在
        if( 0 > (*shmid = shmget(key, shm_size, IPC_CREAT |IPC_EXCL| flag))  && EEXIST == errno )
        {
            //写端已经创建,直接打开
            *shmid = shmget(key, shm_size, flag);
        }
        else
        {
            //创建
            if( 0 > (*shmid = shmget(key, shm_size, IPC_CREAT | flag)) )
            {
                perror("Fail to shmget");
                break;
            }
        }

        //映射到进程私有空间
        if( NULL == (*shm_addr = shmat(*shmid, NULL, 0)) )
        {
            perror("Fail to shmat");
            break;
        }
        
        ret = 0;

    }while(0);

    if( -1 == ret )
    {
        shm_del(shmid, *shm_addr);
    }

    return ret;
}

/*
 * return:
 * -1:error
 *  0:ok
 *  */
int shm_del(int *shmid, char *shm_addr)
{
    int ret = -1;
    do
    {
        /*取消共享内存到进程空间的映射*/
        if( NULL != shm_addr && 0 > shmdt(shm_addr) )
        {
            perror("Fail to shmdt");
            break;
        }

        /*删除共享内存片段*/
        if( 0 < *shmid && 0 > shmctl(*shmid, IPC_RMID, NULL) )
        {
            perror("Fail to shmctl");
            break;
        }

        ret = 0;

    }while{0};

    return ret;
}
