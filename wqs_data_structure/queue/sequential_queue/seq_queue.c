#include <stdio.h>
#include <stdlib.h>
#include "seq_queue.h"

// 创建队列
sequeue * queue_create(int size)
{
    sequeue *sq = NULL;
    sq = (sequeue *)malloc(sizeof(sequeue));
    if ( NULL == sq )
        return NULL;

    sq->data = (datatype*)malloc(sizeof(datatype)*size);
    if( NULL == sq->data )
        return NULL;

    sq->size = size;

    sq->front = sq->rear = 0;

    return sq;
}

// 销毁队列
void queue_destroy(sequeue *sq)
{
    if( NULL != sq->data )
        free(sq->data);
    if( NULL != sq )
        free(sq);
}

// 将x插入到队列中，然后队列尾向后移动一位
int queue_push(sequeue *sq, datatype x)
{
    if( NULL == sq )
    {
        printf("queue point is invalid\n");
        return -1;
    }

    if ((sq->rear + 1) % sq->size == sq->front)
    {
        printf("queue is full\n");
        return -1;
    }

    sq->data[sq->rear] = x;
    sq->rear = (sq->rear+1) % sq->size;

    return 0;
}

// 取出队列中的第一个元素，然后队列头向后移动一位
int queue_pull(sequeue *sq, datatype *x)
{
    if( NULL == sq )
    {
        printf("queue point is invalid\n");
        return -1;
    }

    if (sq->front == sq->rear)
    {
        printf("queue is empty\n");
        return -1;
    }

    *x = sq->data[sq->front];
    sq->front = (sq->front + 1) % sq->size;

    return 0;
}

//队列是否为空
int queue_empty(sequeue *sq)
{
    if( NULL == sq )
    {
        printf("queue point is invalid\n");
        return -1;
    }

    return (sq->front == sq->rear);
}
