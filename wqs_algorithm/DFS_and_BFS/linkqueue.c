#include <stdio.h>
#include <stdlib.h>
#include "linkqueue.h"

linkqueue * queue_create()
{
    linkqueue *lq;
    linklist p;

    p = (linklist)malloc(sizeof(listnode));
    if (p == NULL) return NULL;

    p->next = NULL;

    lq = (linkqueue *)malloc(sizeof(linkqueue));
    if (lq == NULL) return lq;

    lq->front = lq->rear = p;

    return lq;
}

int enqueue(linkqueue *lq, datatype x)
{
    linklist p;

    p = (linklist)malloc(sizeof(listnode));
    if (p == NULL) return -1;

    p->data = x;
    p->next = NULL;

    lq->rear->next = p;
    lq->rear = p;

    return 0;
}

int dequeue(linkqueue *lq, datatype* x)
{
    linklist p;

    if (lq->front->next == NULL)
    {
        printf("queue is empty\n");
        return -1;
    }

    p = lq->front;
    lq->front = p->next;
    free(p);
    p = NULL;

    *x = lq->front->data;

    return 0;
}

int queue_empty(linkqueue *lq)
{
    return (lq->front == lq->rear);
}

