#include "wqs_signal.h"

void test_callback(int signo)
{
    printf("test_callback --> this is signo : %d\n", signo);
}

/* 
 *  sigset_t：信号集类型
 *  sigemptyset():清空信号集
 *  sigaction()：
 *  sigaddset：将制定信号添加到信号集中
 *  sigprocmask()：
 *
 *  */

void test_recv_signal_1(int how, int signum)
{
    sigset_t set;

    sigaddset(&set, signum);    

    sigprocmask(how, &set, NULL);

    return;
}

/* 第二种处理信号的方式 
 * signal()：设置信号的处理函数
 *
 * */

void test_recv_signal_2()
{
    signal(SIGINT, test_callback);
    signal(SIGALRM, test_callback);
}
/*
 * 第三种处理信号的方式
 * pause()：暂时将处理挂起，当接收到有信号时（任何信号都可以），函数返回
 * 注意：该函数在线程中会直接堵塞，发送信号没有反应
 * */
void test_recv_signal_3()
{
    int res = pause();
    printf("signum is %d\n", res);
}
/* 
 *  struct sigaction:信号处理参数结构体 
 *  sigemptyset():清空信号集
 *  sigaction()：设置信号的处理参数
 *
 *  */
void test_recv_signal_4(int signal)
{
   struct sigaction act, old_act;
   memset(&act, 0x00, sizeof(act));
   memset(&old_act, 0x00, sizeof(old_act));

   //清空此信号集
   sigemptyset(&act.sa_mask);

   //添加信号处理函数被调用期间，需要阻塞的信号
   //sigaddset(&act.sa_mask, SIGALRM);

   //设置该信号的处理参数，并将原处理参数保存到old_act中
   act.sa_handler = test_callback;
   //act.sa_flags = SA_RESETHAND;
   sigaction(signal, &act, &old_act);

   // 恢复该信号的原始处理参数
   //sigaction(signal, &old_act, NULL);
}
/* 
 *  sigwait()：将当前线程延迟执行，等待信号集中任意信号的到来
 *  注意：如果sigaction和sigwait同时作用于一个信号，则sigwait生效，sigaction的信号处理将不会被调用
 *  */
void test_recv_signal_5(int signum)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, signum);

#if 0
        int sig = 0;
        sigwait(&set, &sig);
        printf("test_recv_signal_5 --> sigwait signum = %d\n", sig);
#elif 0
        siginfo_t info;
        memset(&info, 0x00, sizeof(info));
        sigwaitinfo(&set, &info);
        printf("test_recv_signal_5 --> sigwaitinfo signum = %d\n", info.si_signo);
#else
        siginfo_t info;
        memset(&info, 0x00, sizeof(info));
        struct timespec timeout;
        timeout.tv_sec = 2;
        timeout.tv_nsec = 0;
        sigtimedwait(&set, &info, &timeout);
        printf("test_recv_signal_5 --> sigtimedwait signum = %d\n", info.si_signo);
#endif
}

/* 
 * pthread_sigmask():用于多线程中的信号掩码，线程中的信号掩码继承进程中的信号掩码
 *  */
#include <pthread.h>
void *pthread_thread(void *arg)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    test_recv_signal_5(SIGINT);
}

pthread_t g_td;
void test_recv_signal_6(void)
{
    pthread_create(&g_td, NULL, pthread_thread, NULL);
    pthread_detach(g_td);
}


/*
 * 主动发送信号的方式
 * alarm()：设置闹钟函数，只能发送SIGALRM信息，闹钟时间单位为秒
 * 注意:
 * 1.每个进程只能设置一个闹钟
 * 2.如果设置多个以最后一次为主
 * 3.定时时间到达的时候，内核会向进程发送SIGALRM(14),这个信号默认是杀死进程，也可以使用signal()函数拦截SIGALRM信息，自己处理
 * */
void test_send_signal_1(int time_alarm)
{
    int res = alarm(time_alarm);
    printf("alarm return %d\n", res);
}
/*
 * 主动发送信号的方式
 * setitimer：设置闹钟函数，只能发送SIGALRM信息,与alarm()不同的是，setitimer()可以精确到微秒
 * 注意:
 * 1.每个进程只能设置一个闹钟
 * 2.如果设置多个以最后一次为主
 * 3.定时时间到达的时候，内核会向进程发送SIGALRM(14),这个信号默认是杀死进程，也可以使用signal()函数拦截SIGALRM信息，自己处理
 *
 * getitimer():获取闹钟剩余时间，it_value成员表示剩余时间，it_interval成员表示闹钟触发后，后续发送信号的时间间隔，如果为0，则表示信号只发送一次。
 * */
void test_send_signal_2(int inter_s, int inter_us, int alarm_s, int alarm_us)
{
    struct itimerval it;

    it.it_interval.tv_sec = inter_s;
    it.it_interval.tv_usec = inter_us;
    it.it_value.tv_sec = alarm_s;
    it.it_value.tv_usec = alarm_us;
    setitimer(ITIMER_REAL, &it, NULL);
}
/*
 * 主动发送信号的方式
 * kill()：主动向指定pid号的进程发送指定信号
 * */
void test_send_signal_3(pid_t pid, int signo)
{
    kill(pid, signo);
}
/*
 * 主动发送信号的方式
 * raise()：主动向本进程发送指定信号
 * */
void test_send_signal_4(int signo)
{
    raise(signo);
}
/*
 * 主动发送信号的方式
 * sigqueue()：主动向本进程发送指定信号
 * */
void test_send_signal_5(pid_t pid, int sig)
{
    const union sigval value;
    sigqueue(pid, sig, value);
}

void test_send_signal_6(int signum)
{
   pthread_kill(g_td, signum); 
}
