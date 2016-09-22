#include <stdio.h>
#include <stdlib.h>

int partition(int *a, int low, int high)
{
    int key = a[low];

    while (low < high)
    {
        while (low < high && key <= a[high])
            high--;
        if (low < high) 
            a[low] = a[high];

        while (low < high && key >= a[low])
            low++;
        if (low < high)
            a[high] = a[low];
    }

    a[low] = key;
    return low;
}

void quicksort(int *a, int low, int high)
{
    int t;

    if (low >= high)
        return;
    t = partition(a, low, high);
    quicksort(a, low, t-1);
    quicksort(a, t+1, high);
}

int main()
{   
    int a[] = {111, 34, 78, 34, 32, 21, 12, 12}, i;
//    int a[] = {111, 34, 12, 78, 21, 12, 32, 34}, i;
    int n = sizeof(a) / sizeof(int); 

    quicksort(a, 0, n-1);

    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}
