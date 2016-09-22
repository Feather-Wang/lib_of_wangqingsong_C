#include <stdio.h>

int judgement()
{
    union 
    {
        char ch;
        int a;
    } value;
    value.a = 1;
    return (value.ch == 1);
}

int main(int argc, const char *argv[])
{
    printf("judgement = %d\n", judgement()); 
    return 0;
}
