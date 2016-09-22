//注：如果是在手机上抓包，且使用的网络是手机数据网络，在获取IP协议包时，必须在起始地址，向后偏移2个字节，这是因为手机数据网络采用的是拨号协议，跟电脑网络协议有点区别

#include <stdio.h>
#include <malloc.h>
#include <pcap.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
//#define PCAP_ERRBUF_SIZE 1024*1024

struct ether_header eth;
struct udphdr udp;
struct tcphdr tcp;
struct iphdr ip;

char *inet_nltoa(int hostlong)
{
    struct in_addr listenip;
    listenip.s_addr = hostlong;
    return inet_ntoa(listenip);
}

void getPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    //puts("getPacket..................................");
    int *id = (int *)arg;
    char *buf;
    buf = (char*)malloc(4000);
    memcpy(&eth, packet, sizeof(eth));
    if (eth.ether_type == 0x08) 
    { //IP
        memcpy(&ip, packet + sizeof (eth), sizeof (ip));
        if (ip.protocol == 0x11) 
        { //UDP
            memcpy(&udp, packet + sizeof(eth) + ip.ihl * 4, sizeof(udp));

            memset(buf, 0x00, 4000);
            int wqs_len = sizeof(eth) + ip.ihl * 4 + sizeof(udp);
            puts("***********************************************************************");
            printf("sip = %s, port = %d\n", inet_nltoa(ip.saddr), htons(udp.source));
            printf("dip = %s, port = %d\n", inet_nltoa(ip.daddr), htons(udp.dest));
            int udp_len = htons(udp.len) - sizeof(udp);
            printf("udp len = %d\n", udp_len);
            int pkt_len = pkthdr->len - wqs_len;
            printf("packet size = %d\n", pkt_len);
            printf("number of bytes = %d\n", pkthdr->caplen);
            char *p = (char*) (packet + sizeof (eth) + ip.ihl * 4 + sizeof (udp));
            puts("wangqingsong");
            //arg != NULL ? memcpy(buf, p, udp_len) : memcpy(buf, p, pkt_len);
            memcpy(buf, p, udp_len);
            puts(buf);
        } 
        else if( 0x06 == ip.protocol ) 
        {
            memcpy(&ip, packet + sizeof(eth) + ip.ihl * 4, sizeof(tcp));

            memset(buf, 0x00, 4000);
            int wqs_len = sizeof(eth) + ip.ihl * 4 + sizeof(tcp);
            if( 0 != htons(tcp.dest) )
            {
                puts("***********************************************************************");
                printf("sip = %s, port = %d\n", inet_nltoa(ip.saddr), htons(tcp.source));
                printf("dip = %s, port = %d\n", inet_nltoa(ip.daddr), htons(tcp.dest));
            }
            //puts("pcap_method_1 --> this is tcp packet");
        }
    } 
    else 
    {
        //packet isn't IP ;
    }
    free(buf);
    //puts("\n\n");
}

void *pcap_method_1(void *arg) {
    puts("pcap_method_1");
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    const u_char *packet = NULL;
    struct pcap_pkthdr pkthdr;
    char *buf;
    buf = (char*)malloc(4000);

    memset(errbuf, 0x00, PCAP_ERRBUF_SIZE);

    char dev[] = "eth0";
    handle = pcap_open_live(dev, 65536, 1, 1000, errbuf);
    if (handle == NULL) {
        return NULL;
    }

    pcap_set_buffer_size(handle, 8 * 1024 * 1024);
#if 0
    struct bpf_program filter;
    pcap_compile(handle, &filter, "dst host 192.168.91.44 and dst port 22", 1, 0);
    pcap_setfilter(handle, &filter);
#endif
    while (1) 
    {
        packet = pcap_next(handle, &pkthdr);

        if (packet != NULL) 
        {
            memcpy(&eth, packet, sizeof(eth));
            if (eth.ether_type == 0x08) 
            { //IP
                memcpy(&ip, packet + sizeof (eth), sizeof (ip));
                if (ip.protocol == 0x11) 
                { //UDP
                    memcpy(&udp, packet + sizeof(eth) + ip.ihl * 4, sizeof(udp));

                    memset(buf, 0x00, 4000);
                    int wqs_len = sizeof(eth) + ip.ihl * 4 + sizeof(udp);
                    puts("***********************************************************************");
                    printf("sip = %s, port = %d\n", inet_nltoa(ip.saddr), htons(udp.source));
                    printf("dip = %s, port = %d\n", inet_nltoa(ip.daddr), htons(udp.dest));
                    int udp_len = htons(udp.len) - sizeof(udp);
                    printf("udp len = %d\n", udp_len);
                    int pkt_len = pkthdr.len - wqs_len;
                    printf("packet size = %d\n", pkt_len);
                    printf("number of bytes = %d\n", pkthdr.caplen);
                    char *p = (char*) (packet + sizeof (eth) + ip.ihl * 4 + sizeof (udp));
                    puts("wangqingsong");
                    //arg != NULL ? memcpy(buf, p, udp_len) : memcpy(buf, p, pkt_len);
                    memcpy(buf, p, udp_len);
                    if( arg )
                        puts(buf);
                } 
                else if( 0x06 == ip.protocol ) 
                {
                    memcpy(&tcp, packet + sizeof(eth) + ip.ihl * 4, sizeof(tcp));

                    memset(buf, 0x00, 4000);
                    int wqs_len = sizeof(eth) + ip.ihl * 4 + sizeof(tcp);
                    if( 7777 == htons(tcp.dest) || 7777 == htons(tcp.source) )
                    {
                        puts("***********************************************************************");
                        printf("sip = %s, port = %d\n", inet_nltoa(ip.saddr), htons(tcp.source));
                        printf("dip = %s, port = %d\n", inet_nltoa(ip.daddr), htons(tcp.dest));
                    }

                    //puts("pcap_method_1 --> this is tcp packet");
                }
            } 
            else 
            {
                puts("pcap_method_1 --> this is not ip packet");
            }
        } 
        else
        {
            puts("receive packet error ;");
        }
    }

    return handle;
}

void *pcap_method_2(void *arg) {
    puts("pcap_method_2");
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    const u_char *packet = NULL;
    struct pcap_pkthdr pkthdr;
    char *buf;
    buf = (char*)malloc(4000);

    memset(errbuf, 0x00, PCAP_ERRBUF_SIZE);

    char dev[] = "eth0";

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

#if 0
    struct bpf_program filter;
    pcap_compile(handle, &filter, "dst host 192.168.91.44 and dst port 22", 1, 0);
    pcap_setfilter(handle, &filter);
#endif
    pcap_activate(handle);
    int itotal = 0;
    int id = 0;
    pcap_loop(handle, -1, getPacket, (u_char*)&id);
    pcap_close(handle);
    return handle;
}

int main(int argc, char *argv[])
{
    int res;

    pcap_method_1();
    //pcap_method_2();

    return 0;
}
