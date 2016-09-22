// 顺序链表
#include <stdio.h>
#include "seq_list.h"

// 输出队列中的元素
int sqlist_show(sqlink L)
{
    datatype *element = 0;
    seq_foreach_safe(L, element)
    {
        printf("L --> element = %d\n", *element);
    }
    return 0;
}

int main()
{
    /*
    sqlist L;
    L.last = -1;
    */
    int a[] = {1, 3, 5, 7}, b[] = {7, 9, 3, 20}, i;

    sqlink L1, L2;
    /******** 创建链表 *******/
    L1 = sqlist_create();
    if (L1 == NULL)
        return 0;

    L2 = sqlist_create();
    if (L2 == NULL)
        return 0;

    /************ 插入元素 **************/
    for (i = 0; i < 4; i++)
    {
        sqlist_insert(L1, a[i], i);
        sqlist_insert(L2, b[i], i);
    }

    /************* 显示链表内容 *****************/
    puts("show L1 ---> ");
    sqlist_show(L1);
    puts("show L2 ---> ");
    sqlist_show(L2);

    /************ 对比链表L1和L2，如果L2中有L1中没有的，则插入到L1中 **************************/
    puts("show union L1 L2 --->");
    sqlist_union(L1, L2);
    sqlist_show(L1);

    puts("show L1 2 0 --->");
    sqlist_insert(L1, 3, 2);
    sqlist_insert(L1, 7, 0);
    sqlist_show(L1);

    /************* 删除重复的内容 ******************************/
    puts("show L1 purge --->");
    sqlist_purge(L1);
    sqlist_show(L1);

    /********* 查找L1中是否有元素1 *********/
    i = sqlist_locate(L1, 1);
    printf("sqlist_locate --> i = %d\n", i);

    /********** 取出L1中第一个元素 *******************/
    sqlist_get(L1, 0, &i);
    printf("sqlist_get --> i = %d\n", i);

    /********** 删除L1中第一个元素 *******************/
    sqlist_delete(L1, 0);

    /*************** 销毁链表 ********************/
    sqlist_destroy(L1);
    sqlist_destroy(L2);
    return 0;
}
