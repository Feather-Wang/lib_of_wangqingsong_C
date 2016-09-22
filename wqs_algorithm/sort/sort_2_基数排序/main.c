#include<std.vimo.h>

void radixSort(int *a,int size)
{
    int temp[10][20]={0};
    int order[10]={0};    
    int i,j,k;
    int n,max;
    int p;
    
    for(i = 0; i < size; i++)
        if( max < a[i] )
            max = a[i];
    n=1;
    while( 0 >= (n-max) )
    {
        printf("n = %d ######################################################\n", n);
        for(i=0;i<size;i++)
        {
            k = (a[i]/n) % 10;
            temp[k][order[k]] = a[i];
            printf("index = %d --> temp[%d][%d] = %d\n", i, k, order[k], a[i]);
            order[k]++;
        }   
        p=0;
        for(i=0;i<10;i++)
        {
            if(order[i] != 0)
            {
                for(j=0;j<order[i];j++)
                {
                    a[p] = temp[i][j];
                    p++;
                }
                order[i] = 0;
            }
        }   
        n *= 10;
    }   
}

void main()
{
    int a[20]={3,22,93,3,5,14,28,65,39,81,71,72,48,39,55,105,129,184,196,208}; 
    int i;
    radixSort(a,5);
    for(i=0;i<5;i++)
        printf("%d\n",a[i]);    
}
