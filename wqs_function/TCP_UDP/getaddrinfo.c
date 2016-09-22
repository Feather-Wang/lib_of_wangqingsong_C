#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *name = "www.baidu.com";

    struct addrinfo hint;
    struct addrinfo *result;
    char ipstr[16];   

    memset(&hint, 0x00, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags |= AI_CANONNAME;

    int ret = getaddrinfo(name, NULL, &hint, &result);
    if ( 0 != ret ) 
    {
        fprintf(stderr,"getaddrinfo: &s/n", gai_strerror(ret));
        exit(1);
    }

    printf("canonname -> %s\n", result->ai_canonname);
    struct addrinfo *addr;
    for (addr = result; NULL != addr; addr = addr->ai_next) 
    {
        inet_ntop(AF_INET, &(((struct sockaddr_in *)(addr->ai_addr))->sin_addr), ipstr, 16);
        printf("%s\n", ipstr);
    }

    freeaddrinfo(result);

    exit(0);
}
