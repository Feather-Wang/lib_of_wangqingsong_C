#ifndef __WQS_SYSLOG_H__
#define __WQS_SYSLOG_H__

#include <stdio.h>

#define wqs_LOG_NONE   "\033[m"
#define wqs_LOG_RED    "\033[0;32;31m"
#define wqs_LOG_GREEN  "\033[0;32;32m"
#define wqs_LOG_YELLOW "\033[1;33m"
#define wqs_LOG_BLUE   "\033[0;32;34m"


#define WQS_LOG(log_level, ...) wqs_syslog_printf(log_level, __FILE__, __func__, __LINE__,__VA_ARGS__)

enum wqs_log_level{
  LOG_INFO=1,
  LOG_DEBUG=2,
  LOG_ERROR=4,
  LOG_WARNING=8,
};

int wqs_syslog_printf(int log_level, char *file, const char *func_name, int line, char *fmt,...);
int wqs_syslog_destroy();
int wqs_syslog_init(unsigned char log_flag);

#endif
