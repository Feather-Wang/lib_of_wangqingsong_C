#include "wqs_inotify.h"

/*
 * 注：当监测目标为目录时，当该目录下的文件发生变化时，inotify_event中的name和len时有值的，但是如果监测的是一个文件，则没有值
 */
/*
 ******* 文件事件使用该结构表示 *********
 * struct inotify_event
 * {
 *      __s32 wd;        //watch descriptor
 *      __u32 mask;      //watch mask
 *      __u32 cookie;    //cookie to synchronize two events
 *      __u32 len;       //length (including nulls) of name
 *      char  name[0];   //stub for possible name
 * }
 *
 ******* MASK **********************
 * 适用于inotify_add_watch与read返回的inotify_event中的mask
 * IN_ACCESS                文件被访问
 * IN_ATTRIB                文件属性发生变化
 * IN_CLOSE_WRITE           以write方式打开文件并关闭
 * IN_CLOSE_NOWRITE         以非write方式打开文件并关闭
 * IN_CREATE                文件或目录被创建
 * IN_DELETE                文件或目录被删除(被监测的文件夹A中B文件被删除）
 * IN_DELETE_SELF           被检测的文件或目录被删除(被检测的文件夹A被删除)
 * IN_MODIFY                文件被修改
 * IN_MOVE_SELF             被检测的文件或目录移动
 * IN_MOVED_FROM            文件移出被检测的目录
 * IN_MOVED_TO              文件移入被检测的目录
 * IN_OPEN                  文件被打开
 
 * 上述flag的集合           
 * IN_ALL_EVENTS            以上所有flag的集合
 * IN_MOVE                  IN_MOVED_TO|IN_MOVED_FROM
 * IN_CLOSE                 IN_CLOSE_WRITE|IN_CLOSE_NOWRITE
 *
 * 不常用的flag
 * IN_DONT_FOLLOW           不follow符号链接(since 2.6.15）
 * IN_EXCL_UNLINK
 * 当文件从监测目录中unlink后，则不再报告该文件的相关event，比如监控/tmp使用（since
 * 2.6.36
 * IN_MASK_ADD              追打MASK到被监测的pathname
 * IN_ONESHOT               只监测一次
 * IN_ONLYDIR               只监测目录
 * 
 * 仅由read返回
 * IN_IGNORED               inotify_rm_watch，文件被删除或者文件系统被umount
 * IN_ISDIR                 发生事件的是一个目录
 * IN_Q_OVERFLOW            event队列溢出
 * IN_UNMOUNT               文件系统unmount
 */

#define MAX_SIZE 1024

int main(int argc, char *argv[])
{
    int fd = 0;
    int wd = 0;
    struct inotify_event *event = NULL;

    char file[] = "/root/wang/";

    //初始化实例
    fd = inotify_init();
    if( 0 > fd )
    {
        puts("init error");
        return 1;
    }

    //添加监控的文件，wd为watch描述符
    wd = inotify_add_watch(fd, file, IN_ALL_EVENTS);

    fd_set wfs;
    struct timeval tv;
    int retval = 0;
    int len = 0;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    char buf[MAX_SIZE] = {0};
    while(1)
    {
        memset(buf, 0x00, MAX_SIZE); 
        FD_ZERO(&wfs);
        FD_SET(fd, &wfs);
        tv.tv_sec = 3;
        tv.tv_usec = 0;
        retval = select(fd+1, &wfs, NULL, NULL, &tv);
        if( 0 >= retval )
        {
            puts("select error");
            sleep(1);
            continue;
        }

        if( FD_ISSET(fd, &wfs) )
        {
            len = read(fd, buf, MAX_SIZE);
            if( -1 == len )
            {
                puts("read error");
                continue;
            }
            
            event = (struct inotify_event *)buf;
            if( wd == event->wd )
                puts("wd == event->wd");

            if( IN_CLOSE_WRITE == event->mask )
                puts("IN_CLOSE_WRITE");
            if( IN_MODIFY == event->mask )
                puts("IN_MODIFY");
            if( IN_ACCESS == event->mask )
                puts("IN_ACCESS");
            if( IN_ATTRIB == event->mask )
                puts("IN_ATTRIB");
            
            char name[1024] = {0};
            strcpy(name, event->name);
            printf("name = %s\n", name);
            printf("event --> mask = %x\n", event->mask);
            printf("event --> len = %d\n", event->len);
            printf("event --> name = %s\n", event->name);
            printf("event --> name = %p\n", event->name);
            printf("len = %d, event size = %d\n", len, sizeof(struct inotify_event));
        }
    }
    

    //删除监控的文件
    int ret = inotify_rm_watch(fd, wd);

    close(fd);
    return 0;
}
