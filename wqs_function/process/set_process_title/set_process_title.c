#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define NGX_SETPROCTITLE_PAD       '\0'

typedef int                 ngx_int_t;
typedef unsigned int        ngx_uint_t;

extern char **environ;

static char *ngx_os_argv_last;

int              ngx_argc;
char           **ngx_os_argv;

    u_char *
ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}

/*该函数会计算环境变量的总的大小，然后申请一块内存，然后逐一将环境变量拷贝到这块内存中，再将environ中的环境变量的指针指向这块内存中的具体地址*/
    ngx_int_t
ngx_init_setproctitle()
{
    u_char      *p;
    size_t       size;
    ngx_uint_t   i;

    size = 0;

     /*计算环境变量所用的总的空间的大小，然后以申请足够的空间用于存放环境变量*/
    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    /*为环境分配空间*/
    p = malloc(size);
    if (p == NULL) {
        return NGX_ERROR;
    }

    /*下面开始计算前面用于存放环境变量的最后位置*/
    ngx_os_argv_last = ngx_os_argv[0];

    /*首先计算参数的最后位置*/
    for (i = 0; ngx_os_argv[i]; i++) {
        if (ngx_os_argv_last == ngx_os_argv[i]) {
            ngx_os_argv_last = ngx_os_argv[i] + strlen(ngx_os_argv[i]) + 1;
        }
    }

    /*再计算环境变量占用的最后位置，并完成环境变量的拷贝*/
    for (i = 0; environ[i]; i++) {
        if (ngx_os_argv_last == environ[i]) {

            size = strlen(environ[i]) + 1;
            ngx_os_argv_last = environ[i] + size;

            ngx_cpystrn(p, (u_char *) environ[i], size);
            environ[i] = (char *) p;
            p += size;
        }
    }

    /*该步自减，将会让ngx_os_argv_last指向原environ的最后一个'\0'的位置*/
    /*这步是为了在后面设置进程标题是，如果设置的标题小于参数+环境变量这块连续空间的大小，要把后面没用到的空间初始化，避免留下无用的信息*/
    ngx_os_argv_last--;

    return NGX_OK;
}

/*设置进程标题*/
    void
ngx_setproctitle(char *title)
{
    u_char     *p;

    /*不知道为什么要把argv[1]设置为NULL*/
    ngx_os_argv[1] = NULL;

    /*将"nginx:"拷贝到argv[0]里，在ngx_cpystrn内部会在拷贝到"nginx:"结尾时跳出来*/
    p = ngx_cpystrn((u_char *) ngx_os_argv[0], (u_char *) "nginx: ",
            ngx_os_argv_last - ngx_os_argv[0]);

    /*将之前组织的title也拷贝到argv[0]中*/
    p = ngx_cpystrn(p, (u_char *) title, ngx_os_argv_last - (char *) p);

    /*如果原参数列表+环境变量的连续空间没有用完，则将没用到的部分进行初始化*/
    if (ngx_os_argv_last - (char *) p) {
        memset(p, NGX_SETPROCTITLE_PAD, ngx_os_argv_last - (char *) p);
    }

}

    static ngx_int_t
ngx_save_argv(int argc, char *const *argv)
{
    ngx_argc = argc;
    ngx_os_argv = (char **) argv;

    return NGX_OK;
}

int main(int argc, char *const *argv)
{
    ngx_save_argv(argc,argv);
    ngx_init_setproctitle();

    ngx_setproctitle("xiejian test aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    while(1){
        printf("------------------------------------------------\n");
        system("ps -ef|awk '$8 ~ /nginx/ {print $0}'");
        sleep(10);
    }

    return (0);  
}
