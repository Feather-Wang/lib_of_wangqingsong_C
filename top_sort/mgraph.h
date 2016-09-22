#ifndef MGRAPH_H_
#define MGRAPH_H_

#define MAXN 64

typedef struct
{
    int vertex[MAXN];
    int edge[MAXN][MAXN];
    int n;
}mgraph;

mgraph *mgraph_create(int n, int arr[][n]);
int topsort(mgraph *mg, int id[]);

#endif
