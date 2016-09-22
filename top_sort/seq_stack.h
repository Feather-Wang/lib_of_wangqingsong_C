#ifndef seqstack_H_
#define seqstack_H_

typedef int datatype;
#define N 128

typedef struct 
{
    datatype data[N]; // 栈空间
    int top;          // 栈的数据个数
}seqstack;

seqstack * seqstack_create();         // create sqstack, and set top is -1. success return sqstack point and fail return NULL.
int seqstack_empty(seqstack *s);     // judge whether sqstack s is empty. if s is empty, return 1, otherwise return 0.
int push(seqstack *s, datatype x);   // add x to sqstack s. success return 0 and fail return -1.
int pop(seqstack *s, datatype *p);   // pop up from sqstack s to p. success return 0 and fail return -1.

#endif

