#include <linux/sockios.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <stdlib.h>
#include <string.h>
#include <linux/ethtool.h>
#include <unistd.h>

#define MAXINTERFACES 256

typedef struct sockaddr SA;
typedef struct sockaddr_in SAI;


int main(int argc, char **argv)
{
    register int fd, interface;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;
    struct in_addr ipstr;
    puts("START...");

    /*
       struct in_addr t;
       inet_aton("192.168.2.1", &t);
       */

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t) buf;
        printf("ifc_len = %d\n", ifc.ifc_len);
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc))
        {
            printf("ifc_len = %d\n", ifc.ifc_len);
            interface = ifc.ifc_len / sizeof(struct ifreq);
            while (interface-- > 0)
            {
                puts("**********************************************************");
                printf("ifr_name --> %s\n", buf[interface].ifr_name);
                
                if( ioctl(fd, SIOCGIFADDR, &buf[interface]) )
                    break;

                ipstr = ((SAI*)(&buf[interface].ifr_addr))->sin_addr;
                printf("ifr_addr --> %s\n", inet_ntoa(ipstr));
                
                if( ioctl(fd, SIOCGIFNETMASK, &buf[interface]) )
                    continue;
                ipstr = ((SAI*)(&buf[interface].ifr_netmask))->sin_addr;
                printf("ifr_netmask --> %s\n", inet_ntoa(ipstr));
            }
        }
        else
        {
            perror("cpm: ioctl");
        }
    }
    else
    {
        perror("cpm: socket");
    }
    close(fd);
    printf("0");
    return 0;
}
