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

static int log_time(char *timestr, int str_size)
{
    time_t time_now = time(NULL);
    struct tm now;

    localtime_r(&time_now, &now);

    strftime(timestr, str_size, "%Y-%m-%d %H:%M:%S", &now);
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

int wqs_syslog_destroy()
{
    pthread_mutexattr_destroy(&wqs_log->attr);
    pthread_mutex_destroy(wqs_log->rw_mutex);
    free(wqs_log);

    return 0;
}

int wqs_syslog_printf(int log_level, char *file, const char *func_name, int line, char *fmt,...)
{
    char timestr[64] = {0};
    char head[1024] = {0};
    char body[1024] = {0};
    char logstring[1024] = {0};
    int body_len = 0;
    va_list arg;

    log_time(timestr, 64);

    va_start(arg, fmt);
    body_len = vsnprintf(body, 1024, fmt, arg);
    va_end(arg);

    if( 0 >= body_len )
        return body_len;

    switch (log_level & wqs_log->flag)
    {
    case LOG_INFO:
        snprintf(head, 1024, wqs_LOG_GREEN"[INFO]"wqs_LOG_NONE"[%s][%s:%d][%s]", timestr, file, line, func_name);
        break;
    case LOG_ERROR:
        snprintf(head, 1024, wqs_LOG_RED"[ERROR]"wqs_LOG_NONE"[%s][%s:%d][%s]", timestr, file, line, func_name);
        break;
    case LOG_DEBUG:
        snprintf(head, 1024, wqs_LOG_BLUE"[DEBUG]"wqs_LOG_NONE"[%s][%s:%d][%s]", timestr, file, line, func_name);
        break;
    case LOG_WARNING:
        snprintf(head, 1024, wqs_LOG_YELLOW"[WARNING]"wqs_LOG_NONE"[%s][%s:%d][%s]", timestr, file, line, func_name);
        break;
    default:
        break;
    }

    snprintf(logstring, 1024, "%s %s", head, body);

    pthread_mutex_lock(wqs_log->rw_mutex);
    fwrite(logstring, strlen(logstring), 1, stdout);
    fflush(stdout);
    pthread_mutex_unlock(wqs_log->rw_mutex);

    return strlen(logstring);
}
