#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mgraph.h"

#define N 9

int main()
{
    mgraph *mg;
    int visited[N] = {0};
    int matrix[N][N]={//v0 v1 v2 v3 v4 v5 v6 v7 v8
                 /*v0*/ {0, 1, 1, 0, 0, 0, 0, 0, 0},
                 /*v1*/ {1, 0, 0, 1, 0, 1, 0, 0, 0},
                 /*v2*/ {1, 0, 0, 0, 0, 0, 1, 1, 0},
                 /*v3*/ {0, 1, 0, 0, 1, 0, 0, 0, 0},
                 /*v4*/ {0, 0, 0, 1, 0, 1, 0, 0, 1},
                 /*v5*/ {0, 1, 0, 0, 1, 0, 0, 0, 0},
                 /*v6*/ {0, 0, 1, 0, 0, 0, 0, 1, 0},
                 /*v7*/ {0, 0, 1, 0, 0, 0, 1, 0, 0},
                 /*v8*/ {0, 0, 0, 0, 1, 0, 0, 0, 0}
    };
    
    mg = mgraph_create(N, matrix);

    DFS(mg, 0, visited);
    printf("\n");

    BFS(mg, 0);
    
    return 0;
}

