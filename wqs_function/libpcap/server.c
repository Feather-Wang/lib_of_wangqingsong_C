#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <pthread.h>

typedef struct sockaddr SA;

#define  N  4000
#define SERVER_IP ("192.168.7.189")
#define SERVER_PORT 7777

#define MAXEPOLLSIZE 20
#define WAITTIME 5000

struct ether_header eth;
struct udphdr udp;
struct iphdr ip;

char *inet_nltoa(int hostlong)
{
    struct in_addr listenip;
    listenip.s_addr = hostlong;
    return inet_ntoa(listenip);
}
#if 0
int main(int argc, char *argv[])
{
    int server_fd = -1;
    char recv_buf[N];
    struct sockaddr_in server_addr,
                       client_addr;
    socklen_t clientaddr_len = 0;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(server_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("fail to bind");
        exit(-1);
    }

    struct sockaddr_in *ip = NULL;
    clientaddr_len = sizeof(client_addr);
    while ( 1 )
    {
                memset(&client_addr, 0, sizeof(client_addr));
                memset(recv_buf, 0, sizeof(recv_buf));
                recvfrom(server_fd, recv_buf, N, 0, (SA *)&client_addr, &clientaddr_len);
                printf("recv from [%s:%d] : %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_buf);

                memset(recv_buf, 0, sizeof(recv_buf));
                strcpy(recv_buf, "Welcome to Server");
                sendto(server_fd, recv_buf, N, 0, (SA *)&client_addr, sizeof(client_addr));
    }

    return 0;
}
#endif
void *pcap_server(void *arg) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    const u_char *packet = NULL;
    struct pcap_pkthdr pkthdr;

    int server_fd = -1;
    char recv_buf[N];
    struct sockaddr_in server_addr,
                       client_addr;
    socklen_t clientaddr_len = 0;

    if ((server_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("fail to socket");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(server_fd, (SA *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("fail to bind");
        exit(-1);
    }



    memset(errbuf, 0x00, PCAP_ERRBUF_SIZE);

    char dev[] = "eth1";

    int status = 0;
    handle = pcap_create(dev, errbuf);
    if (handle == NULL) {
        return NULL;
    }
    status = pcap_set_snaplen(handle, 4096);
    if (status < 0) {
        return NULL;
    }
    status = pcap_set_promisc(handle, 1); //opt_promisc);
    if (status < 0) {
        return NULL;
    }
    status = pcap_set_timeout(handle, 1000);
    if (status < 0) {
        return NULL;
    }
    status = pcap_set_buffer_size(handle, 8 * 1024 * 1024);
    if (status < 0) {
        return NULL;
    }
    struct bpf_program filter;
    int ret = pcap_compile(handle, &filter, "host 192.168.70.189", 1, 0);
    printf("compile ret = %d\n", ret);
    ret = pcap_setfilter(handle, &filter);
    printf("setfilter ret = %d\n", ret);
    pcap_activate(handle);
    int itotal = 0;

    //struct sockaddr_in *ip = NULL;
    clientaddr_len = sizeof(client_addr);
    while (1) 
    {
        packet = pcap_next(handle, &pkthdr);
#if 0
        puts("*************8");
        if( packet != NULL )
        {
            int w_len = recvfrom(server_fd, recv_buf, N, 0, (SA *)&client_addr, &clientaddr_len);
            printf("w_len = %d\n", w_len);
        }
#endif
#if 1
        if (packet != NULL) 
        {
            memcpy(&eth, packet, sizeof(eth));
            if (eth.ether_type == 0x08) 
            { //IP
                memcpy(&ip, packet + sizeof (eth), sizeof (ip));
                if (ip.protocol == 0x11) 
                { //UDP
                    memcpy(&udp, packet + sizeof(eth) + ip.ihl * 4, sizeof(udp));
                    if( 7777 == htons(udp.dest) )
                    {
                        int wqs_len = sizeof(eth) + ip.ihl * 4 + sizeof(udp);
                        puts("***********************************************************************");
                        printf("sip = %s, port = %d\n", inet_nltoa(ip.saddr), htons(udp.source));
                        printf("dip = %s, port = %d\n", inet_nltoa(ip.daddr), htons(udp.dest));

                        int udp_len = htons(udp.len) - sizeof(udp);
                        printf("udp len = %d\n", udp_len);
                        int pkt_len = pkthdr.len - wqs_len;
                        printf("packet size = %d\n", pkt_len);
                        printf("number of bytes = %d\n", pkthdr.caplen);
                        
                        memset(&client_addr, 0, sizeof(client_addr));
                        //memset(recv_buf, 0, sizeof(recv_buf));
                        int w_len = recvfrom(server_fd, recv_buf, N, 0, (SA *)&client_addr, &clientaddr_len);
                        printf("w_len = %d\n", w_len);
                        puts(recv_buf);
                        //printf("recv from [%s:%d] : %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_buf);
#if 0
                        memset(recv_buf, 0, sizeof(recv_buf));
                        strcpy(recv_buf, "Welcome to Server");
                        sendto(server_fd, recv_buf, N, 0, (SA *)&client_addr, sizeof(client_addr));
#endif
                    }
                } 
                else 
                {
                    //packet isn't UDP ;
                }
            } 
            else 
            {
                //packet isn't IP ;
            }
        } 
        else
        {
            puts("receive packet error ;");
        }
#endif
    }
    return handle;
}

int main(int argc, char *argv[])
{

    pthread_t pid;
    pthread_create(&pid, NULL, (void *) pcap_server, NULL);
    while(1)
        sleep(1000);

    return 0;
}
