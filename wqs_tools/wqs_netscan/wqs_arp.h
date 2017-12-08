#ifndef __WQS_ARP_H__
#define __WQS_ARP_H__

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>

/* get struct sockaddr_ll */
#include <netpacket/packet.h>
/* get struct ifreq */
#include <net/if.h>
/* get struct ether_arp */
#include <netinet/if_ether.h>

#define MAC_BCAST_ADDR  (unsigned char *) "\xff\xff\xff\xff\xff\xff"

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct arp_req
{
    struct ether_header eh;
    struct ether_arp ea;
    u_char padding[18];
};

extern void get_errmsg(int err, char *errmsg);
extern int arp_build(struct arp_req *arp_send, uint8_t *ip_dst, char *dev_name);
extern int arp_solve(uint8_t *arp_reply, uint8_t *ip_dst, uint8_t *wqs_netscan);

#endif
