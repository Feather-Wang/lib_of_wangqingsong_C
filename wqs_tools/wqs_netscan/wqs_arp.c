#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wqs_arp.h"

void get_errmsg(int err, char *errmsg)
{
    switch( err )
    {
        case -101:
            sprintf(errmsg, "Get Local Mac failed");
            break;
        case -102:
            sprintf(errmsg, "Get Local IP failed");
            break;
        case -103:
            sprintf(errmsg, "This is not ARP Reply");
            break;
        case -104:
            sprintf(errmsg, "This is not specified IP returned");
            break;
        default:
            sprintf(errmsg, "this errno is not invalid");
            break;
    }
}

int get_ip_mac_by_interface(char *dev_name, unsigned char mac[ETH_ALEN], struct in_addr *ip)
{
    int reqfd = 0;
    struct ifreq macreq;

    reqfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* specified the interface name */
    strcpy(macreq.ifr_name, dev_name);

    /* 获取本地接口MAC地址*/
    if(ioctl(reqfd, SIOCGIFHWADDR, &macreq) != 0)
        return -101;
    memcpy(mac, macreq.ifr_hwaddr.sa_data, ETH_ALEN);

    /* 获取本地接口IP地址*/
    if(ioctl(reqfd, SIOCGIFADDR, &macreq) != 0)
        return -102;
    memcpy(ip, &(((struct sockaddr_in *)(&(macreq.ifr_addr)))->sin_addr), sizeof(*ip));

    close(reqfd);

    return 0;
}

int arp_build(struct arp_req *arp_send, uint8_t *ip_dst, char *dev_name)
{
    unsigned char local_mac[ETH_ALEN] = {0};
    struct in_addr local_ip;
    int ret = -1;

    memset(&local_ip, 0x00, sizeof(local_ip));

    ret = get_ip_mac_by_interface(dev_name, local_mac, &local_ip);
    if( 0 != ret )
        return ret;

    /* 填写以太网头部*/
    memcpy(arp_send->eh.ether_dhost, MAC_BCAST_ADDR, ETH_ALEN);
    memcpy(arp_send->eh.ether_shost, local_mac, ETH_ALEN);
    arp_send->eh.ether_type = htons(ETHERTYPE_ARP);

    /* 填写arp数据 */
    arp_send->ea.arp_hrd = htons(ARPHRD_ETHER);
    arp_send->ea.arp_pro = htons(ETHERTYPE_IP);
    arp_send->ea.arp_hln = ETH_ALEN;
    arp_send->ea.arp_pln = sizeof(local_ip);
    arp_send->ea.arp_op = htons(ARPOP_REQUEST);
    memcpy(arp_send->ea.arp_sha, local_mac, ETH_ALEN);
    memcpy(arp_send->ea.arp_spa, &local_ip, sizeof(local_ip));
    inet_aton(ip_dst, (struct in_addr*)arp_send->ea.arp_tpa);

    return 0;
}

int arp_solve(uint8_t *arp_reply, uint8_t *ip_dst, uint8_t *wqs_netscan)
{
    struct arp_req *arp_recv = (struct arp_req *)arp_reply;
    struct in_addr ip_addr;

    if( ARPOP_REPLY != ntohs(arp_recv->ea.arp_op) )
        return -103;

    memset(&ip_addr, 0x00, sizeof(ip_addr));
    inet_aton(ip_dst, &ip_addr);

    if( memcmp(arp_recv->ea.arp_spa, &ip_addr, sizeof(ip_addr)) )
        return -104;

    sprintf(wqs_netscan, "DestIP=[%s], ReturnIP=[%d:%d:%d:%d], Mac=[%02X:%02X:%02X:%02X:%02X:%02X]", 
            ip_dst,
            arp_recv->ea.arp_spa[0], arp_recv->ea.arp_spa[1], arp_recv->ea.arp_spa[2], arp_recv->ea.arp_spa[3],
            arp_recv->ea.arp_sha[0], arp_recv->ea.arp_sha[1], arp_recv->ea.arp_sha[2], arp_recv->ea.arp_sha[3], arp_recv->ea.arp_sha[4], arp_recv->ea.arp_sha[5]);

    return 0;
}
