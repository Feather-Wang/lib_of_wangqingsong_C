#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    int ret = -1;
    int channel[2] = {0};
    char buf[1024] = {0};

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, channel);
    if( 0 > ret ) 
    {
        fprintf(stderr, "socketpair error, ret=[%d], errno=[%d]\n", ret, errno);
    }

    int nb = 1;
    ioctl(channel[0], FIONBIO, &nb);
    ioctl(channel[1], FIONBIO, &nb);

    int on = 1;
    ioctl(channel[0], FIOASYNC, &on);

    /*设置SIGIO或SIGURG信号的进程ID或进程组ID*/
    fcntl(channel[0], F_SETOWN, getpid());

    int flags = 0;
    flags = fcntl(channel[0], F_GETFD);
    flags |= FD_CLOEXEC;
    fcntl(channel[0], F_SETFD, flags);
    flags = fcntl(channel[1], F_GETFD);
    flags |= FD_CLOEXEC;
    fcntl(channel[1], F_SETFD, flags);

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "hello world");
    ret = send(channel[0], buf, strlen(buf), 0);
    printf("send ret=[%d], buf=[%s]\n", ret, buf);

    memset(buf, 0x00, sizeof(buf));
    ret = recv(channel[1], buf, sizeof(buf), 0);
    printf("recv ret=[%d], buf=[%s]\n", ret, buf);

    return 0;
}
