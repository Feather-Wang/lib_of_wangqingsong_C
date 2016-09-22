// 链式队列，记录队列的队头指针和队尾指针，插入时通过队尾插入，查找删除时通过队头

#include <stdio.h>
#include <stdlib.h>
#include "link_queue.h"

void linkqueue_show(linkqueue *head)
{
    listnode *entry = NULL;
    linkqueue_foreach(head, entry)
    {
        printf("linkqueue_foreach --> %d\n", *((int*)entry->data));
    }
}

int main()
{
    int i = 0;
    int *tmp = NULL;
    listnode *entry = NULL;

    /* 创建链式队列 */
    linkqueue *head;
    head = linkqueue_create();

    /* 插入元素 */
    for(i = 0; i < 10; ++i)
    {
        tmp = (int*)malloc(sizeof(int));
        *tmp = i;
        linkqueue_push(head, tmp);
    }

    /* 遍历队列 */
    linkqueue_show(head);

    /* 判断队列是否为空，然后逐一将元素取出来 */
    while ( !linkqueue_empty(head) )
    {
        linkqueue_poll(head, (void*)&tmp);
        printf("%d\n", *tmp);
        free(tmp);
    }

    /* 销毁队列 */
    linkqueue_destroy(head);

    return 0;
}
