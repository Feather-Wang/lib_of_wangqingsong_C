#ifndef __SEQ_STACK_H__
#define __SEQ_STACK_H__

typedef int datatype;

typedef struct 
{
    datatype *data; // 栈空间
    unsigned int stack_size;
    unsigned int top;          // 栈的数据个数
}seqstack;

extern seqstack * seqstack_create(int stack_size);         // create sqstack, and set top is -1. success return sqstack point and fail return NULL.
extern int seqstack_empty(seqstack *st);     // judge whether sqstack st is empty. if st is empty, return 1, otherwise return 0.
extern int seqstack_push(seqstack *st, datatype *entry);   // add x to sqstack st. success return 0 and fail return -1.
extern int seqstack_pop(seqstack *st, datatype **entry);   // pop up from sqstack st to p. success return 0 and fail return -1.

#endif
