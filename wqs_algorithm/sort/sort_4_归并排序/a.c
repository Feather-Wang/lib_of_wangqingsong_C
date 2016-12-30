/*理解：利用二分法，将数组不断的进行二分，最后就会分解成单个的元素，然后再重组成新的数组，在重组的过程中，不需要遍历数组进行比较排序，因为重组时，双方必然是排序好的，只需要依次将两个数组插入到临时数组中就可以了，在这个过程中，仅仅需要注意*/
#include <stdlib.h>
#include <stdio.h>

int count = 0;

void Merge(int sourceArr[],int tempArr[], int startIndex, int midIndex, int endIndex)
{
    printf("startIndex = %d, midIndex = %d, endIndex = %d\n", startIndex, midIndex, endIndex);
    int i = startIndex, j=midIndex+1, k = startIndex;

    while(i!=midIndex+1 && j!=endIndex+1)
    {
        printf("i = %d,%d j = %d,%d k = %d,%d\n", i, sourceArr[i], j, sourceArr[j], k, tempArr[k]);
        count ++;
        if(sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];
    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
}

//内部使用递归
void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex)
{
    printf("startIndex = %d, endIndex = %d\n", startIndex, endIndex);
    int midIndex;
    if(startIndex < endIndex)
    {
        midIndex = (startIndex + endIndex) / 2;
        MergeSort(sourceArr, tempArr, startIndex, midIndex);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex);
        Merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
    }
}

int main(int argc, char * argv[])
{
    //int a[8] = {50, 10, 20, 30, 70, 40, 80, 60};
    int a[7] = {6, 202, 100, 301, 38, 8, 1};
    int i, b[7];
    MergeSort(a, b, 0, 6);
    for(i=0; i<7; i++)
        printf("%d ", a[i]);
    printf("\n");
    printf("count = %d\n", count);
    return 0;
}
