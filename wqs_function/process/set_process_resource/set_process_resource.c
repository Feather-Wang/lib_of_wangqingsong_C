#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/time.h>
#include <sys/resource.h>

/*
获取/设置 进程、进程组或用户的进程执行优先级
which、who：
PRIO_PROCESS : 一个特定的进程，此时 who 的取值为进程 ID。
PRIO_PGRP : 一个进程组的所有进程，此时 who 的取值为进程组 ID。
PRIO_USER : 一个用户拥有的所有进程，此时参数 who 取值为实际用户 ID 。
prio:
指定优先级值，取值范围：[-20, 20], 数值越低代表有较高的优先次序, 执行会较频繁. 此优先权默认是0, 而只有超级用户 (root)允许降低此值.

返回值：
执行成功则返回0, 如果有错误发生返回值则为-1, 错误原因存于errno.
1、ESRCH：参数which 或who 可能有错, 而找不到符合的进程
2、EINVAL：参数which 值错误.
3、EPERM：权限不够, 无法完成设置
4、EACCES：一般用户无法降低优先权 

注意：当指定的一组进程的优先级不同时, getpriority() 将返回其中优先级最低的一个。此外，当 getpriority() 返回 -1 时，可能是发生错误，也有可能是返回的是指定进程的优先级。区分它们的惟一方法是在调用 getpriority() 前将 errno 清零。如果函数返回 -1 且 errno 不为零，说明有错误产生。
*/
void set_process_priority(void)
{
    int which = 0;
    int who = 0;
    int prio = 0;

    int ret = 0;

    /*获取优先级*/
    which = PRIO_PROCESS;
    who = 0;
    prio = getpriority(which, who);
    printf("getpriority(), prio=[%d]\n", prio);

    which = PRIO_PROCESS;
    who = 0;
    prio = 12;
    ret = setpriority(which, who, prio);
    
}

/*
根据resource参数，获取/设置指定资源的使用限制，每一种资源都有相关的软限制和硬限制，软限制是内核强加给相应资源的限制值，硬限制是软限制的最大值。非授权调用进程只可以将其软限制指定为0-硬限制范围内的某个值，同时能不可逆转地降低其硬限制。授权进程可以任意改变其软硬限制
struct rlimit
{
    rlim_t rlim_cur;    // soft limit
    rlim_t rlim_max;    // hard limit ( ceiling for rlim_cur )
}

resource:
RLIMIT_AS           进程的最大虚内存空间，字节为单位。这个限制会影响brk、mmap、mremap，当越过了这个限制就会发生错误，并且errno被设置为ENOMEM，这个值是一个long型，在32位机器上这个限制最大2GB，或者不限制
RLIMIT_CORE         进程可以创建的最大core文件的大小，如果此限制为0，将不能创建，另外，当达到此限制时，所有正在写core文件的进程都将被中断 
RLIMIT_CPU          最大允许的CPU使用时间，秒为单位。当进程达到软限制，内核将给其发送SIGXCPU信号，这一信号的默认行为是终止进程的执行。然而，可以捕捉信号，处理句柄可将控制返回给主程序。如果进程继续耗费CPU时间，核心会以每秒一次的频率给其发送SIGXCPU信号，直到达到硬限制，那时将给进程发送 SIGKILL信号终止其执行。
RLIMIT_DATA         进程数据段的最大值。这个限制会影响brk、sbrk，当越过了这个资源的软限制会报错并设置errno为ENOMEM
RLIMIT_FSIZE        进程可建立的文件的最大大小。如果进程试图超出这一限制时，核心会给其发送SIGXFSZ信号，默认情况下将终止进程的执行。然而，可以捕捉信号，如果这样做的话，相关的系统调用（比如write()、truncate()）会失败并设置errno为EFBIG
RLIMIT_LOCKS        进程可建立的锁和租赁的最大值。
RLIMIT_MEMLOCK      进程在RAM中可以锁定的内存的最大数，单位为字节。实际上，这个限制被舍入到系统页大小的最近倍数。这个限制会影响mlock()、mlockall()、mmap() MAP_LOCKED操作。在Linux2.6.9以后的版本，也影响shmctl() SHM_LOCK操作，其中它对共享内存段中可能被调用进程的实际用户ID锁定的总字节设置最大值。
RLIMIT_MSGQUEUE     进程可为POSIX消息队列分配的最大字节数。
RLIMIT_NICE         进程可通过setpriority() 或 nice()调用设置的最大完美值。
RLIMIT_NOFILE       指定比进程可打开的最大文件描述符大1的值，超出此值，将会产生EMFILE错误。
RLIMIT_NPROC        用户可拥有的最大进程数。达到这个限制，fork()将会报错，并设置errno为EAGAIN错误
RLIMIT_RTPRIO       进程可通过sched_setscheduler 和 sched_setparam设置的最大实时优先级。
RLIMIT_SIGPENDING   用户调起的进程，可以排队等候的信号的数量的限制。标准信号和实时信号都会统计检查这个限制，无论如何，这个限制只会在调用sigqueue()时强制执行；它总可能使用kill()到任何一个还没有排队到进程的信号的一个实例。
RLIMIT_STACK        进程堆栈的最大大小，以字节为单位。当达到这个限制，将会产生一个SIGSEGV信号
*/

#include <sys/time.h>
#include <sys/resource.h>
void set_process_ulimit_resource(void)
{
    int resource = 0;
    struct rlimit rlim;
    int ret = -1;

    ret = getrlimit(RLIMIT_NOFILE, &rlim);
    printf("SYSTEM RLIMIT_NOFILE, cur=[%ld], max=[%ld]\n", rlim.rlim_cur, rlim.rlim_max);

    rlim.rlim_cur = 1000;
    rlim.rlim_max = 1200;
    ret = setrlimit(RLIMIT_NOFILE, &rlim);
    
    ret = getrlimit(RLIMIT_NOFILE, &rlim);
    printf("SYSTEM RLIMIT_NOFILE, cur=[%ld], max=[%ld]\n", rlim.rlim_cur, rlim.rlim_max);
}

int main(int argc, const char *argv[])
{
    /*设置进程优先级*/
    set_process_priority();

    /*设置进程资源*/
    set_process_ulimit_resource();

    while( 1 )
        sleep(1);
    return 0;
}
