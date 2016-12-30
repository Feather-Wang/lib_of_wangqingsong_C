/*
 *1、创建线程：pthread_create（）
 2、回收线程的三种方式
 pthread_join（）：该函数是阻塞型函数，会一直等待线程的结束，参数1：线程描述符，参数2：线程结束时调用pthread_exit()返回的地址，该地址必须不随线程结束而释放，否则不用。
 pthread_detach（）：该函数时非阻塞函数，当线程结束时，进程会进行线程资源的回收
 pthread_self()：获取当前线程的描述符，通常与pthread_detach配合使用，用于回收线程，在线程开始时，调用pthread_detach（pthread_self()）
 * */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <libwqs/wqs_log.h>

int a = 10;

void *pthread_func(void *arg)
{
    /*给线程起一个别名，方便在调试时确定是哪个线程*/
    /*存放线程名的数组最大为16字符，且最后一个字符为'\0'*/
    char pname[16] = {0};
    memset(pname, 0x00, 16);
    snprintf(pname, "pthread_func");
    prctl(PR_SET_NAME, pname);

    int *argument = (int*)arg;
    WQS_LOG(LOG_INFO, "argument = %d\n", *argument);
    int index = 0;

	while(1)
	{
		printf("index = %d.\n", index++);

        if( 3 == index )
            break;
        sleep(1);
    }

    char buf[128] = {0};
    sprintf(buf, "wang");
	pthread_exit((void*)buf);
}

int main()
{
	int res;
	pthread_t tid;
    void *pvalue = NULL;
    int a = 100;

    pvalue = &a;

    wqs_syslog_init("INFO");

	//线程创建            TID    属性   线程函数     给线程函数传递的参数
	res = pthread_create(&tid, NULL, pthread_func, (void *)&a);
	if(res != 0){
		perror("Fail to pthread create");
		exit(EXIT_FAILURE);
	}

    //pthread_cancel(tid[0]); // send end cmd to pthred tid[0].

	//阻塞到到对应的子线程结束,其中第二个参数，用来接收线程结束时，调用的pthread_exit()的参数, 正常接收的前提是，返回的地址空间不会随着线程结束而释放，否则接收到的也是无用的地址
	pthread_join(tid, (void *)&pvalue);//回收线程
    WQS_LOG(LOG_INFO, "the retured value of the pthread is %s\n", (char*)pvalue);

    wqs_syslog_destroy();

	exit(EXIT_FAILURE);
}
