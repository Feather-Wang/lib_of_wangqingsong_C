#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/ip_icmp.h>

# define ODDBYTE(v)	(v)

/*make checksum code*/
unsigned short in_cksum(const u_short *addr, register int len, u_short csum)
{
	register int nleft = len;
	const u_short *w = addr;
	register u_short answer;
	register int sum = csum;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1)
		sum += ODDBYTE(*(u_char *)w); /* le16toh() may be unavailable on old systems */

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

void sock_addr_bind(struct sockaddr_in *whereto, char *ip)
{
    whereto->sin_family = AF_INET;
    inet_aton(ip, &whereto->sin_addr);
}

/*build icmp-ping packet*/
int icmp_build( unsigned char *buf, int *buf_len, unsigned short icp_flag, unsigned short icp_id )
{
    struct icmphdr *icp;
    *buf_len = sizeof(struct icmphdr);

    icp = (struct icmphdr *)buf;
    icp->type = ICMP_ECHO;
    icp->code = 0;
    icp->checksum = 0;
    icp->un.echo.sequence = htons(icp_flag);
    icp->un.echo.id = htons(icp_id);

    icp->checksum = in_cksum((unsigned short*)icp, sizeof(struct icmphdr), 0);

    return 0;
}
/*resolve icmp reply packet*/
int icmp_resolv( unsigned char *buf, int buf_len, struct sockaddr_in *whereto, struct sockaddr_in *wherefrom, unsigned short icp_flag, unsigned short icp_id)
{
    int ret = -1;
    struct iphdr *ip = (struct iphdr *)buf;
    int hlen = 0;
    do {
        hlen = ip->ihl * 4;
        if( buf_len < hlen + 8 || ip->ihl < 5 )
        {
            fprintf(stderr, "icmp-ping: this packet too short (%d bytes) from %s\n", buf_len, inet_ntoa(wherefrom->sin_addr));
            break;
        }

        if( whereto->sin_addr.s_addr != wherefrom->sin_addr.s_addr )
        {
            fprintf(stderr, "dst ip address is not same\n");
            break;
        }

        struct icmphdr *icp = (struct icmphdr *)(buf + hlen);
        if( ICMP_ECHOREPLY == ntohs(icp->type) )
        {
            if( ntohs(icp->un.echo.id) != icp_id || ntohs(icp->un.echo.sequence) != icp_flag ) 
            {
                fprintf(stderr, "reply flag != send flag or reply id != send id\n");
                break;
            }
        }
        else
        {
            switch(icp->type)
            {
                case ICMP_ECHO:
                    return -1;
                case ICMP_DEST_UNREACH:/*3.dst address is unreachable*/
                case ICMP_SOURCE_QUENCH:/*4.source is closed*/
                case ICMP_REDIRECT:/*5.*/
                case ICMP_TIME_EXCEEDED:/*11.timeout*/
                    /*error process*/
                    break;
                default:
                    break;
            }
        }

        fprintf(stdout, "icmp reply success\n");

        ret = 0;

    } while ( 0 );

    return ret;
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in whereto;
    memset(&whereto, 0x00, sizeof(whereto));
    unsigned char buf[1024] = {0};
    int buf_len = 0;
    int icmp_sock = 0;
    unsigned short icp_flag = getpid();
    unsigned short icp_id = 0;
    
    do{
        icmp_sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
        if( -1 == icmp_sock )
        {
            fprintf(stderr, "socket error\n");
            break;
        }
    
        sock_addr_bind(&whereto, "192.168.91.63");

        icmp_build( buf, &buf_len, icp_flag, icp_id);

        int len = sendto(icmp_sock, buf, buf_len, 0, (struct sockaddr*)&whereto, sizeof(struct sockaddr_in));
        printf("sendmsg --> len = %d\n", len);

        memset(buf, 0x00, sizeof(buf));
        struct sockaddr_in wherefrom;
        socklen_t wherefrom_len;
        memset(&wherefrom, 0x00, sizeof(wherefrom));
        len = recvfrom(icmp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&wherefrom, &wherefrom_len);

        if( -1 == icmp_resolv(buf, len, &whereto, &wherefrom, icp_flag, icp_id) )
        {
            fprintf(stderr, "icmp_resolv is error\n");
            break;
        }
        printf("wherefrom_len = %d, len = %d\n", wherefrom_len, len);
        int i = 0;
        for (i = 0; i < len; i++) 
        {
            printf("i = %x\n", buf[i]);
        }

    }while(0);

    return 0;
}
