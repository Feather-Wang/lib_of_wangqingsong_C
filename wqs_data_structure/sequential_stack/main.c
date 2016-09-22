// 顺序栈
#include <stdio.h>
#include <stdlib.h>
#include "seq_stack.h"

int main()
{
    int t;
    seqstack *st = seqstack_create(24);

    puts("111");
    t = 1;
    seqstack_push(st, &t);
    t = 2;
    seqstack_push(st, &t);
    t = 3;
    seqstack_push(st, &t);

    int *element = NULL;
    while (! seqstack_empty(st))
    {
        seqstack_pop(st, &element);
        printf("%d\n", *element);
        free(element);
    }

    seqstack_destroy(st);

    return 0;
}
