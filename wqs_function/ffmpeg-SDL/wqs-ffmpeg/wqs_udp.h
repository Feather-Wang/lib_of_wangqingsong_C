#ifndef __WQS_UDP_H__

#define __WQS_UDP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "wqs_sdk.h"

typedef struct sockaddr SA;

#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

void *wqs_udp(void *arg);
void *wqs_readfile(void *arg);

#endif
