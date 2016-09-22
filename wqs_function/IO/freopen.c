// freopen可以输出重定向，主要用于重定向stdout，stdin和stderr

#include <stdio.h>
#include <string.h>
#include <errno.h>
int main(int argc, const char *argv[])
{
    // 重定向 stdout
    if( NULL == freopen("1.txt", "w", stdout) )
    {
        printf("freopen failed");
        return -1;
    }
    
    printf("after freopen\n");
    puts("Hello World");

    if( NULL == freopen("/dev/tty", "w", stdout) )
    {
        printf("freopen failed");
        return -1;
    }
    printf("/dev/tty\n");
//################################################################################
    // 重定向 stdin
    if( NULL == freopen(argv[1], "r", stdin) )
    {
        printf("freopen failed");
        return -1;
    }

    int wang = 0;
    scanf("%d", &wang);
    printf("wang = %d\n", wang);

    if( NULL == freopen("/dev/tty", "r", stdin) )
    {
        printf("freopen failed");
        return -1;
    }

    return 0;
}
