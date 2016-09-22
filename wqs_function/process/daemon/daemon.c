#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int Daemon()
{
	int i;
	pid_t pid;
	int fdtablesize;

	if((pid = fork()) < 0)
	{
		perror("Fail to fork");
		exit(EXIT_FAILURE);
	}else if(pid > 0){
		exit(EXIT_SUCCESS);
	}
	
	//创建新的会话期
	setsid();

	fdtablesize = getdtablesize();
	//关闭使用的文件描述符
	for(i = 0;i < fdtablesize;i ++)
		close(i);

	//改变工作目录
	chdir("/");

	//重设文件掩码
	umask(0);

	return 0;
}

int main(int argc,char *argv[])
{
	int flags = 0;
	int fd;
	char buf[] = "Test Daemon";

	if(argc < 2)
	{
		fprintf(stderr,"Usage : %s argv[1].\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	flags = atoi(argv[1]);

	if((fd = open("log",O_WRONLY | O_CREAT | O_APPEND,0666)) < 0)
	{
		fprintf(stderr,"Fail to open %s : %s.\n","log",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	if(!flags)
		Daemon();

	while(1)
	{
		write(fd,buf,strlen(buf));
		sleep(1);
	}

	return 0;
}
