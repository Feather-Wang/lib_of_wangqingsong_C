#include <stdio.h>
#include <stdlib.h>

#include "balance_binary_tree.h"

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

/*层序遍历*/
node_t *queue[1000];
#define MOVE(x) (x=(x+1)%1000)
void print(node_t *p)
{
    if( NULL == p )
    {
        printf("ths tree is empty!\n");
        return;
    }
    int front,rear,temp,count;

    front=rear=-1; count=temp=0;
    queue[MOVE(rear)]=p; count++;
    
    while(front!=rear)
    {
        p=queue[MOVE(front)]; 
        count--;
        
        
        if(p->lchild)  
            queue[MOVE(rear)]=p->lchild,temp++;
        if(p->rchild)  
            queue[MOVE(rear)]=p->rchild,temp++;
        
        printf("%d->%d ", *((int*)p->element), p->bf);
        if(count==0)
        {
            printf("\n");
            count=temp;
            temp=0;
        }    
    }
    printf("\n");
}

int test(void) 
{
    /*create binary_search_tree*/
    bbtree_t *btree = bbtree_create(compareTo, deleteTo);
    if ( NULL == btree ) 
    {
        fprintf(stderr, "bbtree_create is error\n");
        return -1;
    }
    
    int *delete_value = NULL;
    /*insert ten element to binary_search_tree*/
    int *value = NULL;
    int i = 0;
    int max = 10000;
    for (i = 0; i < max; i++) 
    {
        value = (int*)malloc(sizeof(int));
        if( NULL == value )
        {
            fprintf(stderr, "value is malloc error\n");
            return 0;
        }
        *value = make_srand(max);
        if( bbtree_find(btree, (void*)value) )
        {
            i--;
            free(value);
        }
        else
        {
            if( NULL == delete_value && i == max/2 )
                delete_value = value;

            printf("[%d] %d\n", i, *value);
            bbtree_insert(btree, &(btree->root), (void*)value);
        }
    }
    
#if 0
    printf("Original:\n");
    print(btree->root);
    printf("value = %d\n", *value);
    node_t *node = NULL;
    node = bbtree_find(btree, value);
    if( NULL == node ) 
    {
        fprintf(stderr, "don't find [%d]\n", *value);
    }
    else 
    {
        //printf("find element is [%d]\n", *((int*)(node->element)));
    }
#endif

    //printf("bbtree_delete %d\n", *delete_value);
    bbtree_delete(btree, &(btree->root), delete_value);
    //printf("After deletion:\n");
    //print(btree->root);

    /*free btree*/
    bbtree_destroy(&btree, &(btree->root));

    return 0;
}

int main(int argc, const char *argv[])
{
    while(1) 
    {
        test();
    }
    return 0;
}
