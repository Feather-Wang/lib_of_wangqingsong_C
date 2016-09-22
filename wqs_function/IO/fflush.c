/*
 *fflush():立即刷新指定缓冲区
 * */
#include <stdio.h>

int main()
{
    printf("a");
    fflush(stdout);

    while(1);

    return 0;
}
