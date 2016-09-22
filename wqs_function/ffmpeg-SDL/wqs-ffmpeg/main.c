#include "wqs_sdk.h"
#include "wqs_udp.h"
#include "wqs_ffmpeg.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>

int main()
{
	int res;
	pthread_t pthread_udp;
	pthread_t pthread_ffmpeg;
    int *pvalue;
    int a = 100;
    int b = 200;

	init_semaphore(&r_rs,0);
	init_semaphore(&w_rs,1);

    TSBUF tsbuf;
    memset(&tsbuf, 0, sizeof(TSBUF));

	//线程创建            TID    属性   线程函数     给线程函数传递的参数
	res = pthread_create(&pthread_udp, NULL, wqs_udp, (void *)&tsbuf);
	//res = pthread_create(&pthread_udp, NULL, wqs_readfile, (void *)&tsbuf);
	if(res != 0){
		perror("Fail to pthread create");
		exit(EXIT_FAILURE);
	}
    printf("pth1 = %lu\n",pthread_udp);

    res = pthread_create(&pthread_ffmpeg, NULL, wqs_ffmpeg, (void *)&tsbuf);
	if(res != 0){
		perror("Fail to pthread create");
		exit(EXIT_FAILURE);
	}
    printf("pth1 = %lu\n",pthread_ffmpeg);

	pthread_detach(pthread_udp);//回收线程
	pthread_join(pthread_ffmpeg,NULL);//回收线程

    puts("end...");
	exit(0);
}
