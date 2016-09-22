#include <stdio.h>
#include <stdlib.h>
#include "adjlist.h"

void adjlist_insert(linklist s, int v, int u)//insert(s, 1, 3)
{
    linklist p, q;
    p = s+v;//&s[v]

    q = (linklist)malloc(sizeof(listnode));
    if (q == NULL) return ;
    q->data = u;

    while (p->next != NULL && p->next->data < u)
        p = p->next;

    q->next = p->next;
    p->next = q;
}

int firstadj(linklist s, int v)//firstadj(s, 1)
{
    if (s[v].next)
        return s[v].next->data;

    return -1;
}

int nextadj(linklist s, int v, int u)//nextadj(s, 1, 3)
{
    linklist p = s[v].next;
    while (p->data != u)
        p = p->next;

    if (p->next)
        return p->next->data;

    return -1;
}

void DFS(linklist s, int v, int *visited)
{
    int u;

    printf("V%d ", v);
    visited[v] = 1;

    u = firstadj(s, v);

    while (u != -1)
    {
        if (!visited[u])
            DFS(s, u, visited);
        u = nextadj(s, v, u);
    }
}
