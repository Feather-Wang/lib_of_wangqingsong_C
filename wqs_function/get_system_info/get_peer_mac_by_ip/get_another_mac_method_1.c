#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <sys/ioctl.h>

/* struct arpreq 结构体 */
#include <netinet/if_ether.h>

int Get_Entry()
{
    int sd;

    struct arpreq arpreq;
    struct sockaddr_in *sin;
    struct in_addr ina;
    char ip[] = "192.168.91.46";
    unsigned char *hw_addr;

    int rc;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket() error\n");
        exit(1);
    }
    printf("Find arp entry for IP : %s\n", ip);

    memset(&arpreq, 0, sizeof(struct arpreq));

    sin = (struct sockaddr_in *) &arpreq.arp_pa;
    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    ina.s_addr = inet_addr(ip);
    memcpy(&sin->sin_addr, (char *)&ina, sizeof(struct in_addr));

    strcpy(arpreq.arp_dev, "eth0");

    rc = ioctl(sd, SIOCGARP, &arpreq);

    if (rc < 0)
    {
        perror("Entry not available in cache...\n");
    }
    else
    {
        printf("\nentry has been successfully retreived\n");
        hw_addr = (unsigned char *) arpreq.arp_ha.sa_data;

        printf("HWAddr found : %02X:%02X:%02X:%02X:%02X:%02X\n",  
                hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]);
    }

    return 0;
} 
int main(int argc, char *argv[])
{
    Get_Entry();
    return 0;
}
