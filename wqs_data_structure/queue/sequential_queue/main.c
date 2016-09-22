// 顺序队列
#include <stdio.h>
#include <stdlib.h>
#include "seq_queue.h"

int main()
{
    int t;

    /*
     *sequeue sq;
     *sq.front=sq.rear=0;
     * */


    /********** 创建队列 : 24是队列的元素上限 *****************/
    sequeue *sq;
    sq = queue_create(24);

    /************ 插入元素 ******************/
    queue_push(sq, 10);
    queue_push(sq, 100);
    queue_push(sq, 1000);

    /************** 遍历队列，不取出 ***********************/
    datatype *element = NULL;
    foreach(sq, element)
    {
        printf("foreach --> element = %d\n", *element);
    }

    /**************** 逐个取出元素 *******************/
    while (! queue_empty(sq))
    {
        queue_pull(sq, &t);
        printf("%d\n", t);
    }

    queue_destroy(sq);

    return 0;
}
