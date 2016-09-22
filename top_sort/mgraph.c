#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "seq_stack.h"
#include "mgraph.h"
/*
typedef struct
{
    int vertex[MAXN];
    int edge[MAXN][MAXN];
    int n;
}mgraph;
*/

// 创建矩阵，将矩阵arr的信息复制到mg里的edge，n记录行数，vertex记录列数
mgraph *mgraph_create(int n, int arr[][n])
{
    mgraph *mg;
 
    if ((mg = (mgraph *)malloc(sizeof(mgraph))) == NULL)
    {
        printf("malloc error\n");
        return mg;
    }

    bzero(mg, sizeof(mgraph));
    mg->n = n;

    int i, j;
    for (i = 0; i < n; i++)
    {
        mg->vertex[i] = i;
        for (j = 0; j < n; j++)
        {
            mg->edge[i][j] = arr[i][j];
        }
    }

    return mg;
}

int topsort(mgraph *mg, int id[])
{
    int i, j, v, count = 0;
    seqstack * s = seqstack_create();

    for (i = 0; i < mg->n; i++)
    {
        id[i] = 0;
        for (j = 0; j < mg->n; j++)
            id[i] += mg->edge[j][i];

        if (id[i] == 0)
            push(s, i);
    }

    while (! seqstack_empty(s))
    {
        pop(s, &v);
        count++;
        printf("V%d ", v);
        for (i = 0; i < mg->n; i++)
        {
            if (mg->edge[v][i])
            {
                id[i]--;
                if (!id[i])
                    push(s, i);
            }
        }
    }

    if (count == mg->n)
        return 1;

    return 0;
}
