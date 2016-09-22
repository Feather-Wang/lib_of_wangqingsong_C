#include <stdio.h>
#include <string.h>
#include "mgraph.h"

#define N 6

int main()
{
    mgraph *mg;
    int id[N];
    
	int matrix[N][N]={
	  /*		 v0 v1 v2 v3 v4 v5						*/	
	  /*v0*/	{0, 1, 0, 1, 1, 0},
	  /*v1*/	{0, 0, 0, 0, 0, 1},
	  /*v2*/	{0, 1, 0, 0, 0, 1},
	  /*v3*/	{0, 0, 0, 0, 0, 0},
	  /*v4*/	{0, 0, 0, 1, 0, 1},
	  /*v5*/	{0, 0, 0, 0, 0, 0}};

    mg = mgraph_create(N, matrix);
    if (topsort(mg, id))
        printf("not cycle\n");
    else
        printf("cycle\n");

	return 0;
}
