#ifndef __WQS_DNS_H__
#define __WQS_DNS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>


typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef struct sockaddr SA;

#define DNS_MAXDN	255	/* max DN length */
#define DNS_MAXLABEL	63	/* max DN label length */
#define DNS_MAXNAME	1024	/* max asciiz domain name length */

struct dns_req{
  uint16_t id;
  uint16_t flags;
  uint16_t que;
  uint16_t ans;
  uint16_t ant;
  uint16_t add;
  char url[1000];
  uint16_t url_len;
  uint16_t type;
  uint16_t class;
};

struct dns_res{
  uint16_t id;
  uint16_t flags;
  uint16_t que;
  uint16_t ans;
  uint16_t ant;
  uint16_t add;
};

extern int dns_build(uint8_t *build_buf, uint8_t *url);
extern int dns_solve(uint8_t *buf, int len, uint8_t *ip_list);
/*非线程安全函数，返回的指针为静态数组*/
extern char *wqs_dns_strerror(int error_code);

#endif
