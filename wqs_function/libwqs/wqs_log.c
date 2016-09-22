#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "wqs_log.h"

#define SIZE_128 128

struct wqs_syslog_t{
    unsigned char flag;
    pthread_mutex_t *rw_mutex;
    pthread_mutexattr_t attr;
};

static struct wqs_syslog_t *wqs_log;

static int getsystime(char *time_buf){
    time_t t = time(NULL);
    struct tm *ptr;

    ptr = localtime(&t);

    strftime(time_buf, 64, "%F %T", ptr);
    return 0;
}

int wqs_syslog_init(unsigned char log_flag)
{
    wqs_log = (struct wqs_syslog_t*)malloc(sizeof(struct wqs_syslog_t));
    if( NULL == wqs_log )
        return -1;

    memset(wqs_log, 0x00, sizeof(struct wqs_syslog_t));

    wqs_log->flag = log_flag;

    pthread_mutexattr_init(&wqs_log->attr);
    wqs_log->rw_mutex = mmap(0, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1, 0);
    pthread_mutexattr_setpshared(&wqs_log->attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(wqs_log->rw_mutex, &wqs_log->attr);

    return 0;
}

int wqs_syslog_printf(int log_level, char *file, const char *func_name, int line, char *fmt,...)
{
    char timebuf[64];
    va_list arg;
    int flag_print_log = 0;

    getsystime(timebuf);
    va_start(arg, fmt);
    pthread_mutex_lock(wqs_log->rw_mutex);

    switch (log_level & wqs_log->flag)
    {
    case LOG_INFO:
        printf(wqs_LOG_GREEN"[INFO]"wqs_LOG_NONE"[%s][%s][%s][Line:%d]", timebuf, file, func_name, line);
        flag_print_log = 1;
        break;
    case LOG_ERROR:
        printf(wqs_LOG_RED"[ERROR]"wqs_LOG_NONE"[%s][%s][%s][Line:%d]", timebuf, file, func_name, line);
        flag_print_log =1;
        break;
    case LOG_DEBUG:
        printf(wqs_LOG_BLUE"[DEBUG]"wqs_LOG_NONE"[%s][%s][%s][Line:%d]", timebuf, file, func_name, line);
        flag_print_log =1;
        break;
    case LOG_WARNING:
        printf(wqs_LOG_YELLOW"[WARNING]"wqs_LOG_NONE"[%s][%s][%s][Line:%d]", timebuf, file, func_name, line);
        flag_print_log =1;
        break;
    default:
        break;
    }

    if( flag_print_log )
    {
        vfprintf(stderr, fmt, arg);
        fflush(stderr);
    }

    pthread_mutex_unlock(wqs_log->rw_mutex);
    va_end(arg);

    return 0;
}

int wqs_syslog_destroy()
{
    pthread_mutexattr_destroy(&wqs_log->attr);
    pthread_mutex_destroy(wqs_log->rw_mutex);
    free(wqs_log);

    return 0;
}
