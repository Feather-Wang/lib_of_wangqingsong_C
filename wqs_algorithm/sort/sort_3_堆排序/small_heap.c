#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARENT(index) ((index-1)/2)
#define LCHILD(index) (index*2+1)
#define RCHILD(index) (index*2+2)

/*交换两个变量的值*/
static void swap(int *value1, int *value2)
{
    int temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

/*为整个数组进行heap化，以满足heap的性质*/
void heap_sort(int arr[], int index_last)
{
    int index = index_last, index_temp = -1, index_lchild = 0, index_rchild = 0;

    while( 0 <= index )
    {
        //printf("-------------- %d\n", arr[index]);
        index_lchild = LCHILD(index);
        index_rchild = RCHILD(index);

        /*如果当前结点是叶子结点，则获取上一个结点（相对数组来说）的下标，继续处理*/
        if(index_lchild > index_last && index_rchild > index_last)
        {
            /*index_temp保存处理过的最前面的结点，当需要继续下探，调整heap结构时，靠该变量可以快速回到这个位置*/
            if( -1 != index_temp )
            {
                index = index_temp;
                index_temp = -1;
            }
            else
                --index;
            continue;
        }

        /*如果当前结点有左结点，且当前结点大于左结点，则交换他们的数据，并将下标指向左结点*/
        if(index_lchild <= index_last && arr[index] > arr[index_lchild])
        {
            //printf("lchild...%d <---> %d\n", arr[index], arr[index_lchild]);
            swap(&arr[index], &arr[index_lchild]);
            index = index_lchild;
            continue;
        }

        /*如果当前结点有右结点，且当前结点大于右结点，则交换他们的数据，并将下标指向右结点*/
        if(index_rchild <= index_last && arr[index] > arr[index_rchild])
        {
            //printf("rchild...%d <---> %d\n", arr[index], arr[index_rchild]);
            swap(&arr[index], &arr[index_rchild]);
            index = index_rchild;
            continue;
        }

        /*如果左右结点的数据都处理完毕，则获取上一个结点（相对于数组来说）的下标，继续处理*/
        if( -1 != index_temp )
        {
            index = index_temp;
            index_temp = -1;
        }
        else
            --index;
    }
}

/*
 * size:arr数组的最大容量
 * index_last:最后一个有效结点的下标
 * */
int
heap_insert(int arr[], int value, int size, int index_last)
{
    if( index_last + 1 >= size )
    {
        fprintf(stderr, "arr is no space\n");
        return -1;
    }

    /*将value插入到arr数组最后一个有效结点的后面*/
    ++index_last;
    arr[index_last] = value;
    /*从最后一个结点，开始调整heap*/
    int index_parent = PARENT(index_last), index = index_last;
    while( index_parent != index && arr[index_parent] > arr[index] )
    {
        swap(&arr[index_parent], &arr[index]);
        index = index_parent;
        index_parent = PARENT(index);
    }

    return 0;
}

/*将堆顶结点取出*/
int
heap_delete(int arr[], int index_last)
{
    /*数组是否为空*/
    if( -1 == index_last )
    {
        fprintf(stderr, "this heap is empty\n");
        return 0;
    }

    /*保存堆顶结点到临时变量*/
    int return_value = arr[0];

    int index_lchild = 0, index_rchild = 0, index = 0;
    while( 1 )
    {
        /*获取当前结点的左结点和右结点的下标*/
        index_lchild = LCHILD(index);
        index_rchild = RCHILD(index);
        /*判断：当前结点是否是叶子结点，如果是的话，就将最后一个结点移到这个结点，将最后一个结点置为空，退出循环*/
        if( index_lchild > index_last && index_rchild > index_last )
        {
            arr[index] = arr[index_last];
            arr[index_last] = 0;
            break;
        }

        /*如果不是叶子结点，判断左结点和右结点的大小，将较小的一个移动到当前结点，然后将该子结点设置为当前结点，进行下次循环判断，一直找到最下面的叶子结点*/
        if( arr[index_lchild] < arr[index_rchild] )
        {
            arr[index] = arr[index_lchild];
            index = index_lchild;
        }
        else
        {
            arr[index] = arr[index_rchild];
            index = index_rchild;
        }
    }

    /*从交换后的叶子结点开始，向上调整堆结构*/
    int index_parent = PARENT(index);
    while( index_parent != index && arr[index_parent] > arr[index] )
    {
        swap(&arr[index_parent], &arr[index]);
        index = index_parent;
        index_parent = PARENT(index);
    }

    /*返回保存的堆顶结点*/
    return return_value;
}
