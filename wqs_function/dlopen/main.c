#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./test.h"

// dlopen\dlsym\dlclose等函数的头文件
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    int a = 10;
    int b = 2;

    int i = 0;
    for(i = 0; i < argc; i++)
        printf("argv[%d]=%s\n", i, argv[i]);

    // 使用dlopen打开库文件，RTLD_LAZY是一种打开模式，
    // 表示对于动态库中的未定义的符号不执行解析，只对函数引用有效，对于变量引用总是立即解析
    void *dp = NULL;
    dp = dlopen("./libtest.so", RTLD_LAZY);
    if( NULL == dp )
    {
        puts(" libtest.so open failed");
        return -1;
    }

    int (*test)(int a, int b);
    test = NULL;
    printf("test ---> %p\n", test);

    // 通过dp，找到名字为argv[1]的函数名，函数类型为int (*)(int, int)
    test = (int (*)(int, int))dlsym(dp, argv[1]);

    printf("test ---> %p\n", test);

    printf(" result --> %d\n", test(a, b));

    // 关闭库
    dlclose(dp);

    return 0;
}
