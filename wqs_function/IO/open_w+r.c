#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define N 64

int main(int argc, char *argv[])
{
    int fdr, fdw;
    char buf[N] = {0};
    ssize_t n;

    if (argc < 3)
    {
        printf("usage:%s srcfile destfile\n", argv[0]);
        return 0;
    }

    if ((fdr = open(argv[1], O_RDONLY)) == -1)
    {
        perror("open for reading");
        return -1;
    }

    if ((fdw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror("open for writing");
        return -1;
    }

    while ((n = read(fdr, buf, N)) > 0)
        write(fdw, buf, n);

    // 会将fdw文件的大小扩大到100的大小，如果原来有10大小的数据，剩下的90会用'\0'填充，如果大于100，会把大于的部分去掉
    if (-1 == ftruncate(fd, 100))
    {
        perror("ftruncate");
        return -1;
    }   
    close(fdr);
    close(fdw);

    return 0;
}
