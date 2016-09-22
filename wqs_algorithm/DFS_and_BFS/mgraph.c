#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "linkqueue.h"
#include "mgraph.h"

mgraph *mgraph_create(int n, int arr[][n])
{
    mgraph *mg;
    int i, j;

    if ((mg = (mgraph *)malloc(sizeof(mgraph))) == NULL)
            return mg;

    mg->n = n;
    for (i = 0; i < n; i++)
    {
        mg->vertex[i] = i;
        for (j = 0; j < n; j++)//memcpy
            mg->edge[i][j] = arr[i][j];
    }

    return mg;
}

int firstadj(mgraph *mg, int u)
{
    int i;
    for (i = 0; i < mg->n; i++)
        if (mg->edge[u][i])
            return i;

    return -1;
}

int nextadj(mgraph *mg, int u, int w)//(mg, 2, 1)
{
    int i;

    for (i = w+1; i < mg->n; i++)
    {
        if (mg->edge[u][i])
            return i;
    }
    return -1;
}

void DFS(mgraph *mg, int v, int *visited)
{
    int u;

    printf("V%d ", v);
    visited[v] = 1;

    u = firstadj(mg, v);

    while (u != -1)
    {
        if (!visited[u])
            DFS(mg, u, visited);
        u = nextadj(mg, v, u);
    }
}

void BFS(mgraph *mg, int v)
{
    int visited[MAXN] = {0}, i;
    linkqueue *lq;

    printf("V%d ", v);
    visited[v] = 1;

    lq = queue_create();
    enqueue(lq, v);

    while (!queue_empty(lq))
    {
        dequeue(lq, &v);

        for (i = 0; i < mg->n; i++)
        {
            if (mg->edge[v][i])
            {
                if (!visited[i])
                {
                    printf("V%d ", i);
                    visited[i] = 1;
                    enqueue(lq, i);
                }
            }
        }
    }
    printf("\n");
}
