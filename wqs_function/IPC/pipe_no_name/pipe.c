#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 执行：./a.out
int main()
{
	int fd[2];
	pid_t pid;

    // 创建无名管道描述符，一共两个描述符，fd[0]用于读管道数据，fd[1]用于向管道中写数据
    // 管道的大小有限，可以通过ulimit -a查看
	if(pipe(fd) < 0)
	{
		perror("Fail to pipe");
		exit(EXIT_FAILURE);
	}

	if((pid = fork()) < 0)
	{
		perror("Fail to fork");
		exit(EXIT_FAILURE);
	
	}else if(pid == 0){
		close(fd[1]);
        char read_buf[128] = {0};
		while(1)
		{
			printf("The child process read pipe.");
			read(fd[0],read_buf,128);
			printf("child read : %s  %d bytes.\n",read_buf, strlen(read_buf));
		}
	
	}else{
		close(fd[0]);
        char write_buf[128] = {0};
		while(1)
		{
			printf(">");
			fgets(write_buf,sizeof(write_buf),stdin);

			write(fd[1],write_buf,strlen(write_buf));
		}
	}


	exit(EXIT_FAILURE);
}
