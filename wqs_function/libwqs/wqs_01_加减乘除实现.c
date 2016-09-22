#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void binary(int a)
{
    char buf[33] = {0};
    int temp = 0x1;
    int i = 0;
    for(i = 0; i < 32; i++) 
    {
        if( a & temp ) 
        {
            buf[31-i] = '1';
        }
        else 
        {
            buf[31-i] = '0';
        }
        temp <<= 1;
    }
    printf("%d --> %s\n", a, buf);
}

int add_r(int a, int b)
{
    int sum = a ^ b;
    int temp = (a & b) << 1;
    int new_num = 0;
    
    while( 0 != temp )
    {
        new_num = temp;
        temp = (sum & temp) << 1;
        sum = sum ^ new_num;
    }
    return sum;
}

int sub_r(int a, int b)
{
    return add_r(a, add_r(~b, 1));
}

int mul_r(int a, int b)
{
    int i = 0;
    int mul_num = 0;
    for(i = 0; i < b; i++) 
    {
        mul_num = add_r(mul_num, a);
    }

    return mul_num;
}

int div_r(int a, int b)
{
    int div_num = 0;
    int i = 0;
    while( a >= b ) 
    {
        div_num++;
        a = sub_r(a, b);
    }
    return div_num;
}

int main(int argc, const int *argv[])
{
    int a = 175;
    int b = 23;
    printf("main --> %d + %d = %d\n", a, b, add_r(a, b));
    printf("main --> %d - %d = %d\n", a, b, sub_r(a, b));
    printf("main --> %d * %d = %d\n", a, b, mul_r(a, b));
    printf("main --> %d / %d = %d\n", a, b, div_r(a, b));

    return 0;
}
