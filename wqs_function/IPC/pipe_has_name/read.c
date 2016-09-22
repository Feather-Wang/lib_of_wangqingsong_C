#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>

// 执行：./r wang
int main(int argc, const char *argv[])
{
    int fd;

    // 如果管道文件存在，就打开，不存在，就创建
    if( 0 != access( argv[1], F_OK) )
    {
        // 创建管道文件，参数1：管道文件名，参数2：管道文件的权限
        if( 0 > mkfifo( argv[1], 0777) )
        {            
            printf("createnamed pipe failed.\n");
        }
    }
    
    // 打开管道文件，参数1：管道文件名，参数2：打开的权限设置
    fd = open( argv[1],O_RDONLY|O_NONBLOCK);        //     非阻塞方式打开，只读

    char read_buf[128] = {0};
    while(1)
    {
        memset(read_buf, 0x00, 128);
        read(fd,read_buf,128);
        printf("child read : [%d] %s\n", strlen(read_buf), read_buf);
        sleep(1);
    }
    close(fd);

    return 0;
}
