#ifndef _SQLIST_H_
#define _SQLIST_H_

#define N 100
typedef int datatype;
typedef struct
{
    datatype data[N]; // 存放数据的顺序队列，固定的最大个数
    int last;         // 队列中的存入的个数, -1表示无数据，0表示第0位有数据，1表示0-1位有数据，以此类推
}sqlist, *sqlink;

#define seq_foreach_safe(head, entry) \
    for(entry = &(head)->data[0]; entry <= (head)->data+(head)->last; ++entry)

extern sqlink sqlist_create();                             // create sqlist, and set last is -1. fail return NULL and success return sqlink point.
void sqlist_destroy(sqlink L);                             // destroy the sqlist

extern int sqlist_empty(sqlink L);                         // judge whether the sqlist L is empty. empty return 1 and not empty return 0.

extern int sqlist_get(sqlink L, int pos, datatype *x);     // get x from sqlist L at specified location pos. pos is 0 to L->last. success return 0 and fail return -1.
extern int sqlist_locate(sqlink L, datatype x);            // judge whether the sqlist L contains x. L contains x return index value and otherwise return -1.
extern int sqlist_insert(sqlink L, datatype x, int pos);   // insert x to sqlist L at specified location with pos. success return 0 and fail return -1.
extern int sqlist_delete(sqlink L, int pos);               // delete data from sqlist L at specified location with pos. success return 0 and fail return -1.
extern int sqlist_union(sqlink La, sqlink Lb);             // insert element from Lb to La with La not contains element. success return 0 and fail return -1.
extern int sqlist_purge(sqlink L);                         // delete repeated element from sqlist L. success return 0.

#endif
