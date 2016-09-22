#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seq_stack.h"

// 建立栈头
seqstack * seqstack_create(int stack_size)
{
    seqstack *st;
    st = (seqstack *)malloc(sizeof(seqstack));
    if ( NULL == st )
        return NULL;
    memset(st, 0, sizeof(seqstack));

    st->data = (datatype *)malloc(stack_size*sizeof(datatype));
    if ( NULL == st->data )
    {
        free(st);
        return NULL;
    }
    st->stack_size = stack_size;

    st->top = -1;

    return st;
}

// 销毁栈
void seqstack_destroy(seqstack *st)
{
    if( NULL != st->data )
        free(st->data);

    if( NULL != st )
        free(st);
}

// 栈是否为空
int seqstack_empty(seqstack *st)
{
    return (st->top == -1);
}

// 入栈
int seqstack_push(seqstack *st, datatype *entry)
{
    if (st->top == st->stack_size-1)
    {
        printf("stack is full\n");
        return -1;
    }
    st->top++;
    st->data[st->top] = *entry;

    return 0;
}

// 出栈
int seqstack_pop(seqstack *st, datatype **entry)
{
    if (st->top == -1)
    {
        printf("stack is empty\n");
        return -1;
    }

    *entry = (datatype*)malloc(sizeof(datatype));
    if( NULL == entry )
        return -1;

    **entry = st->data[st->top];
    st->top--;

    return 0;
}
