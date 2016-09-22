#ifndef __WQS_SYSTEM_FUNC_REWRITE_H__

#define __WQS_SYSTEM_FUNC_REWRITE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int judgment(const char *name); // 程序状态判断函数
int wqs_system_s(const char *cmd_line);
int wqs_system_p(const char *cmd_line, char *return_msg);
char * wqs_fgets(char *s, int size, FILE *stream);
int ReplaceIp(char **ut_buf, unsigned int *len, char *src, char *dst, int count_times);

#endif
