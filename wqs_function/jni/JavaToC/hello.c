#include <stdio.h>
#include <string.h>
#include "jni.h"

JNIEXPORT jint JNICALL Java_JavaToC_helloworld(void)
{
    printf("hello world\n");
    return 0;
}
