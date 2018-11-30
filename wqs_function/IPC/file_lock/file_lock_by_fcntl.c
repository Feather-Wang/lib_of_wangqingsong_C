/*
fcntl()是系统调用，而lockf()是库函数，是对fcntl()函数的进一步封装
fcntl支持强制性锁：对一个特定文件打开其设置组ID位(S_ISGID)，并关闭其组执行位(S_IXGRP)，则对该文件开启了强制性锁机制。再Linux中如果要使用强制性锁，则要在文件系统mount时，使用-omand打开该机制。
注意：另外，需要注意的是，flock()的锁和fcntl/lockf的锁是互不干涉、互不影响的。所以说，你可以同时给一个文件描述符fd加上flock和fcntl/lockf两个锁
通过/proc/locks可以查看fcntl打开的锁。

函数原型：
函数原型：
#include <unistd.h>
int lockf(int fd, int cmd, off_t len);
通过函数参数的功能，可以看出lockf只支持排他锁（写锁），不支持共享锁（读锁）。
cmd：
F_LOCK：给文件互斥加锁，若文件以被加锁，则会一直阻塞到锁被释放。
F_TLOCK：同F_LOCK，但若文件已被加锁，不会阻塞，而回返回错误。
F_ULOCK：解锁。
F_TEST：测试文件是否被上锁，若文件没被上锁则返回0，否则返回-1。
len:
为从文件当前位置的起始要锁住的长度

#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, arg);
arg：arg部分是一个结构体参数，结构体定义的部分结构为：
struct flock 
{
...
short l_type; //Type of lock: F_RDLCK（读锁）, F_WRLCK（写锁）, F_UNLCK（释放锁）
short l_whence; //How to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END
off_t l_start; //Starting offset for lock
off_t l_len; //Number of bytes to lock 
pid_t l_pid; // PID of process blocking our lock (F_GETLK only) 
...
}

特性：
1）可递归（同flock）（不明白这里指的递归是什么意思）
2）加读锁（共享锁）文件必须是读打开的，加写锁（排他锁）文件必须是写打开。
3）进程不能使用F_GETLK命令来测试它自己是否再文件的某一部分持有一把锁。
4）进程终止时，他所建立的所有文件锁都会被释放（同flock）。
5）任何时候关闭一个描述符时，则该进程通过这一描述符可以引用的文件上的任何一把锁都被释放（这些锁都是该进程设置的），这一点与flock不同。
示例：
fd1 = open(pathname, …);
lockf(fd1, F_LOCK, 0);
fd2 = dup(fd1);
close(fd2);
则在close(fd2)后，再fd1上设置的锁会被释放，如果将dup换为open，以打开另一描述符上的同一文件，则效果也一样。
fd1 = open(pathname, …);
lockf(fd1, F_LOCK, 0);
fd2 = open(pathname, …);
close(fd2);
6）由fork产生的子进程不继承父进程所设置的锁，这点与flock也不同。（因为flock创建的锁是和文件打开表项（struct file）相关联的，而不是fd，所以复制出了fd都可以操作这把锁，所以子进程继承了父进程的锁。flock里面要关闭所有复制出的fd，锁才会释放）
7）在执行exec后，新程序可以继承原程序的锁，这点和flock是相同的。（如果对fd设置了close-on-exec，则exec前会关闭fd，相应文件的锁也会被释放）。
8）支持强制性锁（跟flock不同），在介绍中写了怎么使用强制性锁。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    int fd = 0;
    struct flock f_lock;
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

    f_lock.l_type = F_WRLCK; //写锁
    //f_lock.l_type = F_RDLCK; //读锁
    f_lock.l_whence = 0;
    f_lock.l_len = 0;   /*0表示整个文件加锁*/

    printf("get flock before\n");
    ret = fcntl(fd, F_SETLKW, &f_lock);
    printf("get flock after,ret=[%d]\n",ret);

    sleep(5);

    for( index = 0; index < 10; index++ )
    {
        sprintf(buf, "[%s] index=[%d]\n", argv[0], index);
        ret = write(fd, buf, strlen(buf));
        printf("write,len=[%d]\n", ret);
        sleep(1);
    }

    f_lock.l_type = F_UNLCK;
    printf("unlock before\n");
    ret = fcntl(fd, F_SETLKW, &f_lock);
    printf("unlock after,ret=[%d]\n",ret);

    close(fd);

    return 0;
}
