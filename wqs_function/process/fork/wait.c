#include <stdio.h>  
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

/*
 *pid_t wait(&status);
 *
 * 特点:会导致调用wait的进程阻塞,阻塞到有子进程变成僵尸态
 * ,wait函数会释放僵尸态子进程未释放的资源
 *
 * 注意:如果没有子进程，wait函数会立即返回，此时返回值为-1
 *
 */

int main()
{
    pid_t pid1,pid2;
    pid_t pid;
    int status;

    if((pid1 = fork()) < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);

    }else if(pid1 == 0){

        printf("Create child process ( PID : %d PPID : %d).\n",getpid(),getppid());
        sleep(3);
        exit(0x0122);

    }else{

        if((pid2 = fork()) < 0)
        {
            perror("Fail to fork");
            exit(EXIT_FAILURE);

        }else if(pid2 == 0){

            printf("Create child process ( PID : %d PPID : %d).\n",getpid(),getppid());
            while(1);

        }else{
            /*father process start exec code after create child process*/
            /*note : pid > 0,the kernel return child process pid for father process*/

            while(wait(&status) > 0)
            {
                printf("The father.\n");
                //正常退出
                if(WIFEXITED(status)){
                    printf("The child terminated normally,return exit statu : %#x.\n",WEXITSTATUS(status));
                }
                //非正常退出
                else if(WIFSIGNALED(status)){
                    printf("The child terminated by a signal,the signal num : %d.\n",WTERMSIG(status));
                }
                else if(WIFSTOPPED(status)){
                    printf("The child stopped by a signa,the signal num : %d.\n",WSTOPSIG(status));
                }
                else if(WIFCONTINUED(status)){
                    printf("The child resumed by delivery of SIGCONT.\n");
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
