#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

char buf[1024];
pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

void *input_buffer(void *arg)
{
    int index = 0;
	while(1)
	{
		//申请锁
        pthread_mutex_lock(&g_mutex);

        sprintf(buf, "%d number", index);
		printf("[%s][%d] >\n", __func__, index++);
		//fgets(buf,sizeof(buf),stdin);
		//buf[strlen(buf)-1] = '\0';
		
        //释放条件变量
        pthread_cond_signal(&g_cond);
		//释放锁
        pthread_mutex_unlock(&g_mutex);

		if(strncmp(buf,"quit",4) == 0)
			break;
        sleep(1);
	}

	pthread_exit(NULL);
}

void *output_buffer(void *arg)
{
    int index = 0;
	while(1)
	{
		//申请锁
        pthread_mutex_lock(&g_mutex);

        //等待条件变量
        pthread_cond_wait(&g_cond, &g_mutex);
		printf("[%s][%d] %s.\n", __func__, index++, buf);

		//释放锁
        pthread_mutex_unlock(&g_mutex);
			
		if(strncmp(buf,"quit",4) == 0)
				break;
	}

	pthread_exit(NULL);
}


int main()
{
	int res;
	pthread_t tid[2];
    int *pvalue;
    int a = 100;

    //初始化线程锁
    pthread_mutex_init(&g_mutex, NULL); //or g_mutex = PTHREAD_MUTEX_INITIALIZER
    pthread_cond_init(&g_cond, NULL); //or g_cond = PTHREAD_COND_INITIALIZER

	//线程创建            TID    属性   线程函数     给线程函数传递的参数
	res = pthread_create(&tid[0],NULL,input_buffer,(void *)&a);
	if(res != 0){
		perror("Fail to pthread create");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&tid[1],NULL,output_buffer,NULL);
	if(res != 0){
		perror("Fail to pthread create");
		exit(EXIT_FAILURE);
	}
	
    //pthread_cancel(tid[0]); // send end cmd to pthred tid[0].
	//阻塞到到对应的子线程结束
	pthread_join(tid[0],NULL);//回收线程
	pthread_join(tid[1],(void *)&pvalue);//回收线程

    //销毁锁
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);

	exit(EXIT_FAILURE);
}
