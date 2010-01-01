#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "b+tree.h"

#include <time.h>
#include <sys/time.h>

int make_srand(int range)
{
    struct timeval tpstart;
    unsigned int n = 0;

    gettimeofday( &tpstart, NULL);
    unsigned int seed = tpstart.tv_usec;
    srand(seed);

    n = rand_r(&seed) % range;

    return n;
}

int compareTo(void *va1, void *va2)
{
    int *value1 = (int*)va1;
    int *value2 = (int*)va2;
    int ret = -1;

    if( *value1 < *value2 )
    {
        ret = -1;
    }
    else if ( *value1 == *value2 ) 
    {
        ret = 0;
    }
    else if ( *value1 > *value2 ) 
    {
        ret = 1;
    }

    return ret;
}
int deleteTo(void *key)
{
    if( NULL != key )
        free(key);
    return 0;
}

/*层序遍历*/
bptree_node_t *queue[100000];
#define MOVE(x) (x=(x+1)%100000)
void print(bptree_t *btree)
{
    puts("Show...");
    bptree_node_t *p = btree->root;
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
        
        putchar('[');
        int index = 0;
        for(index = 0; index < p->keynum; index++) 
        {
            if( NULL != p->child[index] )
                queue[MOVE(rear)]=p->child[index],temp++;
            printf("%d  ", *((int*)(p->key[index])));
        }
        putchar(']');

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
    printf("rbptree_create running...\n");
    bptree_t *btree = bptree_create(compareTo, deleteTo);
    if ( NULL == btree ) 
    {
        fprintf(stderr, "rbptree_create is error\n");
        return -1;
    }
    
    int *delete_value = (int*)malloc(sizeof(int));
    /*insert ten element to binary_search_tree*/
    int *value = NULL;
    int i = 0;
    int max = 500000;
    for (i = 0; i < max; i++) 
    {
        value = NULL;
        value = (int*)malloc(sizeof(int));
        if( NULL == value )
        {
            fprintf(stderr, "value is malloc error\n");
            return 0;
        }
        //*value = i;
        *value = make_srand(max*10);
        if( 0 == bptree_search(btree, (void*)value) )
        {
            printf("[%d] --- [%d] node is existed\n", i, *value);
            i--;
            free(value);
        }
        else
        {
            //printf("----------------------------------------------------------------- [%d] %d\n", i, *value);
            if( i == max/2 )
                *delete_value = *value;

            bptree_insert(btree, (void*)value);
            //print(btree);
        }
    }
    
#if 1
    //printf("Original:\n");
    //print(btree);

    //*delete_value = 9;
    printf("rbptree_delete %d\n", *delete_value);
    bptree_delete(btree, delete_value);
    //printf("After deletion:\n");
    //print(btree);
#endif

    //check_tree(btree, btree->root, 0, NULL);

    /*free btree*/
    puts("rbptree_destron running...");
    bptree_destory(&btree);
    if( NULL != delete_value )
        free(delete_value);

    return 0;
}

int main(int argc, const char *argv[])
{
    int i = 0;
    do{
        printf("main --> i = %d\n", i++);
        test();
        printf("\n\n\n");
        sleep(10);
#if 1
    }while( 1 );
#else
    }while( 0 );
#endif
    return 0;
}
