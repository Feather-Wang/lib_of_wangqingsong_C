#include <stdio.h>
#include <stdlib.h>

#include "binary_search_tree.h"

#include <time.h>
#include <sys/time.h>

int make_srand(double range)
{
    struct timeval tpstart;
    unsigned int n = 0;

    gettimeofday( &tpstart, NULL);
    srand( tpstart.tv_usec );

    n = (int)(range*rand() / (RAND_MAX+1.0));

    return n;
}

int compareTo(void *va1, void *va2)
{
    int *value1 = (int*)va1;
    int *value2 = (int*)va2;

    if( *value1 < *value2 )
        return -1;
    else if ( *value1 == *value2 ) 
    {
        return 0;
    }
    else if ( *value1 > *value2 ) 
    {
        return 1;
    }
}
int deleteTo(node_t *node)
{
    if( NULL != node->element )
        free(node->element);
}

int main(void) 
{
    /*create binary_search_tree*/
    bstree_t *btree = bstree_create(compareTo, deleteTo);
    if ( NULL == btree ) 
    {
        fprintf(stderr, "bstree_create is error\n");
        return -1;
    }
    
    /*insert ten element to binary_search_tree*/
    int *value = NULL;
    int i = 0;
    int max = 1000000;
    for (i = 0; i < max/10; i++) 
    {
        value = (int*)malloc(sizeof(int));
        if( NULL == value )
        {
            fprintf(stderr, "value is malloc error\n");
            return 0;
        }
        *value = make_srand(max);
        if( bstree_find(btree, (void*)value) )
        {
            i--;
            free(value);
        }
        else
        {
            //printf("[%d] %d\n", i, *value);
            bstree_insert(btree, (void*)value);
        }
    }
    
    //printf("Original:\n");
    print_sorted_tree(btree->root);
    //printf("value = %d\n", *value);
    node_t *node = NULL;
    node = bstree_find(btree, value);
    if( NULL == node ) 
    {
        fprintf(stderr, "don't find [%d]\n", *value);
    }
    else 
    {
        //printf("find element is [%d]\n", *((int*)(node->element)));
    }

    bstree_delete(btree, node);
    //printf("After deletion:\n");
    print_sorted_tree(btree->root);

    /*free btree*/
    bstree_destroy(&btree);

    return 0;
}
