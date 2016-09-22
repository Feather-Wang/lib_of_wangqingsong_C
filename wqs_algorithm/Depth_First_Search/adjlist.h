#ifndef ADJLIST_H_
#define ADJLIST_H_

typedef struct node
{
    int data;
    struct node *next;
}listnode, *linklist;

void adjlist_insert(linklist s, int v, int u);
int firstadj(linklist s, int v);
int nextadj(linklist s, int v, int u);

#endif
