#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seq_stack.h"

// 建立栈头
seqstack * seqstack_create()
{
    seqstack *s;
    s = (seqstack *)malloc(sizeof(seqstack));
    if ( NULL == s )
        return s;

    memset(s, 0, sizeof(seqstack));
    s->top = -1;

    return s;
}

// 栈是否为空
int seqstack_empty(seqstack *s)
{
    return (s->top == -1);
}

// 入栈
int push(seqstack *s, datatype x)
{
    if (s->top == N-1)
    {
        printf("stack is full\n");
        return -1;
    }
    s->top++;
    s->data[s->top] = x;

    return 0;
}

// 出栈
int pop(seqstack *s, datatype *p)
{
    if (s->top == -1)
    {
        printf("stack is empty\n");
        return -1;
    }

    *p = s->data[s->top];
    s->top--;

    return 0;
}
