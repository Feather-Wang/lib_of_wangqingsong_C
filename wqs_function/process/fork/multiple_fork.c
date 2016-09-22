#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//进程最大数 /proc/sys/kernel/pid_max

int global_var = 5;

int main(int argc, char *argv)
{
	pid_t pid;
	int local_var = 6;
    int multiple = 2;

    int i = 0;
    for(i = 0; i < multiple; i++)
    {
        pid = fork();
        if( 0 > pid )
        {
		    perror("Fail to fork 111");
            i --;
        }
        else if( 0 == pid )
        {
            break;
        }
    }

	if( 0 > pid )
	{
		perror("Fail to fork");
		exit(EXIT_FAILURE);

	}else if( 0 == pid ){
		/*child process start exec code*/
		while(1)
		{
			global_var ++;
			local_var ++;

			printf("\n**********************************************\n");
			printf("The pid = %d.\n",pid);
			printf("The child process(PID = %d  PPID = %d) : ",getpid(),getppid());
			printf("Hello word.\n");
			printf("global_var = %d,local_var = %d.\n",global_var,local_var);
			printf("\n**********************************************\n");
			sleep(1);
		}

	}else{//pid > 0,the pid value is child process PID
		/*father process start exec code after create child process*/
		while(1)
		{
			printf("\n############################################\n");
			printf("The pid = %d.\n",pid);
			printf("The father process(PID = %d  PPID = %d) : ",getpid(),getppid());
			printf("Hello word.\n");
			printf("global_var = %d,local_var = %d.\n",global_var,local_var);
			printf("\n############################################\n");
			sleep(1);
		}
	}
	
	return 0;
}
