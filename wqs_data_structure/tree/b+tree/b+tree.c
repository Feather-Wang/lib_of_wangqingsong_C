#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "b+tree.h"

/*node is full*/
    static int
is_full(bptree_t *btree, bptree_node_t *node)
{
    return (node->keynum == btree->M);
}

    static int
get_ceil(bptree_t *btree)
{
    return (btree->M/2 + btree->M%2);
}

/*delete a key*/
    static void
key_destory(bptree_t *btree, void **key)
{
    if(NULL != *key)
    {
        /*if set deleteTo, use deleteTo, otherwise, use free()*/
        if( btree->deleteTo )
        {
            btree->deleteTo(*key);
        }
        else 
        {
            free(*key);
        }
        *key = NULL;
    }
}

/*destory a node*/
    static void
node_destory( bptree_node_t **node )
{
    if( NULL != *node )
    {
        if( NULL != (*node)->key )
            free((*node)->key);
        if( NULL != (*node)->child )
            free((*node)->child);
        free(*node);
        *node = NULL;
    }
}

/*
 * create a node
 * return:
 * a new node pointer
 * */
    static bptree_node_t *
node_create(unsigned int M)
{
    bptree_node_t *node = NULL, *ret = NULL;
    do
    {
        /*申请结点空间，并初始化*/
        node = (bptree_node_t *)malloc(sizeof(bptree_node_t));
        if( NULL == node )
            break;
        memset(node, 0x00, sizeof(bptree_node_t));

        /*keynum为0，is_leaf为1*/
        node->keynum = 0;
        node->is_leaf = 1;

        /*为key值和child值申请空间，大小根据参数M来指定，然后初始化*/
        node->key = (void **)malloc(sizeof(void*)*M);
        if( NULL == node->key )
            break;
        memset(node->key, 0x00, sizeof(void*)*M);

        node->child = (bptree_node_t **)malloc(sizeof(bptree_node_t*)*M);
        if( NULL == node->child )
            break;
        memset(node->child, 0x00, sizeof(bptree_node_t*)*M);

        ret = node;

    }while(0);

    /*如果创建结点失败，则把内存申请成功的部分，重新释放掉*/
    if( NULL == ret )
    {
        node_destory(&node);
    }

    return ret;
}

/*
 * description:
 * 供bptree_destory调用，用于销毁B+Tree
 * 该函数为递归函数*/
    static void
bptree_destory_child(bptree_t *btree, bptree_node_t **node)
{
    /*从0到keynum-1，判断该结点是否有子结点，如果有，则将子结点作为参数，递归调用该函数，直到找到叶子结点，然后调用node_destory()销毁叶子结点*/
    int index = 0;
    for(index = 0; index < (*node)->keynum; index++) 
    {
        if( NULL != (*node)->child[index] )
            bptree_destory_child(btree, &((*node)->child[index]));
        /*释放key值的内存，因为所有的key值在叶子结点上都有，所以释放的时候，只需要释放叶子结点上的key值就行了*/
        if( (*node)->is_leaf )
            key_destory(btree, &((*node)->key[index]));
    }
    node_destory(node);
}


/*
 * 结点间移动key值函数：
 * description:
 * 从右子结点借一个key值到当前子结点：主要用于供bptree_delete调用，在删除过程中，如果要下降的子结点树等于底线值，而相邻子结点都大于底线值，这时候就不能通过合并子结点的办法，保证下降的子结点的key的个数在底线值以上了，这时候就需要将当前结点的一个key值下降到子结点中，然后从相邻结点中借一个key值，补充到当前结点的key值中
 * parent：父结点
 * index_node：需要主动借key值的结点，在parent中key的下标
 * 返回值：
 * -1：失败
 *  0：成功*/
    static int
bptree_move_right_child(bptree_t *btree, bptree_node_t *parent, int index_node)
{
    /*node_1为主动借key值的结点，node_2为被动借key值的结点*/
    bptree_node_t *node_1 = parent->child[index_node];
    bptree_node_t *node_2 = parent->child[index_node + 1];

    /*将parent的key值，移动到node_1中，将node_2的第一个child移动到node_1中，node_1的keynum+1*/
    node_1->key[node_1->keynum] = node_2->key[0];
    node_1->child[node_1->keynum] = node_2->child[0];
    node_1->keynum += 1;


    /*将node_2的key值和child整体前移，因为第一个key值和child都移到了parent和node_1*/
    int index = 0;
    for(index = 0; index < node_2->keynum-1; index++) 
    {
        node_2->key[index] = node_2->key[index + 1];
        node_2->child[index] = node_2->child[index + 1];
    }

    /*将空掉的key和child置为NULL，keynum-1*/
    node_2->key[index] = NULL;
    node_2->child[index] = NULL;
    node_2->keynum--;

    /*将node_2的第一个key值添加到parent中*/
    parent->key[index_node + 1] = node_2->key[0];

    return 0;
}

/*与bptree_move_right_child一样，该函数是从左子结点借过来一个key值*/
    static int
bptree_move_left_child(bptree_t *btree, bptree_node_t *parent, int index_node)
{
    /*node_1为主动借key值的结点，node_2为被动借key值的结点*/
    bptree_node_t *node_1 = parent->child[index_node];
    bptree_node_t *node_2 = parent->child[index_node-1];

    /*将node_1的key值和child整体后移，为即将迁移过来的key和child腾出位置*/
    int index = 0;
    for( index = node_1->keynum - 1 ; index >= 0; index--)
    {
        node_1->key[index + 1] = node_1->key[index];
        node_1->child[index + 1] = node_1->child[index];
    }

    /*将父结点的key值，移动到node_1中，将node_2的最后一个child移动到node_1中*/
    node_1->key[0] = node_2->key[node_2->keynum - 1];
    node_1->child[0] = node_2->child[node_2->keynum - 1];
    node_1->keynum++;

    /*将node_2中的最后一个key值提升到parent中，并将移走的key和child置为空，key值个数减1*/
    parent->key[index_node] = node_2->key[node_2->keynum - 1];
    node_2->key[node_2->keynum - 1] = NULL;
    node_2->child[node_2->keynum - 1] = NULL;
    node_2->keynum--;

    return 0;
}

/*
 * 合并函数：
 * 将两个key值个数为ceil(M/2.0)的子结点，合并成一个，主要供bptree_delete调用，防止在下降过程中，下降到一个ceil(M/2.0)个key值的结点上，这样在做其他操作时，可能会破坏B-Tree的性质
 * parent：将要合并的结点的父结点
 * index_node：将要合并的结点中，用于下沉到子结点中的key的下标
 * 返回值：
 * -1：失败
 *  0：成功*/
    static int
bptree_merge_child(bptree_t *btree, bptree_node_t *parent, int index_node)
{
    //printf("merge 1 : key[index_node] --> %d\n", *((int*)parent->key[index_node]));
    /*获取要合并的两个子结点，node_1结点会吸收node_2结点的key值和child值*/
    bptree_node_t *node_1 = parent->child[index_node-1];
    bptree_node_t *node_2 = parent->child[index_node];

    int index = 0;

    /*从0到node_2->keynum-1，将node_2中的所有的key和child都拷贝到node_1中*/
    for(index = 0; index < node_2->keynum; index++) 
    {
        node_1->key[node_1->keynum+index] = node_2->key[index];
        node_1->child[node_1->keynum+index] = node_2->child[index];
    }
    node_1->keynum += node_2->keynum;

    /*将parent中下沉的key和指向node_2的child都取消，后面的成员往前移动*/
    for( index = index_node; index < parent->keynum-1; index++ )
    {
        parent->key[index] = parent->key[index+1];
        parent->child[index] = parent->child[index+1];
    }
    parent->key[index] = NULL;
    parent->child[index] = NULL;
    parent->keynum--;

    /*因为node_2中的key值和child都迁移到node_1中了，所以node_2已经没有用了，释放掉node_2的内存空间*/
    node_destory(&node_2);

    return 0;
}

/*
 * 分裂函数：
 * 将一个key值个数达到M-1的结点，分裂成两个小结点，主要供bptree_insert和bptree_insert_nofull调用，防止在插入的过程中，遇到一个满结点，这样会破坏B-Tree的性质
 * parent：将要分裂的结点的父结点
 * index_node：node子结点在parent中的位置
 * node：将要分裂的结点
 * 返回值：
 * -1：失败
 *  0：成功*/
    static int
bptree_split_child(bptree_t *btree, bptree_node_t *parent, int index_node, bptree_node_t *node)
{
    /*申请一个新的结点，用于存放分裂后的部分key*/
    bptree_node_t *node_new = node_create(btree->M);
    if( NULL == node_new )
        return -1;

    /*index_up是将要提升到父结点中的key的下标*/
    int index_up = (node->keynum + 1) / 2;
    /*分裂一个满结点时，分裂出去的key的个数，包括来index_up下标所在的结点*/
    int copy_key_num = node->keynum - index_up;

    /*新的结点与分裂的结点的is_leaf是一样的，在拷贝key值得过程中，也会把相关的child*/
    node_new->is_leaf = node->is_leaf;
    /*需要从node拷贝到node_new中的key的个数*/
    node_new->keynum = copy_key_num;
    /*设置node_new的父结点指向*/
    node_new->parent = parent;

    /*拷贝key值的过程*/
    int j = 0;
    for( j = 0;  j < copy_key_num;  j++) 
    {
        /*这里用index_up+j，是因为index_up后面的key，都是要拷贝到node_new中去的*/
        node_new->key[j] = node->key[index_up + j]; 
        /*因为实际存放的是key值得地址，所以只需要将指针置为NULL，即可从node结点中移除*/
        node->key[index_up + j] = NULL;
    }

    /*判断：如果node不是叶子结点，还需要把相应的子结点的信息，也拷贝到node_new中*/
    if( !node->is_leaf ) 
    {
        for(j = 0; j < copy_key_num; j++) 
        {
            node_new->child[j] = node->child[index_up + j];
            node->child[index_up + j] = NULL;
        }
    }

    /*node中剩下的key的数量*/
    node->keynum = index_up;

    /*
     * 为index_up提升到父结点做准备，腾出空间，存放node_New的地址，
     * 因为node_new是在node的后面，所以在父结点中的位置，也应该是在index_node的下一个结点，所以从index_node+1开始，往后的结点，整体向后移动一位，然后将child[index_node+1]指向node_new
     * */
    for(j = parent->keynum - 1; j >= index_node + 1; j--) 
    {
        parent->key[j + 1] = parent->key[j];
        parent->child[j + 1] = parent->child[j];
    }
    parent->key[index_node + 1] = node_new->key[0];
    parent->child[index_node + 1] = node_new;

    /*父结点的key值数量增加1*/
    parent->keynum++;

    return 0;
}


/*
 * description:
 * 供bptree_insert调用，插入key值，当root不为满结点时或将root分裂后，调用该函数，将value插入到B+Tree中*/
    static int
bptree_insert_nofull(bptree_t *btree, void *value)
{
    bptree_node_t *temp = btree->root;
    int i = 0;

    /*循环，找到合适的插入点，一般插入的结点都为叶子结点*/
    while( 1 )
    {
        i = temp->keynum - 1;
        /*如果temp是叶子结点，则直接找一个位置，将value插入到该结点中*/
        if( temp->is_leaf ) 
        {
            while( i >= 0 && -1 == btree->compareTo(value, temp->key[i]) )
            {
                temp->key[i+1] = temp->key[i];
                --i;
            }

            temp->key[i+1] = value;
            temp->keynum++;
            break;
        }
        else 
        {
            /*如果该结点不是叶子结点，则找到合适的子结点，一直找到合适的叶子结点位置*/
            while( i >= 0 && -1 == btree->compareTo(value, temp->key[i]) )
                --i;
            
            /*
             * 这里是插入时的一个特殊处理：当插入的key值比b+树中所有的key值都小时（因为这套函数在设计b+树时，采用的是内结点为最小索引的方式），沿路下降时，将最小的索引key值替换成value，并将value插入到第一个叶子结点
             * */
            if( -1 == i ) 
            {
                temp->key[0] = value;
                i = 0;
            }

            bptree_node_t *child = temp->child[i];

            /*如果该子结点已满，则分裂该子结点*/
            if( is_full(btree, child) )
            {
                bptree_split_child(btree, temp, i, child);
                if( 1 == btree->compareTo(value, temp->key[i+1]) )
                    ++i;
            }

            /*让temp指向该子结点，循环判断*/
            temp = temp->child[i];
        }
    }

    return 0;
}

/*插入一个key值*/
    int
bptree_insert( bptree_t *btree, void *value )
{
    bptree_node_t *root = btree->root;
    /*如果根结点满了，则申请一个新的根结点，将原根结点作为新根结点的一个子结点，对根进行分裂是增加B树高度的唯一途径*/
    if( is_full(btree, root) )
    {
        /*申请一个新的结点*/
        bptree_node_t *temp = node_create(btree->M);
        if( NULL == temp )
            return -1;

        /*新结点，非叶子结点，key的数量为0，为什么新结点的key值个数为1，这是因为B+Tree的性质6决定的（我自己笔记总结的几个性质）*/
        temp->is_leaf = 0;
        temp->keynum = 1;
        temp->key[0] = root->key[0];
        temp->child[0] = root;

        /*root结点的父指针指向temp*/
        root->parent = temp;

        /*对原根结点进行分裂*/
        if( -1 == bptree_split_child(btree, temp, 0, root) )
        {
            node_destory(&temp);
            return -1;
        }
        /*分裂成功后，让root重新指向新的根结点*/
        btree->root = temp;

        /*进行插入key值*/
        bptree_insert_nofull(btree, value);
    }
    else
    {
        /*如果根结点没有满，则直接调用bptree_insert_nofull()插入*/
        bptree_insert_nofull(btree, value);
    }

    return 0;
}

/*判断x结点的key值个数是否在底线值上，num_ceil为结点内key值的最小个数*/
#define IS_CEIL(x, num_ceil) (NULL != (x) && num_ceil >= (x)->keynum)
/*delete a key*/
    int
bptree_delete( bptree_t *btree, void *value)
{
    bptree_node_t *temp = btree->root;
    int index = 1;
    int num_ceil = get_ceil(btree);
    int ret = -1;

    while( NULL != temp )
    {
        index = 0;
        /*从第一个key值到最后一个key值，与value比较，找到与value相等或合适的位置*/
        while( index < temp->keynum && 0 <= btree->compareTo(value, temp->key[index]) )
            ++index;
        index--;
        /*如果value在当前结点内且当前结点为叶子结点，则进行删除操作*/
        if( temp->is_leaf && index < temp->keynum && 0 == btree->compareTo(value, temp->key[index]) ) 
        {
            //printf("delete 1 : key[0] --> %d, index = %d\n", *((int*)temp->key[0]), index);
            /*
             * 到达相应的结点后，有两种情况：
             * 第一种：删除的key值在叶子结点中的下标大于0。这种情况下，只需要将该key值删除掉，然后后面的key值和child值依次前移即可。
             * 第二种：删除的key值在叶子结点中的下标为0。这种情况下，需要将所有已删除key值为索引的结点都替换成key[1]，将key[1]替换成新的索引key值，然后在叶子结点中，将后续的key值依次前移
             */
            if( 0 == index ) 
            {
                /*处理第二种情况*/
                bptree_node_t *node_parent = temp->parent;
                int index_parent = 0;
                while( NULL != node_parent ) 
                {
                    if( node_parent->key[0] == temp->key[0] )
                    {
                        node_parent->key[0] = temp->key[1];
                    }
                    else 
                    {
                        index_parent = 1;
                        while( index_parent < node_parent->keynum && 0 != btree->compareTo(temp->key[0], node_parent->key[index_parent]) )
                            ++index_parent;
                        node_parent->key[index_parent] = temp->key[1];
                        break;
                    }
                    node_parent = node_parent->parent;
                }    
            }

            /*将删除key值得后续key值和child值依次前移*/
            key_destory(btree, &(temp->key[index]));
            while( index < temp->keynum-1 )
            {
                temp->key[index] = temp->key[index+1];
                index++;
            }
            temp->key[index] = NULL;
            temp->keynum--;
            ret = 0;
            break;
        }

        //printf("delete 2 : key[0] --> %d, index = %d\n", *((int*)temp->key[0]), index);
        /*如果在本结点没有找到要删除的key值，则找到合适的子结点，继续查找*/
        /*
         * 在让temp指向子结点之前，需要保证子结点的keynum>num_ceil，采取的措施有两种（因为相邻的子结点可以是左边的，也可以是右边的，所以每种又可以分别两种，总共4种）
         * 1、合并：如果该子结点与相邻的子结点的keynum都为num_ceil，则将这两个子结点进行合并
         * 2、转移key值，如果该子结点的keynum为num_ceil，而相邻的子结点的keynum都大于num_ceil，则从相邻子结点借一个结点到该子结点，以保证该子结点的keynum大于num_ceil
         * */
        if( 0 <= index && index <= temp->keynum-2 && IS_CEIL(temp->child[index], num_ceil) && IS_CEIL(temp->child[index+1], num_ceil) ) 
        {
            //printf("merge 1 --> index = %d\n", index);
            bptree_merge_child(btree, temp, index+1);
        }
        else if( 1 <= index && index <= temp->keynum-1 && IS_CEIL(temp->child[index-1], num_ceil) && IS_CEIL(temp->child[index],num_ceil) ) 
        {
            //printf("merge 2 --> index = %d\n", index);
            bptree_merge_child(btree, temp, index);
            index--;
        }
        else if( 0 <= index && index <= temp->keynum-2 && IS_CEIL(temp->child[index],num_ceil) && NULL != temp->child[index+1] && num_ceil < temp->child[index+1]->keynum ) 
        {
            //printf("move 3 --> index = %d\n", index);
            bptree_move_right_child(btree, temp, index);
        }
        else if( 1 <= index && index <= temp->keynum-1 && IS_CEIL(temp->child[index],num_ceil) && NULL != temp->child[index-1] && num_ceil < temp->child[index-1]->keynum ) 
        {
            //printf("move 4 --> index = %d\n", index);
            bptree_move_left_child(btree, temp, index);
        }
        temp = temp->child[index];
    }

    if( 0 == ret )
    {
        if( 1 == btree->root->keynum )
        {
            temp = btree->root;
            btree->root = btree->root->child[0];
            node_destory(&temp);
        }
    }

    return ret;
}


/*
 * B树查找
 * 返回值：
 * 0：找到了
 * -1：没有找到
 * */
    int
bptree_search( bptree_t *btree, void *value )
{
    int i = 0;
    bptree_node_t *pstr = btree->root;
    int ret = -1;

    //printf("bptree_search --> value = %d\n", *((int*)value));
    while( NULL != pstr )
    {
        i = 0;
        while( i < pstr->keynum && 1 == btree->compareTo(value, pstr->key[i]) )
        {
            ++i;
        }

        //printf("i = %d, node = %d\n", i, pstr->keynum > 1 ? *((int*)pstr->key[0]) : -1 );
        if( pstr->keynum > 0 && i < pstr->keynum && 0 == btree->compareTo(value, pstr->key[i]) ) 
        {
            //printf("[%s] pstr->key[i] = %d\n", __func__, *((int*)pstr->key[i]));
            //printf("key = %p, value = %p\n", pstr->key[i], value);
            ret = 0;
            break;
        }
        /* 0 == i : 这是为了判断当搜索值value，比b+树中所有的key值都小时，直接返回-1，表示没有该key值*/
        else if( 0 == i || pstr->is_leaf ) 
        {
            //puts("2");
            ret = -1;
            break;
        }
        else 
        {
            //puts("3");
            pstr = pstr->child[i-1];
        }
    }

    return ret;
}


/*create a b_tree root*/
    bptree_t *
bptree_create( compare_t compareTo, delete_t deleteTo )
{
    bptree_t *btree = (bptree_t *)malloc(sizeof(bptree_t));
    if( NULL == btree )
        return NULL;
    memset(btree, 0x00, sizeof(bptree_t));

    btree->M = 4;
    btree->compareTo = compareTo;
    btree->deleteTo = deleteTo;

    bptree_node_t *root = node_create(btree->M);
    if( NULL == root )
        return NULL;

    btree->root = root;

    return btree;
}

/*destory a b_tree*/
    void
bptree_destory(bptree_t **btree)
{
    if( NULL != *btree ) 
    {
        bptree_destory_child(*btree, &((*btree)->root));
        free(*btree);
        *btree = NULL;
    }
}
