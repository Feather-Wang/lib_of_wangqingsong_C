/*
flock()是系统调用

注意：另外，需要注意的是，flock()的锁和fcntl/lockf的锁是互不干涉、互不影响的。所以说，你可以同时给一个文件描述符fd加上flock和fcntl/lockf两个锁

通过/proc/locks可以查看fcntl打开的锁。

函数原型：
nclude <sys/file.h>
int flock(int fd, int operation);
Apply or remove an advisory lock on the open file specified by fd，通过fd在一个指定的打开的文件上添加和一处一个建议性锁。
operation：
LOCK_SH ：共享锁（读锁）
LOCK_EX ：排他锁（写锁）或者独占锁
LOCK_UN : 解锁。
LOCK_NB：非阻塞（与以上三种操作一起使用）

特性：
1）只能给整个文件上锁，而不能对文件的某一部分上锁，这是于fcntl/lockf的第一个重要区别，后者可以对文件的某个区域上锁。
2）flock只能产生劝告性锁。我们知道，linux存在强制锁（mandatory lock）和劝告锁（advisory lock）。所谓强制锁，比较好理解，就是你家大门上的那把锁，最要命的是只有一把钥匙，只有一个进程可以操作。所谓劝告锁，本质是一种协议，你访问文件前，先检查锁，这时候锁才起作用，如果你不管三七二十一，就要读写，那么劝告锁没有任何的作用。而遵守协议，读写前先检查锁的那些进程，叫做合作进程。
3）flock可以有共享锁（读锁）和排它锁（写锁），lockf只支持排它锁（写锁），但是fcntl里面参数可以有RDLCK读锁。
4）flock和fcntl/lockf的区别主要在fork和dup时候的区别，后面有讲。
5）flock不能再NFS文件系统上使用，如果要在NFS使用文件锁，请使用fcntl。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/file.h>

int main(int argc, const char *argv[])
{
    int fd = 0;
    char buf[1024] = {0};
    int index = 0;
    int ret = -1;
    printf("pid=[%d]\n", getpid());

    fd = open(argv[1], O_RDWR | O_CREAT, 0666);
    if( 0 > fd )
    {
        printf("open[%s] error\n", argv[1]);
        return -1;
    }

    printf("get flock before\n");
    ret = flock(fd, LOCK_EX);
    printf("get flock after,ret=[%d]\n",ret);

    for( index = 0; index < 10; index++ )
    {
        sprintf(buf, "[%s] index=[%d]\n", argv[0], index);
        ret = write(fd, buf, strlen(buf));
        printf("write,len=[%d]\n", ret);
        sleep(1);
    }

    ret = flock(fd, LOCK_UN);
    printf("get flock after,ret=[%d]\n",ret);

    close(fd);

    return 0;
}
