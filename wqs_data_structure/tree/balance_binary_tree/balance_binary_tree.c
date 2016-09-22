#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "balance_binary_tree.h"

/*
 * search for minimum value
 * traverse lchild
 */
static node_t *find_min(node_t *node)
{
    node_t *tmp;
    tmp = node;

    if ( NULL == tmp ) 
        return NULL;
    
    /*while循环，直到找到node树中最小的一个节点*/
    while( NULL != tmp->lchild ) 
    {
        tmp = tmp->lchild;
    }

    return tmp;
}

/*
 * search for maximum value
 * traverse rchild
 */
static node_t *find_max(node_t *node)
{
    node_t *tmp;
    tmp = node;
    
    if ( NULL == tmp ) 
        return NULL;
    
    /*while循环，直到找到node树中最大的一个节点*/
    while( NULL != tmp->rchild ) 
    {
        tmp = tmp->rchild;
    }

    return tmp;
}

/*该函数以node为节点，进行左向旋转,
 * node为二级指针是必要的，否则函数的最后一句话*node=tmp将不起作用,
 * 函数中的三步可在纸上画出实际的小例子，就可以清晰明了了*/
static int left_rotate(node_t **node)
{
    //printf("left_rotate...\n");
    node_t *root = *node;
    node_t *tmp = root->rchild;

    /*第一步*/
    root->rchild = tmp->lchild;
    tmp->lchild = root;

    /*第二步，更新高度*/
    (*node)->bf = 0;
    tmp->bf = 0;

    /*第三步*/
    *node = tmp;
    
    return 0;
}

/*该函数以node为节点，进行右向旋转，
 * node为二级指针是必要的，否则函数的最后一句话*node=tmp将不起作用*/
static int right_rotate(node_t **node)
{
    //printf("right_rotate...\n");
    node_t *root = *node;
    node_t *tmp = root->lchild;

    /*第一步*/
    root->lchild = tmp->rchild;
    tmp->rchild = root;

    /*第二步，更新高度*/
    (*node)->bf = 0;
    tmp->bf = 0;

    /*第三步*/
    *node = tmp;
    
    return 0;
}

enum
{
    LL = 11,
    LR = 9,
    RR = -11,
    RL = -9
};
/*node值为最小不平衡二叉树的根节点*/
static int keep_balance(node_t **node)
{
    /*表示二叉树不平衡的类型，后面会根据其他方式判断，然后给该变量赋值*/
    int direction = 0;

    node_t *tmp = NULL;
    /*首先判断高度差是否为2，这说明是左子树过高，需要以node为节点进行向右旋转，保持平衡，
     * 如果不是2，则判断是否为-2，这说明是右子树过高，需要已node为节点进行向左旋转，保持平衡*/
    if( 2 == (*node)->bf )
    {
        tmp = (*node)->lchild;
        /*如果是左子树过高，接着判断node的左子树的高度值是1还是-1，
         * 如果是-1，说明node的左子树的右子树高，这就是其中的一种特殊情况LR，这种情况需要进行两次旋转，首先以node的lchild节点为根节点，进行一次左向旋转，然后再以node为根节点，进行一次右向旋转即可，
         * 如果是1，说明node的左子树中，依然还是左子树高，这就是普通情况LL，这种情况下只需要以node为节点，进行一次右向旋转即可*/
        if( -1 == tmp->bf ) 
        {
            direction = LR;
        }
        else if( 1 == tmp->bf ) 
        {
            direction = LL;
        }
    }
    else if( -2 == (*node)->bf ) 
    {
        tmp = (*node)->rchild;
        /*如果是右子树过高，借着判断node的右子树的高度值是1或-1，
         * 如果是1，说明node的右子树的左子树高，这就是其中的一种特殊情况RL，这种情况与LR一样，也需要进行两次旋转，首先以node的rchild节点为根节点进行一次右向旋转，然后再以node为根节点，进行一次左向旋转，
         * 如果是-1，说明node的右子树中，依然还是右子树高，这就是普通情况RR，这种情况下只需要以node为节点，进行一次左向旋转即可*/
        if( 1 == tmp->bf )
        {
            direction = RL;
        }
        else if( -1 == tmp->bf )
        {
            direction = RR;
        }
    }
    //printf("keep_balance...direction = %d\n", direction);

    /*判断不平衡的四种情况，根据上面的描述，进行相应的旋转*/
    switch(direction)
    {
        case LR:
            left_rotate(&((*node)->lchild));
        case LL:
            right_rotate(node);
            break;
        case RL:
            right_rotate(&((*node)->rchild));
        case RR:
            left_rotate(node);
            break;
    }
    return 0;
}

/***************************************************************/

/*插入节点
 * 返回值：表示两个意思，第一个意思是插入成功或者失败，第二个意思是高度是否发生了变化
 * 0：插入成功，高度没有发生变化
 * 1：插入成功，高度发生了变化
 * -55：插入失败（内存分配失败）*/
int bbtree_insert(bbtree_t *btree, node_t **node, void *value)
{
    int ret = 0;
    if( NULL == (*node) )
    {
        *node = (node_t *)malloc(sizeof(node_t));
        if( NULL == *node )
        {
            fprintf(stderr, "[%s] bbtree_insert failed\n", __func__);
            return -55;
        }
        (*node)->element = value;
        (*node)->bf = 0;
        (*node)->lchild = NULL;
        (*node)->rchild = NULL;
        /*这里可以返回1或-1，但不能返回其他的值，因为插入的该节点要通过返回值告诉父节点，高度发生了变化，而父节点是根据返回值是否为1或-1，来判断高度产生了变化的*/
        return 1;
    }
    else if( -1 == btree->compareTo(value, (*node)->element) ) 
    {
        ret = bbtree_insert(btree, &((*node)->lchild), value);
        /*查看子节点高度是否产生了变化，
         * 如果没有发生变化，则直接返回0，这个判断主要是因为当消除不平衡后返回0，如果不加该判断，上层节点的高度有可能是-1或1，在下边返回的主要是该节点的bf，就可能再次发生高度变化，产生错误，所以该判断主要是消除错误的高度变化，
         * 如果产生了变化，该节点也跟着改变，又因为子节点位于该节点的左边，所以不管怎么变化，只要高度改变了，该节点都是左子树的高度增加了，所以要加上1，表示左子树增高了
         * 如果发生了错误，则直接返回错误值*/
        if( 0 == ret || -55 == ret ) 
            return ret;
        if( 1 == ret || -1 == ret )
            (*node)->bf += 1;

        switch((*node)->bf)
        {
            /*左子树高度产生变化时，高度取值只能有0和1，这时只要返回它的高度值即可，其他值为失去平衡，需要执行回复平衡函数*/
            case 0:
            case 1:
                return (*node)->bf;
            default:
                //printf("keep_balance...[element]:%d, [bf]:%d\n", *((int*)(*node)->element), (*node)->bf);
                keep_balance(node);
                /*这里返回0的原因为，keep_balance()函数已经将不平衡的部分归于平衡，所以更上层的节点高度不用产生变化了，所以返回0*/
                return 0;
        }
    }
    else if( 1 == btree->compareTo(value, (*node)->element) ) 
    {
        ret = bbtree_insert(btree, &((*node)->rchild), value);
        /*与左子树的变化一样，
         * 如果没有发生变化，则直接返回0,这个判断主要是因为当消除不平衡后返回0，如果不加该判断，上层节点的高度有可能是-1或1,在下边返回的主要是该节点的bf，就可能再次发生高度变化，产生错误，所以该判断主要是消除错误的高度变化，
         * 只要右子树的高度发生了变化，该节点的高度都应该-1，表示右子树增高了*/
        if( 0 == ret || -55 == ret ) 
            return ret;
        if( 1 == ret || -1 == ret )
            (*node)->bf -= 1;

        switch((*node)->bf)
        {
            /*左子树高度产生变化时，高度取值只能有0和-1，这时只要返回它的高度值即可，其他值为失去平衡，需要执行回复平衡函数*/
            case 0:
            case -1:
                return (*node)->bf;
            default:
                //printf("keep_balance...[data]:%d, [bf]:%d\n", *((int*)(*node)->element), (*node)->bf);
                keep_balance(node);
                /*这里返回0的原因为，keep_balance()函数已经将不平衡的部分归于平衡，所以更上层的节点高度不用产生变化了，所以返回0*/
                return 0;
        }
    }
}

/*删除节点*/
int bbtree_delete(bbtree_t *btree, node_t **node, void *value)
{
    int ret = 0;
    /*找到该节点后，销毁内存，并将*node置为NULL*/
    if( 0 == btree->compareTo(value, (*node)->element) ) 
    {
        node_t *tmp = *node;
        /*判断左子树和右子树是否都有*/
        if( NULL != tmp->lchild && NULL != tmp->rchild ) 
        {
            /*找到删除节点的左子树中最大的一个节点tmp_max，然后交换tmp_max和*node保存的值，然后在node的左子树种查找调换的值，在进行删除*/
            node_t *tmp_max = find_max(tmp->lchild);
            void *tmp_data = tmp->element;
            tmp->element = tmp_max->element;
            tmp_max->element = tmp_data;
            int ret = bbtree_delete(btree, &((*node)->lchild), value);
            /*判断返回值，查看高度是否发生了变化，然后进行处理*/
            if( 0 == ret )
                return 0;
            if( 1 == ret || -1 == ret )
                (*node)->bf -= 1;
            switch((*node)->bf)
            {
                case 0:
                    return 1;
                case -1:
                case 1:
                    return 0;
                default:
                    //printf("keep_balance...[element]:%d, [bf]:%d\n", *((int*)(*node)->element), (*node)->bf);
                    keep_balance(node);
                    return 0;
            }
        }
        else 
        {
            /*如果只有右子树，只需要让*node=tmp->rchild即可，这样就可以跳过删除的节点*/
            if( NULL != tmp->rchild )
            {
                *node = tmp->rchild; 
            }
            /*如果只有左子树，只需要让*node=tmp->lchild即可，这样就可以跳过删除的节点*/
            else if( NULL != tmp->lchild ) 
            {
                *node = tmp->lchild;
            }
            else 
            {
                *node = NULL;
            }

            /*释放内存*/
            if( NULL != btree->deleteTo )
            {
                btree->deleteTo(tmp);
            }
            else
            {
                free(tmp->element);
            }
            free(tmp);
            /*返回1，表示高度发生了变化*/
            return 1;
        }
    }
    else if( -1 == btree->compareTo(value, (*node)->element) ) 
    {
        ret = bbtree_delete(btree, &((*node)->lchild), value);
        /*0==ret判断的用意与节点插入函数中的用意一样，说明请看插入函数中的说明*/
        if( 0 == ret ) 
            return 0;
        /*当产生变化后，高度-1，在插入中是+1，这是因为则是左子树中发生了高度变化，因为是删除，只能是高度降低，所以是-1*/
        if( 1 == ret || -1 == ret )
            (*node)->bf -= 1;

        /*高度变化判断，
         * 如果是0，说明之前有左子树或者右子树，而现在没有了，所以高度产生了变化，需要返回1（也可以是-1），
         * 如果是1或-1，说明之前左子树的高度和右子树的高度相等，删除节点后，高度还是在平衡范围内，不需要调整平衡，所以返回0，
         * 如果走到default，则说明该节点为最小不平衡二叉树，需要恢复平衡*/
        switch((*node)->bf)
        {
            case 0:
                return 1;
            case -1:
            case 1:
                return 0;
            default:
                //printf("keep_balance...[element]:%d, [bf]:%d\n", *((int*)(*node)->element), (*node)->bf);
                keep_balance(node);
                /*这里返回0的原因为，keep_balance()函数已经将不平衡的部分归于平衡，所以更上层的节点高度不用产生变化了，所以返回0*/
                return 0;
        }
    }
    else if( 1 == btree->compareTo(value, (*node)->element) ) 
    {
        ret = bbtree_delete(btree, &((*node)->rchild), value);
        /*这两个if的判断的用途，请见左分支部分的说明，情况一样*/
        if( 0 == ret )
            return 0;
        if( 1 == ret || -1 == ret )
            (*node)->bf += 1;

        /*该switch说明，请见左分支部分，情况一样*/
        switch((*node)->bf)
        {
            case 0:
                return 1;
            case 1:
            case -1:
                return 0;
            default:
                //printf("keep_balance...[element]:%d, [bf]:%d\n", *((int*)(*node)->element), (*node)->bf);
                keep_balance(node);
                /*这里返回0的原因为，keep_balance()函数已经将不平衡的部分归于平衡，所以更上层的节点高度不用产生变化了，所以返回0*/
                return 0;
        }
       
    }
}

/*
 * search for value
 *
 */
node_t *bbtree_find(bbtree_t *btree, void *value) 
{
    if ( NULL == btree->root ) 
        return NULL; 

    node_t *tmp = btree->root;
    int ret = -1;
    while( 0 != ret && NULL != tmp)
    {
        /*compare value and tmp->element*/
        ret = btree->compareTo( value, tmp->element );
        
        switch(ret)
        {
            case -1:
                tmp = tmp->lchild;
                break;
            case 0:
                break;
            case 1:
                tmp = tmp->rchild;
                break;
            default:
                break;
        }
    }

    return tmp;
}

/*create a binary_binary_tree*/
bbtree_t *bbtree_create(compare_t compareTo, delete_t deleteTo)
{
    bbtree_t *btree = NULL;
    btree = (bbtree_t *)malloc(sizeof(bbtree_t));
    if( NULL == btree )
    {
        return NULL;
    }
    memset( btree, 0x00, sizeof(bbtree_t) );
    btree->compareTo = compareTo;
    btree->deleteTo = deleteTo;

    return btree;
}

/*destroy the binary_binary_tree*/
void bbtree_destroy(bbtree_t **btree, node_t **node)
{
    if( NULL != *node && NULL != (*node)->lchild )
    {
        bbtree_destroy(btree, &((*node)->lchild));
    }

    if( NULL != *node && NULL != (*node)->rchild )
    {
        bbtree_destroy(btree, &((*node)->rchild));
    }

    if( NULL != *node )
    {
        /*释放内存*/
        if( NULL != (*btree)->deleteTo )
        {
            (*btree)->deleteTo(*node);
        }
        else
        {
            free((*node)->element);
        }
        free(*node);
        *node = NULL;
    }

    if( NULL == (*btree)->root ) 
    {
        free(*btree);
        *btree = NULL;
    }
}
