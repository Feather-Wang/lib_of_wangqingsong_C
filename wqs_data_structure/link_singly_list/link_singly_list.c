#include <stdio.h>
#include <stdlib.h>
#include "link_singly_list.h"

// 创建链表头
listnode* list_create()
{
    listnode *H = NULL;

    H = (listnode *)malloc(sizeof(listnode));
    if ( NULL == H )
    {
        printf("malloc error\n");
        return NULL;
    }

    H->data = NULL;
    H->next = NULL;
    
    return H;
}

/* 销毁链表 */
void list_destroy(listnode *H)
{
    listnode * p = H->next;

    if( NULL == p )
    {
        printf("the link list is empty\n");
        return;
    }
    
    listnode * tmp = NULL;
    while ( NULL != p )
    {
        tmp = p;
        p = p->next;
        if( NULL != tmp->data)
            free(tmp->data);
        if( NULL != tmp )
            free(tmp);
    }
    free(H);
}

/* 1 empty*/
int list_empty(listnode *H)
{
    return ( H->next == NULL );
}

// 查找是否有该成员节点，存在返回0，不存在返回-1
int list_locate(listnode *H, void *data, listnode **entry)
{
    listnode * p = H->next;

    while ( NULL != p )
    {
        if ( *((int*)p->data) == *((int*)data) )
        {
            *entry = p;
            return 0;
        }

        p = p->next;
    }

    return -1;
}

// 头部插入
int list_insert(listnode *H, void *data)
{
    listnode * p = NULL; 

    p  = (listnode *)malloc(sizeof(listnode));
    if ( NULL == p )
    {
        printf("malloc error\n");
        return -1;
    }
    p->data = data;
    
    p->next = H->next;
    H->next = p;

    return 0;
}

// 按data的大小，排序插入
int list_order_insert(listnode *H, void *data)
{
    listnode * p = H, *q = NULL;

    while (p->next && *((int*)p->next->data) < *((int*)data))
        p = p->next;

    q  = (listnode *)malloc(sizeof(listnode));
    if ( NULL == q )
    {
        printf("malloc error\n");
        return -1;
    }
    q->data = data;

    q->next = p->next;
    p->next = q;

    return 0;
}

// 删除节点
int list_delete(listnode *H, void *data)
{
    listnode * p = H->next, *q = NULL;

    if ( NULL == p )
    {
        printf("remove position is invalid\n");
        return -1;
    }

    while( p )
    {
        if( *((int*)p->data) == *((int*)data) )
        {
            q->next = p->next;
            if(p->data)
                free(p->data);
            if(p)
                free(p);
            return 0;
        }

        q = p;
        p = p->next;
    }

    return -1;
}

int list_delete_entry(listnode *H, listnode *entry)
{
    listnode * p = H->next, *q = NULL;

    if ( NULL == p )
    {
        printf("remove position is invalid\n");
        return -1;
    }

    while( p )
    {
        if( *((int*)p->data) == *((int*)entry->data) )
        {
            q->next = p->next;
            if(p->data)
                free(p->data);
            if(p)
                free(p);
            return 0;
        }

        q = p;
        p = p->next;
    }


    return -1;
}

// 反转链表
int list_reverse(listnode *H)
{
    if( NULL == H )
    {
        puts("H is NULL");
        return -1;
    }

    listnode *prev = H->next, *next = NULL;

    if( NULL == prev || NULL == prev->next)
    {
        puts("this list is empty or has just one node");
        return 0;
    }

    next = prev->next;
    while ( prev->next )
    {
        prev->next = next->next;
        next->next = H->next;
        H->next = next;

        next = prev->next;
    }

    return 0;
}
