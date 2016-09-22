#ifndef __WQS_SIGNAL_H__
#define __WQS_SIGNAL_H__

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

/*
 * test_recv_signal_1():通过sigprocmask()函数进信号进行阻塞处理设置
 * */
void test_recv_signal_1(int how, int signum);
/*
 * test_recv_signal_2():使用signal()逐个设置信号捕捉，然后设置处理方式（忽略或者调用回调函数）
 * */
void test_recv_signal_2();
/*
 * test_recv_signal_3():使用pause()，使当前进程挂起，等待任意信号的到来
 * */
void test_recv_signal_3();
/*
 * test_recv_signal_4():
 * 使用sigaction()对信号进程处理，比signal()更强大，属于POSIX标准
 * */
void test_recv_signal_4(int signal);
/*
 * test_recv_signal_5():
 * 使用sigwait()函数，延迟当前线程执行，等待信号集中的任意信号的到来
 * */
void test_recv_signal_5(int signum);
/*
 * test_recv_signal_6():
 * 使用pthread_sigmask()进行线程中的信号掩码设置
 * */
void test_recv_signal_6(void);
/********************************************************************/

/*
 * test_send_signal_1():使用alarm()设置定时器，发送SIGALRM信号
 * */
void test_send_signal_1(int time_alarm);
/*
 * test_send_signal_2():使用setitimer()设置定时器，发送SIGALRM信号
 * */
void test_send_signal_2(int inter_s, int inter_us, int alarm_s, int alarm_us);
/*
 * test_send_signal_3()：使用kill()主动向指定pid号的进程发送指定信号
 * */
void test_send_signal_3(pid_t pid, int signo);
/*
 * test_send_signal_4()：使用raise()主动向本进程发送指定信号
 * */
void test_send_signal_4(int signo);
/*
 * test_send_signal_5()：使用sigqueue()主动向本进程发送指定信号，并且可以携带一定的数据
 * */
void test_send_signal_5(pid_t pid, int sig);
/*
 * test_send_signal_6()：使用pthread_kill()主动向指定线程发送指定信号
 * */
void test_send_signal_6(int signum);

#endif
