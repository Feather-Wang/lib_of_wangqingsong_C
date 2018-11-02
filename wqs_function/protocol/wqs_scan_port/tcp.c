#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/tcp.h>

# define ODDBYTE(v)	(v)

const char *sourceip;
const char *sourceport;
const char *destip;
const char *destport;

struct checkhdr
{
    u_int32_t sourceip;
    u_int32_t destip;
    u_int8_t zeros;
    u_int8_t protocol;
    u_int16_t tcp_len;
};

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

void sock_addr_bind(struct sockaddr_in *whereto, const char *ip)
{
    whereto->sin_family = AF_INET;
    inet_aton(ip, &whereto->sin_addr);
}

/*build icmp-ping packet*/
int tcp_build( unsigned char *buf, int *buf_len, unsigned short icp_id )
{
    struct checkhdr *check_head;
    struct tcphdr *tcp_head;

    check_head = (struct checkhdr *)buf;
    check_head->sourceip = inet_addr(sourceip);
    check_head->destip = inet_addr(destip);
    check_head->zeros = 0;
    check_head->protocol = IPPROTO_TCP;
    check_head->tcp_len = 0;

    tcp_head = (struct tcphdr *)(buf+sizeof(struct checkhdr));
#ifdef __FAVOR_BSD
    tcp_head->th_sport = htons(atoi(sourceport));
    tcp_head->th_dport = htons(atoi(destport));
    tcp_head->th_seq = htonl(0x1234);;
    tcp_head->th_ack = 0;
    tcp_head->th_off = 0x5;
    tcp_head->th_flags = 0x02;
    tcp_head->th_win = htons(0x0400);
    tcp_head->th_sum = 0;
    tcp_head->th_urp = 0;
    
    check_head->tcp_len = htons(tcp_head->th_off*4);

    tcp->th_sum = in_cksum((unsigned short*)buf, sizeof(struct checkhdr)+sizeof(struct tcphdr), 0);
    printf("tcp sum=[%x]\n", tcp->th_sum);
#else
    tcp_head->source = htons(atoi(sourceport));
    tcp_head->dest = htons(atoi(destport));
    tcp_head->seq = htonl(0x1234);;
    tcp_head->ack_seq = 0;
    tcp_head->doff = 0x5;
    tcp_head->res1 = 0;
    tcp_head->res2 = 0;
    tcp_head->urg = 0;
    tcp_head->ack = 0;
    tcp_head->psh = 0;
    tcp_head->rst = 0;
    tcp_head->syn = 0;
    tcp_head->fin = 1;
    tcp_head->window = htons(0x0400);
    tcp_head->check = 0;
    tcp_head->urg_ptr = 0;

    check_head->tcp_len = htons(tcp_head->doff*4);
    tcp_head->check = in_cksum((unsigned short*)buf, sizeof(struct checkhdr)+sizeof(struct tcphdr), 0);
#endif
    
    *buf_len = sizeof(struct tcphdr);

    return 0;
}
/*resolve tcp reply packet*/
int tcp_resolv( unsigned char *buf, int buf_len, struct sockaddr_in *whereto, struct sockaddr_in *wherefrom, unsigned short tcp_id)
{
    int ret = 0;

    struct tcphdr *tcp_head = NULL;

    tcp_head = (struct tcphdr *)(buf+20);

#ifdef __FAVOR_BSD
#else
    printf("source port = [%d]\n", ntohs(tcp_head->source));
    printf("tcp_head->dest=[%d]\n", ntohs(tcp_head->dest));
#endif
    return ret;
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in whereto;
    memset(&whereto, 0x00, sizeof(whereto));
    unsigned char buf[1024] = {0};
    int buf_len = 0;
    int tcp_sock = 0;
    unsigned short tcp_id = 0;
    
    do{
        if( 5 != argc )
        {
            printf("./a.out sourceip sourceport destip destport\n");
            return 1;
        }

        sourceip = argv[1];
        sourceport = argv[2];
        destip = argv[3];
        destport = argv[4];

        tcp_sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
        if( -1 == tcp_sock )
        {
            fprintf(stderr, "socket error\n");
            break;
        }
    
        sock_addr_bind(&whereto, destip);

        tcp_build( buf, &buf_len, tcp_id);
        int i = 0;
        for (i = sizeof(struct checkhdr); i < (sizeof(struct checkhdr)+sizeof(struct tcphdr)); i++) 
        {
            printf("%02x ", buf[i]);
        }
        printf("\n");

        int len = sendto(tcp_sock, buf+sizeof(struct checkhdr), buf_len, 0, (struct sockaddr*)&whereto, sizeof(struct sockaddr_in));
        printf("sendmsg --> len = %d\n", len);

        struct sockaddr_in wherefrom;
        socklen_t wherefrom_len;
        while( 1 )
        {
            memset(buf, 0x00, sizeof(buf));
            memset(&wherefrom, 0x00, sizeof(wherefrom));
            len = recvfrom(tcp_sock, buf, sizeof(buf), 0, (struct sockaddr *)&wherefrom, &wherefrom_len);
            printf("destip=[%s], destport=[%d]\n", inet_ntoa(wherefrom.sin_addr), ntohs(wherefrom.sin_port));

            if( -1 == tcp_resolv(buf, len, &whereto, &wherefrom, tcp_id) )
            {
                fprintf(stderr, "tcp_resolv is error\n");
                break;
            }
            printf("wherefrom_len = %d, len = %d\n", wherefrom_len, len);
            for (i = 20; i < len; i++) 
            {
                printf("%02x ", buf[i]);
            }
            printf("\n");
            sleep(1);
        }

    }while(0);

    return 0;
}
