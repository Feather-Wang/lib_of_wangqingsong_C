#ifndef __WQS_SDK_H__

#define __WQS_SDK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define TS_SIZE 4096

typedef struct data
{
    unsigned char tsbuf[TS_SIZE];
    int tslen;
}TSBUF;

sem_t r_rs,w_rs;

#endif
