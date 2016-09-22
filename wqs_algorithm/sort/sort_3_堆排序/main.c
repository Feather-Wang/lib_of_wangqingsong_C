#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show(int arr[], int len)
{
    int i = 0;
    for(i = 0; i < len; i++) 
    {
        printf("[%d] %d ", i, arr[i]);
    }

    putchar(10);
}

int main(int argc, const char *argv[])
{
    int arr[7] = {10, 9, 8, 4, 3, 6};
    //int arr[] = {10, 9, 8, 4, 3, 6, 200, 30, 49, 28, 48, 57};
    int len = 6;
    heap_sort(arr, len-1);

    show(arr, 6);

    heap_insert(arr, 5, 7, len-1);

    show(arr, 7);

    int heap_top = heap_delete(arr, 6);

    show(arr, 6);
    
    return 0;
}
