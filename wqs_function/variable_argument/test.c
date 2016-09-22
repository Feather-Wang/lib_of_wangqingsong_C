#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

void wqs_printf( char *format, ...)
{
    printf("format = %s\n", format);
    va_list arg_ptr;
    va_start(arg_ptr, format);
    //j = va_arg(arg_ptr, char*);
    vfprintf(stderr, format, arg_ptr);
    fflush(stderr);
    va_end( arg_ptr );
}


void wqs_printf_2( char *format, ...)
{
    printf("format 2 = %s\n", format);
    va_list arg_ptr;
    va_start(arg_ptr, format);
    int index = 0;
    for(index = 0; index < 3; index++)
    {
        char *pstr = NULL;
        pstr = va_arg(arg_ptr, char*);
        printf("%s\n", pstr);
    }
    va_end( arg_ptr );
}

int main(int argc, const char *argv[])
{
    wqs_printf("1:%s,2:%s,3:%s\n", "first", "wang", "qing");
    wqs_printf_2("1:%s,2:%s,3:%s\n", "first", "wang", "qing");
    
    return 0;
}
