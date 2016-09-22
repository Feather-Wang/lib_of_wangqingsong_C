/* binary search tree */
#include "binary_search_tree.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/* 
 * print values of the tree in sorted order
 */
void print_sorted_tree(node_t *node)
{
    if ( NULL == node ) 
        return;

    print_sorted_tree(node->lchild);
    
    //printf("%d \n", *((int*)(node->element)));
    
    print_sorted_tree(node->rchild);
}


//=============================================

/*
 * node is root?
 */
static int is_root(node_t *node)
{
    return ( NULL == node->parent );
}

/*
 * node is leaf?
 */
static int is_leaf(node_t *node)
{
    return (NULL == node->lchild && NULL == node->rchild );
}

/* 
 * if an node is a leaf, 
 * then it could be removed with no side effect.
 */
static int delete_leaf(bstree_t *btree, node_t *node)
{
    if( NULL != node->lchild ) 
    {
        node->parent->rchild = node->lchild;
        node->lchild->parent = node->parent;
    }
    else
    {
        if( -1 == btree->compareTo(node->element, node->parent->element) )
            node->parent->lchild = NULL;
        else if( 1 == btree->compareTo(node->element, node->parent->element) )
            node->parent->rchild = NULL;
    }
    
    if( NULL != btree->deleteTo )
    {
        btree->deleteTo(node);
    }
    else
    {
        free(node->element);
    }
    free(node);

    return 0;
}

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

/********************************************************************************************/

/*
 * search for value
 *
 */
node_t *bstree_find(bstree_t *btree, void *value) 
{
    if ( NULL == btree->root ) 
        return NULL; 

    node_t *tmp = btree->root;
    int ret = -1;
    while( 0 != ret && NULL != tmp)
    {
        /*compare value and tmp->element*/
        ret = btree->compareTo( value, tmp->element);
        
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

/* 
 * delete node from tree
 */
int bstree_delete(bstree_t *btree, node_t *node)
{
    int ret = -1;

    do {
        /*if node == root, and root hasn't lchild*/
        if( btree->root == node && NULL == btree->root->lchild )
        {
            free(btree->root);
            if( NULL != btree->root->rchild )
            {
                btree->root = btree->root->rchild;
                btree->root->parent = NULL;
            }
            else
            {
                btree->root = NULL;
            }
        }
        else
        {
            node_t *replace = find_max(node->lchild);
            if( NULL == replace )
            {
                delete_leaf(btree, node);
            }
            else
            {
                node->element = replace->element;
                delete_leaf(btree, replace);
            }
        }

        ret = 0;

    } while (0);

    return ret;
}

/* 
 * insert a value into the tree
 * return root address of the tree
 */
node_t *bstree_insert(bstree_t *btree, void *value)
{
    node_t *node = NULL;

    do{
        /*查找二叉搜索树中是否存在该元素*/
        node = bstree_find(btree, value);
        if( NULL != node )
            break;

        /* get node malloc */
        node = (node_t *)malloc(sizeof(node_t));
        if( NULL == node )
        {
            break;
        }

        /*set node element*/
        node->element = value;
        node->parent  = NULL;
        node->lchild  = NULL;
        node->rchild  = NULL;

        /*if root is null, set root = node*/
        if ( NULL == btree->root ) 
        {
            btree->root = node;
            break;
        }
        
        node_t *tmp = btree->root;
        int ret = 0;
        while(1)
        {
            /* compare node and tmp, is node < tmp, and tmp->lchild, set tmp->lchild = node, otherwise tmp = tmp->lchild*/

            ret = btree->compareTo( node->element, tmp->element);
            if( -1 == ret )
            {
                if ( NULL == tmp->lchild ) 
                {
                    /* then root->lchild is the proper place */
                    tmp->lchild = node;
                    node->parent = tmp;
                    break;
                }
                else 
                {
                    tmp = tmp->lchild;
                }
            }
            else if( 1 == ret ) 
            {
                if ( NULL == tmp->rchild ) 
                {
                    tmp->rchild = node;
                    node->parent = tmp;
                    break;
                }
                else 
                {
                    tmp = tmp->rchild;
                }
            }
        }

    }while(0);

    return node;
}

/*create a binary_search_tree*/
bstree_t *bstree_create(compare_t compareTo, delete_t deleteTo)
{
    bstree_t *btree = NULL;
    btree = (bstree_t *)malloc(sizeof(bstree_t));
    if( NULL == btree )
    {
        return NULL;
    }
    memset( btree, 0x00, sizeof(bstree_t) );
    btree->compareTo = compareTo;
    btree->deleteTo = deleteTo;

    return btree;
}

/*destroy the binary_search_tree*/
void bstree_destroy(bstree_t **btree)
{
    bstree_t *tmp_tree = *btree;
    node_t *node = NULL, *tmp = NULL;
    int ret = -1;
    do{
        /*this tree is only one node , it's root*/
        if(is_leaf(tmp_tree->root))
        {
            if (tmp_tree->deleteTo) 
            {
                tmp_tree->deleteTo(tmp_tree->root);
            }
            else
            {
                free(tmp_tree->root->element);
            }
            tmp_tree->root = NULL;

            ret = 0;
            break;
        }
        /*this tree has zero node*/
        else if ( NULL == tmp_tree->root) 
        {
            ret = 0;
            break;
        }

        /*get a smaller node*/
        node = tmp_tree->root->lchild != NULL ? find_min(tmp_tree->root->lchild) : find_min(tmp_tree->root->rchild);
        /*最后处理的一个节点为root，所以判断root是否为NULL，即可退出循环*/
        while (tmp_tree->root) 
        {
            /*node is leaf, then delete node*/
            if( is_leaf(node) )
            {
                /*static int i = 0;printf("leaf [%d] --> %d\n", i++, *((int*)node->element));*/
                /*delete the element*/
                if (tmp_tree->deleteTo) 
                {
                    tmp_tree->deleteTo(node);
                }
                else
                {
                    free(node->element);
                }

                tmp = node;
                /*if node is root, turn root = NULL, otherwise, node = node->parent, and set lchild or rchild = NULL*/
                if( tmp != tmp_tree->root )
                {
                    node = node->parent;
                    if( node->lchild == tmp )
                    {
                        node->lchild = NULL;
                    }
                    else if( node->rchild == tmp )
                    {
                        node->rchild = NULL;
                    }
                }
                else
                {
                    tmp_tree->root = NULL;
                }
                free(tmp);
            }
            /*if node has left child, node = node->lchild*/
            else if ( NULL != node->lchild ) 
            {
                node = node->lchild;
            }
            /*if node has right child, node = node->rchild*/
            else if ( NULL != node->rchild ) 
            {
                node = node->rchild;
            }
        }

        ret = 0;

    }while(0);

    if( NULL != *btree )
    {
        free(*btree);
        *btree = NULL;
    }
}
