#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    int ret = -1;

    ret = access("./1.txt", F_OK);
    printf("ret = %d, errno = %s\n", ret, strerror(errno));
    printf("errno == ENOENT ? %s\n", errno == ENOENT ? "Yes" : "No");
    return 0;
}
