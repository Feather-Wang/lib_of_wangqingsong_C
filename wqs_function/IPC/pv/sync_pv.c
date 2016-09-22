#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

char buf[1024];

sem_t r_rs,w_rs;

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

void *input_buffer(void *arg)
{
	while(1)
	{
		
		P(&w_rs);//申请写资源

		printf(">");
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf)-1] = '\0';
		
		V(&r_rs);//释放读资源

		if(strncmp(buf,"quit",4) == 0)
			break;
	}

	pthread_exit(NULL);
}

void *output_buffer(void *arg)
{
	while(1)
	{
		
		P(&r_rs);//申请读资源

		printf("%s.\n",buf);

		V(&w_rs);//释放写资源
			
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

	init_semaphore(&r_rs,0);
	init_semaphore(&w_rs,1);


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

	exit(EXIT_FAILURE);
}
