//图的应用：图的深度优先搜索遍历
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adjlist.h"

#define N 9

int main()
{
    listnode adjlist[N] = {{0, NULL}};
    int visited[N] = {0};
    
    int i, j;
    int a[][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 5},
        {2, 6}, {2, 7}, {3, 4}, {4, 5}, {4, 8}, {6, 7}};

    for (i = 0; i < N; i++)
        adjlist[i].data = i;

    // 0-9， 每一个数字一个链表，将相连的数字插入到对应的链表中
    for (i = 0; i < 10; i++)
    {
        adjlist_insert(adjlist, a[i][0], a[i][1]);
        adjlist_insert(adjlist, a[i][1], a[i][0]);
    }


    linklist p;
    for (i = 0; i < N; i++)
    {
        printf("V%d ", i);
        p = adjlist[i].next;
        while (p)
        {
            printf("V%d ", p->data);
            p = p->next;
        }
        printf("\n");
    }
    printf("*****************************\n");
    DFS(adjlist, 0, visited);
    printf("\n");

    return 0;
}
