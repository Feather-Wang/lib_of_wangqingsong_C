// 单向链表的简单运用

#include <stdio.h>
#include <stdlib.h>
#include "link_singly_list.h"
#include <malloc.h>

void list_show(listnode *H, int type)
{
    int *tmp = NULL;
    listnode *entry = NULL;
    linklist_foreach(H, entry)
    {
        tmp = (int*)entry->data;
        printf("%d : foreach --> %d\n", type, *tmp);
    }
    puts("");
}

int main()
{
    /* 创建链表 */
    listnode* H;
    if ( NULL == (H = list_create()) )
        return -1;

    listnode *entry = NULL;

    /* 判断链表是否为空 */
    if( list_empty(H) )
        puts("this list is empty");

#if 0
    /* 插入两个元素 */
    int *tmp = (int*)malloc(sizeof(int));
    *tmp = 10;
    list_insert(H, tmp);
    tmp = (int*)malloc(sizeof(int));
    *tmp = 20;
    list_insert(H, tmp);
#else
    /* 按元素的某个成员顺序插入两个元素 */
    int *tmp = NULL;
    int i = 0;
    for(i = 0; i < 10; ++i)
    {
        tmp = (int*)malloc(sizeof(int));
        *tmp = i;
        list_order_insert(H, tmp);
    }
#endif

    /* 查找链表中是否有指定的元素 */
    if( -1 == list_locate(H, tmp, &entry) )
    {
        printf("there isn't %d in list\n", *tmp);
    }
    else
    {
        printf("list_locate --> %d\n", *((int*)entry->data));
    }

    /* 遍历显示所有元素 */
    list_show(H, 1);

    /* 删除元素 */
    //list_delete(H, tmp);
    list_delete_entry(H, entry);

    /* 遍历显示所有元素 */
    list_show(H, 2);

    /* 反转链表 */
    list_reverse(H);

    /* 遍历显示所有元素 */
    list_show(H, 3);

    /* 销毁链表 */
    list_destroy(H);

    return 0;
}
