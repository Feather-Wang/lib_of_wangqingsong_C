#include <stdio.h>
#include <stdlib.h>
#include "seq_list.h"

// 创建链表
sqlink sqlist_create()
{
    sqlink L = NULL;
    if ( NULL == (L = (sqlink)malloc(sizeof(sqlist))) )
    {
        printf("malloc error\n");
        return L;
    }
    L->last = -1;

    return L;
}

// 删除链表
void sqlist_destroy(sqlink L)
{
    if( NULL != L )
        free(L);
}

/*
 * @ret:1--emtpy
 * */
int sqlist_empty(sqlink L)
{
    return ( -1 == L->last );
}

// 从链表中取出第pos个的数据，存入x
int sqlist_get(sqlink L, int pos, datatype *x)
{
    //pos--[0, L->last]
    if (pos < 0 || pos > L->last)
    {
        printf("position invalid\n");
        return -1;
    }
    *x = L->data[pos];

    return 0;
}

// 遍历链表，查找是否有x，并返回在队列中的位置
int sqlist_locate(sqlink L, datatype x)
{
    int i;

    for (i = 0; i <= L->last; i++)
    {
        if ( x == L->data[i] )
            return i;
    }

    return -1;
}

// 在指定的pos位置插入x 
int sqlist_insert(sqlink L, datatype x, int pos)
{
    int i;

    if (L->last == N-1)
    {
        printf("List is full\n");
        return -1;
    }

    if (pos < 0 || pos > L->last+1)//[0,L->last+1]
    {
        printf("Insert position is invalid\n");
        return -1;
    }

    for (i = L->last; i >= pos; i--)
        L->data[i+1] = L->data[i];

    L->data[pos] = x;
    L->last++;

    return 0;
}

// 删除第pos个的数据
int sqlist_delete(sqlink L, int pos)
{
    int i;

    if (L->last == -1)
    {
        printf("list is empty\n");
        return -1;
    }

    //pos--[0, L->last]
    if (pos < 0 || pos > L->last)
    {
        printf("delete positon is invalid\n");
        return -1;
    }

    for (i = pos+1; i <= L->last; i++)
        L->data[i-1] = L->data[i];

    L->last--;

    return 0;
}

// 查找Lb的元素是否在La中都存在，如果不存在，则插入La中
int sqlist_union(sqlink La, sqlink Lb)
{
    int i;

    for (i = 0; i <= Lb->last; i++)
    {
        //not exist
        if (sqlist_locate(La, Lb->data[i]) == -1)
        {
            if (sqlist_insert(La, Lb->data[i], La->last+1) == -1)
            {
                return -1;
            }
        }
        //else //exist
    }

    return 0;
}

// 删除链表中重复的元素
int sqlist_purge(sqlink L)
{
    int j, k;

    j = 1;

    while (j <= L->last)
    {
        //data[j]
        for (k = j-1; k >= 0; k--)
        {
            if (L->data[j] == L->data[k])
            {
                sqlist_delete(L, j);
                break;
            }
        }

        //not k -1
        //yes k
        if ( k < 0)
            j++;
    }
    
    return 0;
}
