#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char buf[1024] = {0};

    /* 获取当前路径 */
    memset(buf, 0x00, sizeof(buf));
    getcwd(buf, 1024);
    printf("getcwd() --> %s\n", buf);

    return 0;
}
