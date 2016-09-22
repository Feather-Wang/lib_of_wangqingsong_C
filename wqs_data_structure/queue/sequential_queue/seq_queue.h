#ifndef SEQUEUE_H_
#define SEQUEUE_H_

typedef int datatype;
typedef struct
{
    datatype *data;
    int size;
    int front;
    int rear;
}sequeue;

#define foreach(head, entry)\
    int tmp_local_tmp;for(tmp_local_tmp = (head)->front, entry = &(head)->data[tmp_local_tmp]; tmp_local_tmp != (head)->rear; tmp_local_tmp = (tmp_local_tmp+1)%head->size, entry = &(head)->data[tmp_local_tmp])

extern sequeue * queue_create();               // create sequeue, and set front and rear is 0. success return sequeue point and fail return NULL.
extern void queue_destroy(sequeue *sq);               // destroy the sequeue

extern int queue_push(sequeue *sq, datatype x);   // add x to sequeue sq. success return 0 and fail return -1.
extern int queue_pull(sequeue *sq, datatype *x);  // dequeue x from sequeue sq. success return 0 and fail return -1.

extern int queue_empty(sequeue *sq);           // judge whether the queue sq is empty. if sq is empty, return 1, otherwise return 0.

#endif
