#ifndef LINKQUEUE_H_
#define LINKQUEUE_H_

typedef int datatype;

typedef struct node
{
    datatype data;
    struct node *next;
}listnode, *linklist;

typedef struct
{
    linklist front;
    linklist rear;
}linkqueue;

linkqueue * queue_create();
int enqueue(linkqueue *lq, datatype value);
int dequeue(linkqueue *lq, datatype* value);
int queue_empty(linkqueue *lq);

#endif
