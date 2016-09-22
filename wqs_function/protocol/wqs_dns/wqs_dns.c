#include "wqs_dns.h"

static uint16_t dns_id = 0x1000;

#define reversal_2(x) ({\
        typeof(x) _char1 = ((x >> 8) & 0x00ff);\
        typeof(x) _char2 = (x & 0x00ff);\
        _char2 << 8 | _char1;\
        })

#define reversal_4(x) ({\
        typeof(x) _char1 = ((x >> 24) & 0x00ff);\
        typeof(x) _char2 = ((x >> 16) & 0x00ff);\
        typeof(x) _char3 = ((x >> 8) & 0x00ff);\
        typeof(x) _char4 = (x & 0x00ff);\
        _char4 << 24 | _char3 << 16 | _char2 << 8 | _char1;\
        })

static int i2string(uint8_t *string, uint32_t addr_ip)
{
    int ret = -1;

    sprintf(string, "%d.%d.%d.%d", 
            (addr_ip >> 24) & 0x000000ff, (addr_ip >> 16) & 0x000000ff, (addr_ip >> 8) & 0x000000ff, addr_ip & 0x000000ff);

    return ret;
}

static int dns_solve_dnshead(uint8_t *buf, struct dns_res *head)
{
    int ret = -1;
    int len = 0;
    
    memcpy(&(head->id), buf+len, 2);
    head->id = reversal_2(head->id);

    len += 2;
    memcpy(&(head->flags), buf+len, 2);
    head->flags = reversal_2(head->flags);

    len += 2;
    memcpy(&(head->que), buf+len, 2);
    head->que = reversal_2(head->que);

    len += 2;
    memcpy(&(head->ans), buf+len, 2);
    head->ans = reversal_2(head->ans);
    
    len += 2;
    memcpy(&(head->ant), buf+len, 2);
    head->ant = reversal_2(head->ant);
    
    len += 2;
    memcpy(&(head->add), buf+len, 2);
    head->add = reversal_2(head->add);
    
    printf("head info : id = %X, flags = %X, que = %X, ans = %X, ant = %X, add = %X\n",
            head->id, head->flags, head->que, head->ans, head->ant, head->add
            );
    
    ret = 0;
    
    return ret;
}

/*打印返回值错误信息*/
char *wqs_dns_strerror(int error_code)
{
	static char strerror[1024] = {0};
	switch(error_code)
	{
	case -1:
	snprintf(strerror, 1024, "resolve is error");
	break;
		case 1:
		snprintf(strerror, 1024, "format is wrong");
	break;
		case 2:
		snprintf(strerror, 1024, "the error is in server");
	break;
		case 3:
		snprintf(strerror, 1024, "the url is not exist");
	break;
		case 4:
		snprintf(strerror, 1024, "the type of request is not support");
	break;
		default:
		snprintf(strerror, 1024, "this error code is no mineing");
	break;
	}
	return strerror;
}

/* 用于将正常的域名，转换成dns服务器能够识别的域名格式
 * 返回值：为转换后的域名的长度，不能依靠strlen来获取转换后的域名长度
 * */
static int dns_build_url(const uint8_t *old_url, int old_url_len, uint8_t *new_url)
{
    int new_url_len = 0;
    uint8_t tmp[512] = {0};
    uint8_t *pstr = NULL;
    uint8_t *argv[5];
    int count = 0;
    int index = 0;

    sprintf(tmp, "%s", old_url);
    argv[index] = strtok(tmp, ".");
    sprintf(&(new_url[new_url_len]), "%c%s", strlen(argv[index]), argv[index]);
    new_url_len += strlen(argv[index])+1;

    //printf("[%s:%d] argv[%d] = [%d][%s]\n", __FUNCTION__, __LINE__, index, strlen(argv[index]), argv[index]);
    while( (argv[++index] = strtok(NULL, ".") ) )
    {
        sprintf(&(new_url[new_url_len]), "%c%s", strlen(argv[index]), argv[index]);
        new_url_len += strlen(argv[index])+1;
        //printf("[%s:%d] argv[%d] = [%d][%s]\n", __FUNCTION__, __LINE__, index, strlen(argv[index]), argv[index]);
    }
    sprintf(&(new_url[new_url_len]), "%c", 0x00);
    new_url_len += 1;

    //printf("[%s:%d] index = %d\n", __FUNCTION__, __LINE__, index);

    return new_url_len;
}

int wqs_build_dns(uint8_t *build_buf, int id, int flags, int num_q, uint8_t *url){

    struct dns_req req;
    int len = 0; 

    memset(&req,0,sizeof(req));

    req.id=htons(id);
    req.flags=htons(flags);
    req.que=htons(num_q);
    req.ans=0;
    req.ant=0;
    req.add=0;

    req.url_len = dns_build_url(url, strlen(url), req.url);
    req.type=htons(1);
    req.class=htons(1);

    memcpy(build_buf,&req.id,sizeof(req.id));
    len=sizeof(req.id);

    memcpy(build_buf+len,&req.flags,sizeof(req.flags));
    len+=sizeof(req.flags);

    memcpy(build_buf+len,&req.que,sizeof(req.que));
    len+=sizeof(req.que);

    memcpy(build_buf+len,&req.ans,sizeof(req.ans));
    len+=sizeof(req.ans);

    memcpy(build_buf+len,&req.ant,sizeof(req.ant));
    len+=sizeof(req.ant);

    memcpy(build_buf+len,&req.add,sizeof(req.add));
    len+=sizeof(req.add);

    memcpy(build_buf+len,req.url,req.url_len);
    len+=req.url_len;

    memcpy(build_buf+len,&req.type,sizeof(req.type));
    len+=sizeof(req.type);

    memcpy(build_buf+len,&req.class,sizeof(req.class));
    len+=sizeof(req.class);

    return len;
}

int dns_build(uint8_t *build_buf, uint8_t *url){

    int len=0;

    len = wqs_build_dns(build_buf, dns_id, 0x100, 1, url);

    return len;
}

int wqs_solve_dns(uint8_t *buf, int len, uint8_t *ip_list){
    int ret = -1;
    int res_number = 0;
    uint8_t ip_string[24] = {0};
    int iplist_len = 0;

    struct dns_res res_net;
    memset(&res_net, 0x00, sizeof(struct dns_res));

    uint8_t *pstr = NULL;   

    dns_solve_dnshead(buf, &res_net);

    if( res_net.id != dns_id )
    {
        fprintf(stderr, "the id of dns reply is not equal the request id, answer id = %X, request id = %X\n", res_net.id, dns_id);
        return ret;
    }
	/*判断返回码是否为0, 0为无差错*/
	if( 0 != (res_net.flags & 0xF) )
	{
		return (res_net.flags & 0xF);
	}
    pstr = buf + sizeof(struct dns_res);

    /************ 处理应答信息中的请求部分 Start ************/
    // 处理查询url
    uint8_t tmp_ip_size = 0;
    while(1)
    {
        tmp_ip_size = 0;
        memcpy(&tmp_ip_size, pstr, 1);

        if( 0 == tmp_ip_size )
        {
            pstr += 1;
            break;
        }
        pstr += 1 + tmp_ip_size;
    }
    pstr += 4; //这次添加，是为了过滤掉跟在url后边的查询类型和查询类
    /************ 处理应答信息中的请求部分 End ************/

    res_number = res_net.ans;
    do{
        printf("res_number = %d\n", res_number);

        /* 判断回答中是不是使用了偏移指针指向请求中的域名 */
        if( 0xc0 == ( *pstr & 0xc0 ) )
        {
            pstr += 2;
        }
        else
        {
            /* 当回答中的域名没有使用偏移指针时，在这里处理 */
        }

        /* 判断回答类型是否为IPv4地址 */
        uint16_t tmp_type = 0;
        memcpy(&tmp_type, pstr, 2);
        tmp_type = reversal_2(tmp_type);
        //printf("answer_type = %X\n", tmp_type);
        if( 0x0001 == tmp_type )
        {
            /* 获取IP地址长度 */
            pstr += 8;
            uint16_t addr_len = 0;
            memcpy( &addr_len, pstr, 2 );
            addr_len = reversal_2(addr_len);

            /* 获取IP地址 */
            pstr += 2;
            uint32_t addr_ip = 0;
            memcpy( &addr_ip, pstr, addr_len );
            pstr += addr_len;

            addr_ip = reversal_4(addr_ip);

            /* 将得到的16进制的IP地址转化成字符串 */
            memset(ip_string, 0x00, sizeof(ip_string));
            i2string(ip_string, addr_ip);
            //printf("ip_string = %s, addr_ip = %X\n", ip_string, addr_ip);
            
            if( 0 == iplist_len )
            {
                sprintf(ip_list+iplist_len, "%s", ip_string);
                iplist_len += strlen(ip_string);
            }
            else
            {
                sprintf(ip_list+iplist_len, ",%s", ip_string);
                iplist_len += strlen(ip_string) + 1;
            }
        }
        else
        {
            /* 获取内容长度 */
            pstr += 8;
            uint16_t addr_len = 0;
            memcpy( &addr_len, pstr, 2 );
            addr_len = reversal_2(addr_len);

            //printf("addr_len = %x\n", addr_len);
            /* 过滤掉该段内容 */
            pstr += addr_len + 2;
        }

        --res_number;

        if( 0 >= res_number )
            ret = 0;

    }while( 0 < res_number );

    return ret;
}

int dns_solve(uint8_t *buf, int len, uint8_t *ip_list){
    int ret = -1;

    ret = wqs_solve_dns(buf, len, ip_list);

    ++dns_id;

    return ret;
}
