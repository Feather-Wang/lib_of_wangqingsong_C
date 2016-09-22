#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// 执行：./w wang
int main(int argc, const char *argv[])
{
    int fd;

    if( 2 > argc )
    {
        printf("pleaseinput the write data.\n");
    }

    // 打开管道，参数1：管道文件名，参数2：打开权限
    fd = open(argv[1],O_WRONLY|O_NONBLOCK);  //非阻塞方式

    // 向管道中写数据
    write(fd,argv[1], 100);                 //将argv[1]写道fd里面去

    // 关闭管道
    close(fd);

    return 0;
}
