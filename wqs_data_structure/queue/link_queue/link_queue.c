#include <stdio.h>
#include <stdlib.h>
#include "link_queue.h"


// 创建链式队列
linkqueue* linkqueue_create()
{
    linkqueue *head = NULL;

    head = (linkqueue *)malloc(sizeof(linkqueue));
    if ( NULL == head ) 
    {
        return NULL;
    }
    head->front = head->rear = NULL;

    return head;
}

/* 销毁队列 */
void linkqueue_destroy(linkqueue *head)
{
    listnode *entry = NULL;

    while( !linkqueue_empty(head) )
    {
        entry = head->front;
        head->front = head->front->next;

        if( NULL != entry->data )
            free(entry->data);
        if( NULL != entry )
            free(entry);
    }
}

// 插入,从尾部插入，由于有记录尾部的指针，所以不需要遍历链表
int linkqueue_push(linkqueue *head, void *data)
{
    listnode *node = NULL;

    node = (listnode *)malloc(sizeof(listnode));
    if ( NULL == node ) 
        return -1;

    node->data = data;
    node->next = NULL;

    if( NULL == head->front && NULL == head->rear )
    {
        head->front = head->rear = node;
    }
    else
    {
        head->rear->next = node;
        head->rear = node;
    }
    return 0;
}

/* 将第一个节点的data部分的指针赋给data，head->front后移一个节点，释放第一个节点 */
int linkqueue_poll(linkqueue *head, void **data)
{
    if ( linkqueue_empty(head) )
    {
        printf("queue is empty\n");
        return -1;
    }

    listnode *node = NULL;
    node = head->front;
    if( head->front == head->rear )
    {
        head->front = head->rear = NULL;
    }
    else
    {
        head->front = head->front->next;
    }

    *data = node->data;
    free(node);

    return 0;
}

// 链表是否为空
int linkqueue_empty(linkqueue *head)
{
    return (NULL == head->front && NULL == head->rear);
}

