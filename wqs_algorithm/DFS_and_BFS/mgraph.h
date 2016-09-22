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
int firstadj(mgraph *mg, int u);
int nextadj(mgraph *mg, int u, int w);
void DFS(mgraph *mg, int v, int *visited);
void BFS(mgraph *mg, int v);

#endif
