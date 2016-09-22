#include <stdio.h>
#include <string.h>
#include <errno.h>

#define N 64

int main(int argc, char *argv[])
{
    FILE *fp;
    long pos;

    if (argc < 2)
    {
        fprintf(stdout, "usage:%s file\n", argv[0]);
        return 0;
    }

    if ((fp = fopen(argv[1], "r+")) == NULL)
    {
        fprintf(stderr, "fopen:%s\n", strerror(errno));
        return -1;
    }

    if ((pos = ftell(fp)) == -1)
    {
        fprintf(stderr, "ftell:%s\n", strerror(errno));
        return -1;
    }

    printf("%ld\n", pos);

    int i = 0;
    i = fseek(fp, 2, SEEK_SET);
    printf("i = %d\n", i);

    char buf[1024] = {0};
    gets(buf);
    printf("i 2 = %s\n", buf);

    fclose(fp);

    return 0;
}
