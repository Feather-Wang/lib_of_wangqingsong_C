#ifndef __SMALL_HEAP_H__

#define __SMALL_HEAP_H__

typedef small_heap_s small_heap_t;

struct small_heap_s
{
    int *arr;
    int size;
    int len;
};

extern void heap_sort(int arr[], int len);
extern int heap_insert(int arr[], int value, int size, int len);


#endif /* end of include guard: __SMALL_HEAP_H__ */
