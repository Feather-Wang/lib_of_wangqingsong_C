#define BUF_SIZE 1024
/*
 *功能：创建TCP服务器端，成功返回0，失败返回-1
 *fd：指向服务器端的套接字
 *ip：服务器端绑定的ip
 *port：服务器端绑定的端口
 * listen_count：监听连接的个数
 * */
int create_tcp_server(int *fd, char *ip, int port, int listen_count)
{
    int ret = -1;
    struct sockaddr_in server_address;
    int server_len = 0;

    do{
        if( (*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
        {
            perror("create socket failed : ");
            break;
        }

        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(ip);
        server_address.sin_port = htons(port);
        server_len = sizeof(server_address);
        if( bind(*fd, (struct sockaddr *)&server_address, server_len) != 0) // bind socket
        {
            perror(" bind error : ");
            break;
        }
        if(listen(*fd, listen_count) != 0) // listen socket
        {
            perror("listen error : ");
            break;
        }

        ret = 0;

    }while(0);

    if( 0 != ret )
    {
        close(*fd);
    }

    return ret;
}

/* 
 * 功能:创建客户端连接
 * fd:客户端的套接字指针
 * ip:服务器端ip地址
 * port:服务器端端口号
 * */
int create_tcp_connect(int *fd, char *ip, int port)
{
    int ret = -1;
    struct sockaddr_in sock_addr;

    do{
        if( (*fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        {
            perror("socket error : ");
            break;
        }

        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr(ip);
        sock_addr.sin_port = htons(port);

        if( (connect(*fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr))) < 0)
        {
            perror("Connect error : ");
            break;
        }

        ret = 0;

    }while(0);
    
    if( 0 != ret )
    {
        close(*fd);
    }

    return 1;
}
