#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libnet.h>
#include <linux/if_ether.h>

int main(int argc, char *argv[])
{
    int c;
    u_int32_t i;
    libnet_t *l;
    libnet_ptag_t t;
    char *device = "eth0";
    u_int8_t *packet;
    u_int32_t packet_s;
    char errbuf[LIBNET_ERRBUF_SIZE];
    unsigned char mac_src[6] = {0x08, 0x00, 0x27, 0xC1, 0x7E, 0x4B};
    unsigned char mac_dst[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    char *ip_src = "192.168.91.1";
    char *ip_dst = "0.0.0.0";

    printf("libnet 1.1 packet shaping: ARP[link -- autobuilding ethernet]\n"); 

    l = libnet_init(
            LIBNET_LINK_ADV,                        /* injection type */
            device,                                 /* network interface */
            errbuf);                                /* errbuf */

    if (l == NULL)
    {
        fprintf(stderr, "%s", errbuf);
        exit(EXIT_FAILURE);
    }
	else

    /*
     *  Build the packet, remmebering that order IS important.  We must
     *  build the packet from lowest protocol type on up as it would
     *  appear on the wire.  So for our ARP packet:
     *
     *  -------------------------------------------
     *  |  Ethernet   |           ARP             |
     *  -------------------------------------------
     *         ^                     ^
     *         |------------------   |
     *  libnet_build_ethernet()--|   |
     *                               |
     *  libnet_build_arp()-----------|
     */
	 
    i = libnet_get_ipaddr4(l);
  
    t = libnet_build_arp(
            ARPHRD_ETHER,                           /* hardware addr */
            ETHERTYPE_IP,                           /* protocol addr */
            6,                                      /* hardware addr size */
            4,                                      /* protocol addr size */
            ARPOP_REPLY,                            /* operation type */
            mac_src,                               /* sender hardware addr */
            (u_int8_t *)&i,                         /* sender protocol addr */
            mac_dst,                               /* target hardware addr */
            ip_dst,                         /* target protocol addr */
            NULL,                                   /* payload */
            0,                                      /* payload size */
            l,                                      /* libnet context */
            0);                                     /* libnet id */
    if (t == -1)
    {
        fprintf(stderr, "Can't build ARP header: %s\n", libnet_geterror(l));
        goto bad;
    }

    t = libnet_autobuild_ethernet(
            mac_dst,                               /* ethernet destination */
            ETHERTYPE_ARP,                          /* protocol type */
            l);                                     /* libnet handle */
    if (t == -1)
    {
        fprintf(stderr, "Can't build ethernet header: %s\n",
                libnet_geterror(l));
        goto bad;
    }


    if (libnet_adv_cull_packet(l, &packet, &packet_s) == -1)
    {
        fprintf(stderr, "%s", libnet_geterror(l));
    }
    else
    {
        fprintf(stderr, "packet size: %d\n", packet_s);
        libnet_adv_free_packet(l, packet);
    }

    c = libnet_write(l);

    if (c == -1)
    {
        fprintf(stderr, "Write error: %s\n", libnet_geterror(l));
        goto bad;
    }
    else
    {
        fprintf(stderr, "Wrote %d byte ARP packet from context \"%s\"; "
                "check the wire.\n", c, libnet_cq_getlabel(l));
    }
    libnet_destroy(l);
    return (EXIT_SUCCESS);
bad:
    libnet_destroy(l);
    return (EXIT_FAILURE);
}

/* EOF */
