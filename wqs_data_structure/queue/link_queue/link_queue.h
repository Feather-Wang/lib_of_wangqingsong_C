#ifndef LINK_QUEUE_H_
#define LINK_QUEUE_H_

typedef struct node
{
    void *data; // 存放的数据
    struct node *next; // 指向下一个节点的指针
}listnode;

typedef struct
{
    listnode *front; // 指向队列头的指针
    listnode *rear;  // 指向队列尾的指针
}linkqueue;

#define linkqueue_foreach(head, entry)\
    for(entry = head->front; entry != head->rear->next; entry = entry->next)

linkqueue* linkqueue_create();                     // create link queue. success return linkqueue point and fail return NULL.
void linkqueue_destroy(linkqueue *head);           // destroy the link queue.

int linkqueue_push(linkqueue *head, void *data);         // add data to link queue head. success return 0 and fail return -1.
int linkqueue_poll(linkqueue *head, void **data);        // pull the first node's data to data, and free the first data ( expect data memory ), success return 0 and fail return -1.
int linkqueue_empty(linkqueue *head);                 // judge whether the link queue head is empty. if lq is empty, return 1 , otherwise return 0.

#endif
