#include "wqs_system_func_rewrite.h"

#define BUF_SIZE 1024

int judgment(const char *name) // 程序状态判断函数, name为判断的进程名
{
    FILE *fp;
    int i = 0;
    char str[1024] = {0};
    char *pch;
    char buf[128] = {0};
    int flag = 0;

    sprintf(buf,"pidof %s",name);

    fp = popen(buf,"r");  // 执行pidof name命令，并将输出结果重定向到fp。
    if(fp == NULL)
    {
        fprintf(stderr,"fopen :");
        flag = 0;
    }
    else
    {
        memset(str,0,1024);

        while( (fgets(str,1024,fp)) != NULL) // 将pidof命令得到的内容读取到str字符数组中
        {
            if(strstr(str,"\n") == NULL)
            {
                printf("fgets end!!");
                break;
            }
        }

        if(strlen(str) > 1) // 判断str中是否有内容
        {
            flag = 1;           
        }
        else
        {
            flag = 0;        
        }
    }


    if( fp != NULL ) // 关闭文件标识符
    {
        pclose(fp);
    }

  //  printf("flag > %d\n",flag);
    return flag;
}

typedef void (*sighandler_t) (int);
int wqs_system_s(const char *cmd_line)
{
    int ret = 0;
    sighandler_t old_handler;

    old_handler = signal(SIGCHLD, SIG_DFL);
    ret = system(cmd_line);
    signal(SIGCHLD, old_handler);

    return ret;
}

// failure return -1, success return 0; and copy failure or success
// information to return_msg
int wqs_system_p(const char *cmd_line, char *return_msg)
{
    char str[BUF_SIZE];
    FILE *fp = NULL;
    int return_flog = 0;
    int flog_pclose = 0;

    fp = popen(cmd_line, "r");
    if( NULL == fp)
    {
        return_flog = -1;
        sprintf(return_msg, "popen failure!");
    }
    else
    {
        memset(str, 0, BUF_SIZE);
        while( NULL != (fgets(str, BUF_SIZE, fp)) )
        {
            if( '\n' == str[strlen(str)-1] )
            {
                str[strlen(str)-1] = '\0';
            }
        }
        
        if( (-1 == (flog_pclose = pclose(fp))) || WIFEXITED(flog_pclose) )
        {
            return_flog = -1;
            sprintf(return_msg, "%s", str);
        }
        else
        {
            sprintf(return_msg, "OK!");
        }
    }

    return return_flog;
}

/*
 *  get a straight character from stream, copy to s, the character's max size is size value. cut the '\n' charactor.
 *  
 *  s : store character's memory pointer
 *  size : the s's max size.
 *  stream: the character's source , stream is a file pointer.
 * */

char *wqs_fgets(char *s, int size, FILE *stream)
{
    if(s == NULL || stream == NULL || size <= 0)
        return NULL;

    if( fgets(s, size, stream) == NULL )
        return NULL;
    
    if( s[strlen(s)] == '\0' && s[strlen(s)-1] == '\n' )
        s[strlen(s)-1] = '\0';

    return s;
}

/*
 * dst will replace src in the ut_buf.
 *
 * ut_buf : a straight character's pointer
 * len : ths straight character's size
 * src: old straight character's pointer
 * dst: new straight character's pointer
 * count_times: how many time is the dst replace src. 1 or -1, -1 means all
 *              replace.
 * */
int ReplaceIp(char **ut_buf, unsigned int *len, char *src, char *dst, int count_times)
{
    int slen = 0,
        dlen = 0,
        diffLen = 0,
        addLen = 0;
    char *ps = NULL,
         *pe = NULL;

    if( NULL == ut_buf || NULL == *ut_buf || NULL == src || NULL == dst || *len <= 0 )
        return -1;

    slen = strlen(src);
    dlen = strlen(dst);
    diffLen = dlen - slen;
    
    if( diffLen > 0 )
    {
        //if( (ps = memmem(*ut_buf, *len, src, strlen(src))) != NULL )
        if( (ps = strstr(*ut_buf, src)) )
        {
            while( NULL != ps )
            {
                addLen += diffLen;
                ps += slen;
                ps = strstr(ps, src);
            }

            *ut_buf = (char *)realloc(*ut_buf, *len+addLen+1);
            if( NULL == *ut_buf )
                return -1;
        }     
    }
    
    //ps = memmem(*ut_buf, *len, src, strlen(src));
    ps = strstr(*ut_buf, src);
    pe = *ut_buf + *len;

    while( NULL != ps )
    {
        memmove(ps+dlen, ps+slen, pe-ps-slen);
        memcpy(ps, dst, dlen);

        *len += diffLen;
        pe += diffLen;
        ps = strstr(ps, src);
        //ps = memmem(ps, pe-ps, src, strlen(src));

        if( count_times == 1 )
            break;
    }

    return 0;
}
