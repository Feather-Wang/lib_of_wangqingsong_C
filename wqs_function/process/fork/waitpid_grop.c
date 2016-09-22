#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

        setpgrp(); // 自己创建一个组，PGID == PID
        printf("Create child process  pid1 (PID : %d  PGID : %d).\n",getpid(),getpgrp());
        while(1);

    }else{

        if((pid2 = fork()) < 0)
        {
            perror("Fail to fork");
            exit(EXIT_FAILURE);

        }else if(pid2 == 0){

            printf("Create child process pid2 (PID : %d  PGID : %d).\n",getpid(),getpgrp());
            while(1);
            //	printf("Child process (PID : %d  PGID :%d) exit.\n",getpid(),getpgrp());	

        }else{
            sleep(1);
            printf("Father PID : %d.\n",getpid());
            while(1)
            {
                printf("The father waitpid .\n");
                pid = waitpid(-1,&status,WUNTRACED | WCONTINUED | WNOHANG);

                if(pid == -1)
                    break;

                printf("CHILD PID : %d.\n",pid);

                if(WIFEXITED(status))
                {
                    printf("Normal exit.\n");

                }else if(WIFSIGNALED(status)){

                    printf("EXIT by signal(%d).\n",WTERMSIG(status));

                }else if(WIFSTOPPED(status)){

                    printf("Stop by signal(%d).\n",WSTOPSIG(status));
                }else if(WIFCONTINUED(status)){
                    printf("Continue bys SIGCONT.\n");

                }else{

                    printf("Unknown.\n");
                }

            }
        }

    }

    return 0;
}
