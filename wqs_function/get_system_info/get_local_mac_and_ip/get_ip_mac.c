#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
/* get ETH_ALEN */
#include <netinet/if_ether.h>

#define MAXINTERFACES    16

/* get all of local network list ( both ip and mac ) by socket and ioctl */
int get_ip_mac_method_1(void)
{
    puts("get_ip_mac_method_2 #################################################################");
    register int fd = 0;
    int ip_count = 0;
    int index = 0;
    struct ifreq buf[MAXINTERFACES];
    struct arpreq arp;
    struct ifconf ifc;
    if ( -1 == (fd = socket (AF_INET, SOCK_DGRAM, 0)) )
    {
        perror("socket is Error : ");
        return -1;
    }

    ifc.ifc_len = sizeof buf;
    ifc.ifc_buf = (caddr_t) buf;
    if (ioctl (fd, SIOCGIFCONF, (char *) &ifc))
    {
        perror("ioctl SIOCGIFCONF is Error : ");
        return -1;
    }
    /*获取接口信息*/
    ip_count = ifc.ifc_len / sizeof (struct ifreq);
    printf("ip num is %d\n\n\n", ip_count);

    /*根据借口信息循环获取设备IP和MAC地址*/
    for(index = 0; index < ip_count; ++index)
    {
        if( !strcmp(buf[index].ifr_name, "lo") )
            continue;

        /*获取设备名称息*/
        printf ("net device %s\n", buf[index].ifr_name);

        /*判断网卡类型*/
        if (!(ioctl (fd, SIOCGIFFLAGS, (char *) &buf[index])))
        {
            if (buf[index].ifr_flags & IFF_PROMISC)
            {
                puts ("the interface is PROMISC");
            }
        }
        else
        {
            char str[256];
            sprintf (str, "cpm: ioctl device %s", buf[index].ifr_name);
            perror (str);
        }
        /*判断网卡状态*/
        if (buf[index].ifr_flags & IFF_UP)
        {
            puts("the interface status is UP");
        }
        else
        {
            puts("the interface status is DOWN");
        }
        /*获取当前网卡的IP地址*/
        if( !(ioctl (fd, SIOCGIFADDR, (char *) &buf[index])) )
        {
            puts ("IP address is:");
            printf("%s\n", inet_ntoa(((struct sockaddr_in*)(&buf[index].ifr_addr))->sin_addr));
        }
        else
        {
            char str[256];
            sprintf (str, "cpm: ioctl device %s", buf[index].ifr_name);
            perror (str);
        }

#if 0
        if( !(ioctl (fd,   SIOCGENADDR, (char *) &buf[index])) )
        {
            puts ("HW address is:");
            printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                    (unsigned char)buf[index].ifr_enaddr[0],
                    (unsigned char)buf[index].ifr_enaddr[1],
                    (unsigned char)buf[index].ifr_enaddr[2],
                    (unsigned char)buf[index].ifr_enaddr[3],
                    (unsigned char)buf[index].ifr_enaddr[4],
                    (unsigned char)buf[index].ifr_enaddr[5]);
            puts("");
            puts("");
        }
#endif
        if( !(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[index])) )
        {
            puts ("HW address is:");
            printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[0],
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[1],
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[2],
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[3],
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[4],
                    (unsigned char)buf[index].ifr_hwaddr.sa_data[5]);
            puts("");
            puts("");
        }
        else
        {
            char str[256];
            sprintf (str, "cpm: ioctl device %s", buf[index].ifr_name);
            perror (str);
        }
    } //while
    
    close (fd);
    printf("\n\n\n");
    return 0;
}

/* specified the interface name, and get its ip and mac by socket and ioctl */
int get_ip_mac_method_2(void)
{
    puts("get_ip_mac_method_2 #################################################################");
    int reqfd, n;
    struct ifreq macreq;
    unsigned char local_mac[ETH_ALEN] = {0};
    struct in_addr local_ip;

    memset(&local_ip, 0x00, sizeof(local_ip));

    reqfd = socket(AF_INET, SOCK_DGRAM, 0);
    /* specified the interface name */
    strcpy(macreq.ifr_name, "eth0");

    /* 获取本地接口MAC地址*/
    if(ioctl(reqfd, SIOCGIFHWADDR, &macreq) != 0)
        return 1;
    memcpy(local_mac, macreq.ifr_hwaddr.sa_data, ETH_ALEN);

    /* 获取本地接口IP地址*/
    if(ioctl(reqfd, SIOCGIFADDR, &macreq) != 0)
        return 1;
    memcpy(&local_ip, &(((struct sockaddr_in *)(&(macreq.ifr_addr)))->sin_addr), sizeof(local_ip));

    printf("ip = %s\n", inet_ntoa(local_ip));
    printf("mac = %02X:%02X:%02X:%02X:%02X:%02X\n",
            local_mac[0], local_mac[1], local_mac[2], local_mac[3], local_mac[4], local_mac[5]);

    printf("\n\n\n");
    return 0;
}

int main(int argc, const char *argv[])
{
    get_ip_mac_method_1();

    get_ip_mac_method_2();

    return 0;
}
