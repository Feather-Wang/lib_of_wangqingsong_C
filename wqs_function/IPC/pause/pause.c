#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//注意:
//pause()会引起调用进程阻塞，直到有信号到来，然后继续向下执行
//（一个信号默认操作大部分是杀死进程，所以一般情况下，我们会对这个信号进行捕捉）
int main()
{
    //设置闹钟
    alarm(2);
    //等待闹钟信号的到来
	pause();
	printf("Over.\n");

	exit(EXIT_SUCCESS);
}
