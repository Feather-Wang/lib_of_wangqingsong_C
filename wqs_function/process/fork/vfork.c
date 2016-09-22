#include <stdio.h>
#include <stdlib.h>

/*
 *vfork与fork共同地方都可以创建子进程
 *
 *vfork特点:
 *1.子进程先运行，当子进程结束或者调用exec函数族的时候，父进程才开始运行
 *2.父子进程共用同一个物理空间
 注意:如果子进程调用exec函数族的时候，父子进程空间开始独立开来
 *
 */

/*return : 结束一个函数
 *exit : 结束一个进程
 */

int global_var = 10;

int main()
{
	pid_t pid;
	int local_var = 5;

	if((pid = vfork()) < 0)
	{
		perror("Fail to vfork");
		exit(EXIT_FAILURE);
	}else if(pid == 0){
		
		global_var ++;
		local_var ++;
		if(execl("/bin/ls","ls","-a",NULL) < 0)
		{
			perror("Fail to execl");
			exit(EXIT_FAILURE);
		}
		
		printf("The child process runing.\n");
		printf("global_var = %d  local_var = %d.\n",global_var,local_var);
			sleep(1);
	
	}else{

		printf("The father process runing.\n");
		printf("global_var = %d  local_var = %d.\n",global_var,local_var);
	
	}

	exit(EXIT_SUCCESS);
}
