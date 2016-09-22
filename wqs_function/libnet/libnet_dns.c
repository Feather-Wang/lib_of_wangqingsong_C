#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libnet.h>
#include <linux/if_ether.h>



int main() 
{
    libnet_t *handle; /* Libnet句柄 */

    int packet_size; /* 构造的数据包大小 */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, udp_tag, dns_tag; /* 各层build函数返回值 */
    struct ethhdr *macaddr = NULL;       /*获取mac地址*/
    u_short proto = IPPROTO_UDP; /* 传输层协议 */

    char *device = "eth0"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char *src_ip_str = "192.168.91.44"; /* 源IP地址字符串 */
    char *dst_ip_str = "192.168.25.23"; /* 目的IP地址字符串 */
    int dst_port = 22;
    int dns_port = 53;
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */

    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */

    char query[] = "www.baidu.com";

    do{
        /* 初始化Libnet */
        if ( NULL == (handle = libnet_init(LIBNET_RAW4, device, error)) ) {
            fprintf(stderr, "libnet_init failure\n");
            break;
        };

        /* 把目的IP地址字符串转化成网络序 */
        dst_ip = libnet_name2addr4(handle, dst_ip_str, LIBNET_RESOLVE);
        if( -1 == dst_ip )
        {
            fprintf(stderr, "dst ip %s to network format failed : %s\n", dst_ip_str, libnet_geterror(handle));
            break;
        }
        /* 把源IP地址字符串转化成网络序 */
        src_ip = libnet_name2addr4(handle, src_ip_str, LIBNET_RESOLVE);
        if( -1 == src_ip )
        {
            fprintf(stderr, "src ip %s to network format failed : %s\n", src_ip_str, libnet_geterror(handle));
            break;
        }

        /* 
         * build dns payload 
         */
        payload_s = snprintf(payload, sizeof payload, "%cwww%cbaidu%ccom%c%c%c%c%c", 
                0x03, 0x05, 0x03, 0x00, 0x00, 0x01, 0x00, 0x01);

        /* 
         * build packet
         */
        dns_tag = libnet_build_dnsv4(
                LIBNET_UDP_DNSV4_H,          /* TCP or UDP */
                0x7777,        /* id */
                0x0100,        /* request */
                1,             /* num_q */
                0,             /* num_anws_rr */
                0,             /* num_auth_rr */
                0,             /* num_addi_rr */
                payload,
                payload_s,
                handle,
                0
                );
        if ( -1 == dns_tag )
        {
            fprintf(stderr, "Can't build  DNS packet: %s\n", libnet_geterror(handle));
            break;
        }

        udp_tag = libnet_build_udp(
                0x6666, /* 源端口 */
                dns_port, /* 目的端口 */
                LIBNET_UDP_H + LIBNET_UDP_DNSV4_H + payload_s, /* 长度 */
                0, /* 校验和,0为libnet自动计算 */
                NULL, /* 负载内容 */
                0, /* 负载内容长度 */
                handle, /* libnet句柄 */
                0 /* 新建包 */
                );
        if ( -1 == udp_tag ) {
            fprintf(stderr, "libnet_build_tcp failure : %s\n", libnet_geterror(handle));
            break;
        };

        /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
        ip_tag = libnet_build_ipv4(
                LIBNET_IPV4_H + LIBNET_UDP_H + LIBNET_UDP_DNSV4_H + payload_s, /* IP协议块的总长,*/
                0, /* tos */
                242, /* id,随机产生0~65535 */
                //(u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
                0, /* frag 片偏移 */
                64, /* ttl,随机产生0~255 */
                //(u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
                IPPROTO_UDP, /* 上层协议 */
                0, /* 校验和，此时为0，表示由Libnet自动计算 */
                src_ip, /* 源IP地址,网络序 */
                dst_ip, /* 目标IP地址,网络序 */
                NULL, /* 负载内容或为NULL */
                0, /* 负载内容的大小*/
                handle, /* Libnet句柄 */
                0 /* 协议块标记可修改或创建,0表示构造一个新的*/
                );
        if ( -1 == ip_tag ) {
            fprintf(stderr, "libnet_build_ipv4 failure : %s\n", libnet_geterror(handle));
            break;
        };

        packet_size = libnet_write(handle); /* 发送已经构造的数据包*/

    }while(0);
    if( handle )
        libnet_destroy(handle); /* 释放句柄 */
    puts("End");

    return (0);
}
