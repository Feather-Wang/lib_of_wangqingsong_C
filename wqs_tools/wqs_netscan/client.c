#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "wqs_arp.h"

typedef struct sockaddr SA;
int g_flag_silence;

int main(int argc, char *argv[])
{
    int ret = -1;
    int sock_fd = 0;
    char errmsg[1024] = {0};
    char recv_buf[1280] = {0};
    unsigned char mac_dst[ETH_ALEN] = {0};
    struct sockaddr_ll broadcast_addr;
    struct sockaddr_ll broadcast_dst;
    unsigned int dst_len = 0;
    struct arp_req arp_send;

    do{
        if( argc != 3 && argc != 4 )
        {
            puts("Error : ./wqs_arp interface_name dst_ip_addr [--silence]");
            break;
        }
        if( argc == 4 )
        {
            if( 0 == strcmp( argv[3], "--silence") )
            {
                g_flag_silence = 1;
            }
            else
            {
                puts("Error : ./wqs_arp interface_name dst_ip_addr [--silence]");
            }
        }
        char *dev_name = argv[1];
        char *ip_dst = argv[2];
        int ret_len = 0;

        if( 0 > (sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) ) 
        {
            perror("socket error");
            break;
        }

        memset(&broadcast_addr, 0x00, sizeof(broadcast_addr));
        memset(&broadcast_dst, 0x00, sizeof(broadcast_dst));
        broadcast_addr.sll_family = PF_PACKET;
        broadcast_addr.sll_ifindex = if_nametoindex(dev_name);

        memset(&arp_send, 0x00, sizeof(arp_send));
        ret = arp_build(&arp_send, ip_dst, dev_name);
        if( 0 != ret )
        {
            get_errmsg( ret, errmsg );
            if( !g_flag_silence )
                fprintf(stderr, "arp_build : ret=[%d], errmsg=[%s]\n", ret, errmsg);
            break;
        }

        if( 0 >= (ret_len = sendto(sock_fd, &arp_send, sizeof(arp_send), 0, (SA *)&broadcast_addr, sizeof(broadcast_addr))) ) 
        {
            perror("Sendto error");
            break;
        }

        if( 0 >= (recvfrom(sock_fd, recv_buf, sizeof(arp_send), 0, (SA *)&broadcast_dst, &dst_len)) )
        {
            perror("Recvfrom error");
            break;
        }

        char wqs_netscan[1024] = {0};
        ret = arp_solve(recv_buf, ip_dst, wqs_netscan);
        if( 0 != ret )
        {
            get_errmsg( ret, errmsg );
            if( !g_flag_silence )
                fprintf(stderr, "arp_solve : ret=[%d], errmsg=[%s]\n", ret, errmsg);
            break;
        }
        printf("wqs_netscan : %s\n", wqs_netscan);

        ret = 0;

    }while(0);

    if( 0 < sock_fd )
        close(sock_fd);

    return ret;
}
