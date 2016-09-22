#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
 *exec函数族
 *
 *l:列举参数
 *execl(可执行文件所在路径,可执行文件名,参数1，参数2,...,NULL);
 *
 *p:指可执行文件的搜索路径是PATH
 *execlp(可执行文件名，可执行文件名,参数1,参数2,....,NULL)
 *
 *e:传递环境变量,可以通过getenv()函数获得
 *execle(可执行文件所在路径,可执行文件名，参数1，惨素2,...,char *const envp[]);
 *
 *v:以数组的方式传递参数，注意数组的第一个元素必须是可执行文件名，后面的元素始其参数或为NULL
 *execv(可执行文件所在路径，参数数组)
 *execvp(可执行文件名，参数数组)
 */

int main()
{

#if 0
	if(execl("/bin/ls","ls","-l",NULL) < 0)
	{
		perror("Fail to execl");
		exit(EXIT_FAILURE);
	}
#endif 

#if 0
	if(execlp("ls","ls","-a","-l",NULL) < 0)
	{
		perror("Fail to execlp");
		exit(EXIT_FAILURE);
	}
#endif

#if 1
	char *const envp[] = {"PATH1=/bin/sh","PATH2=/bin/bash",NULL};
	if(execle("./test","test","option",NULL,envp) < 0)
	{
		perror("Fail to execle");
		exit(EXIT_FAILURE);
	}

#endif 

#if 0
	char *const argv[] = {"ls","-l","-a",NULL};
	if(execv("/bin/ls",argv) < 0)
	{
		perror("Fail to execv");
		exit(EXIT_FAILURE);
	}
#endif 

#if 0
	char *const argv[] = {"ls","-l","-a",NULL};
	if(execvp("ls",argv) < 0)
	{
		perror("Fail to execv");
		exit(EXIT_FAILURE);
	}
#endif 

	return 0;
}
