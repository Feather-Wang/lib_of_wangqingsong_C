
/*
 * Copyrchild (C) Igor Sysoev
 * Copyrchild (C) Nginx, Inc.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "red_black_tree.h"

/*
 * The red-black tree code is based on the algorithm described in
 * the "Introduction to Algorithms" by Cormen, Leiserson and Rivest.
 */
static inline void
rbtree_left_rotate(rbtree_t *btree, rbtree_node_t *node)
{
    rbtree_node_t  *temp = NULL, *sentinel = btree->sentinel;
    //printf("[%s] element = %d\n", __func__, *((int*)node->element));

    /*第一步*/
    temp = node->rchild;
    node->rchild = temp->lchild;

    /*第二步：因为在第一步中temp->lchild被转移到了node的右结点，所以要更新temp->lchild的parent，指向node*/
    if( sentinel != temp->lchild ) 
    {
        temp->lchild->parent = node;
    }

    /*第三步：temp顶替了node的位置，所以需要将parent指向node->parent所指向的结点*/
    temp->parent = node->parent;

    /*
     * 第四步：
     * 1、如果旋转的支撑点为root，则直接让btree->root指向temp，
     * 2、如果node位于父结点的左结点，则让父结点的左结点指向temp，
     * 3、如果1和2都不是，则让node的父结点的右结点指向temp
     * */
    if( node == btree->root ) 
    {
        btree->root = temp;
    } 
    else if( node == node->parent->lchild )
    {
        node->parent->lchild = temp;
    } 
    else 
    {
        node->parent->rchild = temp;
    }

    /*第五步：让temp的左结点指向node，node->parent指向temp*/
    temp->lchild = node;
    node->parent = temp;
}


static inline void
rbtree_right_rotate(rbtree_t *btree, rbtree_node_t *node)
{
    rbtree_node_t  *temp = NULL, *sentinel = btree->sentinel;
    //printf("[%s] element = %d\n", __func__, *((int*)node->element));

    /*第一步*/
    temp = node->lchild;
    node->lchild = temp->rchild;

    /*第二步：因为在第一步中temp->rchild被转移到了node的左结点，所以要更新temp->rchild的parent，指向node*/
    if( sentinel != temp->rchild )
    {
        temp->rchild->parent = node;
    }

    /*第三步：temp顶替了node的位置，所以需要将parent指向node->parent所指向的结点*/
    temp->parent = node->parent;

    /*
     * 第四步：
     * 1、如果旋转的支撑点为root，则直接让btree->root指向temp，
     * 2、如果node位于父结点的右结点，则让父结点的右结点指向temp，
     * 3、如果1和2都不是，则让node的父结点的左结点指向temp
     * */
    if( node == btree->root )
    {
        btree->root = temp;
    }
    else if( node == node->parent->rchild )
    {
        node->parent->rchild = temp;
    }
    else
    {
        node->parent->lchild = temp;
    }

    /*第五步：让temp的右结点指向node，node->parent指向temp*/
    temp->rchild = node;
    node->parent = temp;
}

/*释放结点*/
static int
rbtree_delete_node(rbtree_t *btree, rbtree_node_t **node)
{
    /*如果删除成员有值，先用deleteTo函数删除结点中的element成员*/
    if( btree->deleteTo )
        btree->deleteTo(*node);
    else
        free((*node)->element);
    /*然后再删除结点，并将结点指针置为NULL*/
    free(*node);
    *node = NULL;
    return 0;
}

/**************************************************/

/*
 * description : insert node
 * return : 0(success), -1(error)
 * */
int
rbtree_insert(rbtree_t *btree, void *value)
{
    rbtree_node_t  **root = NULL, *temp = NULL, *node = NULL, *sentinel = btree->sentinel;

    /*创建一个结点，并初始化*/
    node = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
    if( NULL == node ) 
    {
        fprintf(stderr, "[%s][%d] malloc failed\n", __func__, __LINE__);
        return -1;
    }
    /*所以的NIL结点都指向sentinel*/
    node->parent = sentinel;
    node->lchild = sentinel;
    node->rchild = sentinel;
    node->element = value;
    /*新插入的值设置为RED而不设置成BLACK，是因为设置成BLACK会违背性质5，但可能会违背性质2和性质4，这就需要平衡处理了*/
    color_set_red(node);

    /* a binary tree insert */
    root = (rbtree_node_t **) &btree->root;

    /*判断根节点是否为空，是，则让root结点指向node，并将其设置成黑色*/
    if( NULL == *root )
    {
        *root = node;
        color_set_black(node);
        return 0;
    }

    /*insert node*/
    temp = *root;
    rbtree_node_t **p = NULL;
    /*循环，找到空结点，退出循环*/
    while( 1 )
    {
        p = (-1 == btree->compareTo(node->element, temp->element)) ? &(temp->lchild) : &(temp->rchild);
        if( sentinel == *p )
        {
            break;
        }
        temp = *p;
    }

    /*将node插入到找到的空位，并将parent指向父结点*/
    *p = node;
    node->parent = temp;

    /* re-balance tree */
    /*node不为根节点，且node的父结点为红色，违背了性质4*/
    while (node != *root && color_is_red(node->parent)) 
    {
        /*判断：当前结点的父结点，在爷爷结点的位置，如果在左结点，则进入以下处理，如果在右结点，就进入else{}模块处理*/
        if (node->parent == node->parent->parent->lchild) 
        {
            /*当前结点的父结点，在爷爷结点的左结点*/
            /*获取叔父结点*/
            temp = node->parent->parent->rchild;

            /*如果叔父结点是红色，这样只需要通过变换父结点、叔父结点、以及爷爷结点的颜色，即可re-balance*/
            if (color_is_red(temp)) 
            {
                /*将父结点和叔父结点都变为黑色*/
                color_set_black(node->parent);
                color_set_black(temp);
                /*将爷爷结点变为红色*/
                color_set_red(node->parent->parent);
                /*将node指向爷爷结点，进行下一次判断*/
                node = node->parent->parent;
            }
            else 
            {
                /*如果叔父结点不是红色，就需要进行旋转来达到re-balance*/
                /*
                 * 判断：
                 * node结点是否在父结点的右结点，如果是，则表示旋转情况是LR型，否则，旋转情况为LL型
                 * LL型：只需要以爷爷结点进行右旋即可
                 * LR型：分两步，第一步是以父结点进行左旋，将LR型转换成LL型，再以爷爷结点进行右旋
                 * */
                if (node == node->parent->rchild) 
                {
                    /*让node指向父结点，进行左旋*/
                    node = node->parent;
                    rbtree_left_rotate(btree, node);
                }

                /*将父结点设置成黑色，将爷爷结点设置为红色，在进行旋转*/
                color_set_black(node->parent);
                color_set_red(node->parent->parent);
                rbtree_right_rotate(btree, node->parent->parent);
            }

        } 
        else 
        {
            /*当前结点的父结点，在爷爷结点的右结点*/
            /*将temp指向叔父结点*/
            temp = node->parent->parent->lchild;

            /*如果叔父结点是红色，处理同前面*/
            if (color_is_red(temp)) 
            {
                /*将父结点和叔父结点都变为黑色*/
                color_set_black(node->parent);
                color_set_black(temp);
                /*将爷爷结点变为红色*/
                color_set_red(node->parent->parent);
                /*将node指向爷爷结点，进行下一次判断*/
                node = node->parent->parent;
            }
            else
            {
                /*如果叔父结点不是红色，就需要进行旋转来达到re-balance*/
                /*
                 * 判断：
                 * node结点是否在父结点的左结点，如果是，则表示旋转情况是RL型，否则，旋转情况为RR型
                 * RR型：只需要以爷爷结点进行左旋即可
                 * RL型：分两步，第一步是以父结点进行右旋，将RL型转换成RR型，再以爷爷结点进行左旋
                 * */
                if (node == node->parent->lchild) 
                {
                    /*让node指向父结点，进行右旋*/
                    node = node->parent;
                    rbtree_right_rotate(btree, node);
                }

                /*将父结点设置成黑色，将爷爷结点设置为红色，在进行旋转*/
                color_set_black(node->parent);
                color_set_red(node->parent->parent);
                rbtree_left_rotate(btree, node->parent->parent);
            }
        }
    }

    /*将根结点设置为黑色，这是为了满足性质2*/
    color_set_black(*root);

    return 0;
}


/*description : delete node*/
void
rbtree_delete(rbtree_t *btree, void *value)
{
    unsigned    int       red;
    rbtree_node_t  **root = NULL, *subst = NULL, *temp = NULL, *w = NULL, *node = NULL, *sentinel = btree->sentinel;

    /*找到要删除的结点，如果没有找到，打印提示信息，并退出函数*/
    node = rbtree_find(btree, value);
    if( NULL == node )
    {
        fprintf(stderr, "this node is not existd\n");
        return;
    }

    /* a binary tree delete */

    root = (rbtree_node_t **) &btree->root;

    /*
     * 删除的三种情况：
     * 1、删除的结点没有左子树
     * 2、删除的结点没有右子树
     * 3、删除的结点左子树和右子树都存在
     * */
    if( sentinel == node->lchild )
    {
        /*
         * 第一种情况：
         * 因为左子树不存在，只需要处理右子树就可以了，首先让temp指向右子树，subst指向node（这个步骤是为了在后面有一个判断，用来区别第一二种情况和第三种情况的，如果subst==node，说明是第一种和第二种情况，如果subst!=node，说明是第三种情况
         * */
        temp = node->rchild;
        subst = node;
    }
    else if( sentinel == node->rchild )
    {
        /*
         * 第二种情况：
         * 因为右子树不存在，所以只需要处理左子树就可以了，首先让temp指向左子树，sbust指向node
         * */
        temp = node->lchild;
        subst = node;
    }
    else
    {
        /*
         * 第三种情况：
         * 因为左子树和右子树都存在，首先从右子树中找到一个最小的结点，让subst指向它，temp指向subst的子结点
         * */
        subst = rbtree_min(node->rchild, sentinel );

        /*判断：subst的左结点是否为NIL，理论上肯定是NIL，因为如果不为NIL，则还存在比subst小的结点，这是不合理的，因为subst指向的本来就是node右子树中最小的点了*/
        if( sentinel != subst->lchild )
        {
            puts("444");
            temp = subst->lchild;
            exit(1);
        }
        else
        {
            /*如果subst的左结点为NIL，则让temp指向subst的右结点*/
            temp = subst->rchild;
        }
    }

    /*
     * 如果subst是根结点，第一种和第二种情况会发生这种事件：
     * 这种情况下，只需要让root指向残留的temp指向的子树，然后更新temp的parent成员，并重新给根结点上色。
     * 释放掉node结点的内存
     * */
    if( subst == *root )
    {
        *root = temp;
        /*调整root的颜色为黑色*/
        color_set_black(*root);

        /*释放结点*/
        rbtree_delete_node(btree, &node);

        return;
    }

    /*
     * 保存subst的颜色，
     * 因为在实际中，删除的是subst结点，如果是第一种和第二种情况，subst就是node，但是在第三种情况时，subst是另一个结点，在后续的操作中，subst会代替node的位置（node的子结点和颜色都会转移到subst上），所以会变相的删掉subst原来的位置结点，所以在这里保存subst的颜色，以便后续判断是否需要re-balance*/
    red = color_is_red(subst);

    /*
     * 如果结点是在父结点的左侧，则让父结点的左结点指向temp，跳过subst。
     * 个人感觉，这步判断没有意义，与上面那个判断一样，subst是node右子树中最小的，肯定会在父结点的左测
     * */
    if( subst == subst->parent->lchild )
    {
        subst->parent->lchild = temp;
    }
    else
    {
        subst->parent->rchild = temp;
    }

    /*
     * 如果subst和node是同一个结点，第一种情况和第二种情况会出现这种事件。
     * 这种情况下，直接让temp->parent指向subst的父结点，在上个判断中，已经完成了subst的父结点指向temp的操作
     * */
    if( subst == node )
    {
        temp->parent = subst->parent;
    }
    else
    {
        /*如果是第三种情况*/
        if( subst->parent == node )
        {
            /*
             * 1、当node结点是subst的父结点时，让temp指向subst，
             * 其实这步感觉是多余的，temp本来就是sbust的子结点,temp->parent肯定是指向subst的*/
            printf("temp is NIL ? [%s]\n", temp == sentinel ? "YES" : "NO");
            printf("subst = %d, node = %d\n", *((int*)subst->element), *((int*)node->element));
            temp->parent = subst;
        }
        else
        {
            /*2、当node结点不是subst的父结点时，让temp指向subst的父结点，这是因为subst将会被移动到node的位置，所以需要改变temp的指向关系*/
            temp->parent = subst->parent;
        }

        /*将node的子结点都转移到subst下，交换subst和node的颜色*/
        subst->lchild = node->lchild;
        subst->rchild = node->rchild;
        subst->parent = node->parent;
        color_copy(subst, node);

        /*如果node是根结点，则直接让root指向subst*/
        if( node == *root )
        {
            *root = subst;
        }
        else
        {
            /*如果node在父结点的左侧，则让左结点指向subst，否则，让右结点指向subst*/
            if( node == node->parent->lchild )
            {
                node->parent->lchild = subst;
            }
            else
            {
                node->parent->rchild = subst;
            }
        }

        /*因为node的子结点转移到subst下后，需要更新子结点的parent的指向，指向subst*/
        if( sentinel != subst->lchild )
        {
            subst->lchild->parent = subst;
        }

        if( sentinel != subst->rchild )
        {
            subst->rchild->parent = subst;
        }
    }

    /*释放结点*/
    rbtree_delete_node(btree, &node);

    /*判断删除结点的颜色，如果是红色，则不需要re-balance，因为删除红色结点不违背红黑树的性质*/
    if( red )
    {
        return;
    }

    /* keep balance*/
    /*
     * 平衡是从删除结点的下一个结点，
     * 如果是第一种和第二种情况，就是删除节点的左结点或右结点，
     * 如果是第三种情况，就是找到的最小结点的右结点
     * */
    /*
     * 1、当前结点是红色结点
     * 直接把当前结点染成黑色
     *
     * 2、当前结点是根结点
     * 什么都不做
     *
     * 3、当前结点是黑色，兄弟结点是红色
     * 将兄弟结点染成黑色，父亲结点染成红色，以父结点为结点进行旋转（如果当前结点在父结点左侧，则进行左旋，反之，进行右旋）。
     * 重新获取兄弟结点（旋转后，当前结点的兄弟结点发生了变化），然后进行以下情况的判断。
     *
     * 4、当前结点是黑色，兄弟结点也是黑色，且兄弟结点的两个子结点全是黑色
     * 将兄弟结点染成红色，然后将父结点作为新的当前结点，重新进入re-balance
     *
     * 5、当前结点是黑色，兄弟结点也是黑色，且兄弟结点的右结点（如果当前结点是在父亲结点的右侧，则判断兄弟结点的左结点）是黑色
     * 将兄弟结点的左结点（如果当前结点是在父亲结点的右侧，则这里改成右结点）染成黑色。
     * 将兄弟结点染成红色。
     * 以兄弟结点为支点进行右旋（如果当前结点是在父亲结点的右侧，则进行左旋）。
     * 重新获取兄弟结点（旋转后，当前结点的兄弟结点发生了变化），然后进行以下情况的判断。
     *
     * 6、当前结点是黑色，兄弟结点也是黑色，且兄弟结点的两个子结点不全为黑色结点
     * 将兄弟结点染成父结点的颜色。
     * 将父结点染成黑色。
     * 将兄弟结点的右结点（如果当前结点是在父结点的右侧，则改成左结点）染成黑色。
     * 以父结点为支点进行左旋（如果当前结点是在父结点的右侧，则这里进行右旋）。
     * 将当前结点指向根结点，退出re-balance。
     *
     * 退出re-balance后，将当前结点染成黑色（当前结点只有两种情况，一种：当前结点是红色，另一种：当前结点是根结点，这步可以同时解决情况1和性质2）
     * */
    /*循环的条件：temp不是root结点，且temp是黑色*/
    while( temp != *root && color_is_black(temp) )
    {
        /*如果temp在父结点的左侧*/
        if( temp == temp->parent->lchild )
        {
            /*让w指向父结点的右结点*/
            w = temp->parent->rchild;

            /*如果temp的兄弟结点是红色*/
            if( color_is_red(w) )
            {
                /*将兄弟结点设置为黑色，将父结点设置为红色，以父结点进行左旋，并让w指向新的父结点的右结点*/
                color_set_black(w);
                color_set_red(temp->parent);
                rbtree_left_rotate(btree, temp->parent);
                w = temp->parent->rchild;
            }

            /*如果w的左结点和右结点都是黑色，则将w设置为红色，将temp指向父结点*/
            if( color_is_black(w->lchild) && color_is_black(w->rchild) )
            {
                color_set_red(w);
                temp = temp->parent;
            }
            else
            {
                /*如果w的左结点和右结点不全是黑色，则判断w的右结点是否为黑色*/
                if( color_is_black(w->rchild) )
                {
                    /*如果是，则将w的左结点设置为黑色，w设置为红色，以w进行右旋，让w重新指向temp的兄弟结点（因为在旋转的过程中，结点的位置有变化）*/
                    color_set_black(w->lchild);
                    color_set_red(w);
                    rbtree_right_rotate(btree, w);
                    w = temp->parent->rchild;
                }

                /*将w的颜色变为父结点的颜色，并将父结点设置为黑色，将w的右结点设置为黑色，以父结点进行左旋，让temp指向根结点*/
                color_copy(w, temp->parent);
                color_set_black(temp->parent);
                color_set_black(w->rchild);
                rbtree_left_rotate(btree, temp->parent);
                temp = *root;
            }

        }
        else
        {
            /*如果temp在父结点的右侧，则将w指向父结点的左结点*/
            w = temp->parent->lchild;

            /*如果w是红色*/
            if( color_is_red(w) )
            {
                /*将w设置为黑色，将父结点设置为红色，以父结点进行右旋，并将w重新指向父结点的左结点*/
                color_set_black(w);
                color_set_red(temp->parent);
                rbtree_right_rotate(btree, temp->parent);
                w = temp->parent->lchild;
            }

            /*如果w的左结点和右结点都是黑色，则将w设置为红色，将temp指向父结点*/
            if( color_is_black(w->lchild) && color_is_black(w->rchild) )
            {
                color_set_red(w);
                temp = temp->parent;
            }
            else
            {
                /*如果w的左结点和右结点不全是黑色，则判断w的左结点是否为黑色*/
                if( color_is_black(w->lchild) )
                {
                    /*如果是，则将w的右结点设置为黑色，w设置为红色，以w进行左旋，让w重新指向temp的兄弟结点（因为在旋转的过程中，结点的位置有变化）*/
                    color_set_black(w->rchild);
                    color_set_red(w);
                    rbtree_left_rotate(btree, w);
                    w = temp->parent->lchild;
                }

                /*将w的颜色变为父结点的颜色，并将父结点设置为黑色，将w的左结点设置为黑色，以父结点进行左旋，让temp指向根结点*/
                color_copy(w, temp->parent);
                color_set_black(temp->parent);
                color_set_black(w->lchild);
                rbtree_right_rotate(btree, temp->parent);
                temp = *root;
            }
        }
    }

    /*重新给根结点上色*/
    color_set_black(temp);
}

/*
 * search for value
 *
 */
rbtree_node_t *
rbtree_find(rbtree_t *btree, void *value)
{
    if ( NULL == btree->root ) 
        return NULL; 

    rbtree_node_t *temp = btree->root, *sentinel = btree->sentinel;
    int ret = -1;
    while( 0 != ret && sentinel != temp)
    {
        /*compare value and temp->element*/
        ret = btree->compareTo( value, temp->element );
        //printf("[%d] == [%d] ? [%d]\n", *((int*)value), *((int*)temp->element), ret);
        switch(ret)
        {
            case -1:
                temp = temp->lchild;
                break;
            case 0:
                break;
            case 1:
                temp = temp->rchild;
                break;
            default:
                break;
        }
    }

    return ((sentinel == temp) ? NULL : temp);
}

/*create a binary_binary_tree*/
rbtree_t *
rbtree_create(compare_t compareTo, delete_t deleteTo)
{
    rbtree_t *btree = NULL;

    btree = (rbtree_t *)malloc(sizeof(rbtree_t));
    if( NULL == btree )
    {
        return NULL;
    }
    memset( btree, 0x00, sizeof(rbtree_t) );

    btree->sentinel = (rbtree_node_t*)malloc(sizeof(rbtree_node_t));
    if( NULL == btree->sentinel )
    {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    memset( btree->sentinel, 0x00, sizeof(rbtree_node_t));
    btree->sentinel->element = NULL;

    btree->compareTo = compareTo;
    btree->deleteTo = deleteTo;

    return btree;
}

/*destroy the binary_binary_tree*/
void
rbtree_destroy(rbtree_t **btree)
{
    rbtree_node_t *parent = NULL;
    rbtree_node_t *temp = (*btree)->root;
    rbtree_node_t *sentinel = (*btree)->sentinel;
    while( 1 )
    {
        if( NULL != temp && sentinel != temp->lchild )
        {
            temp = temp->lchild;
            continue;
        }

        if( NULL != temp && sentinel != temp->rchild )
        {
            temp = temp->rchild;
            continue;
        }

        if( NULL != temp && temp != (*btree)->root )
        {
            parent = temp->parent;

            if( parent->lchild == temp )
                parent->lchild = sentinel;
            else
                parent->rchild = sentinel;
            /*释放内存*/
            if( NULL != (*btree)->deleteTo )
            {
                (*btree)->deleteTo(temp);
            }
            else
            {
                free(temp->element);
            }
            free(temp);
            temp = parent;
        }
        else
        {
            /*释放内存*/
            if( NULL != (*btree)->deleteTo )
            {
                (*btree)->deleteTo(temp);
            }
            else
            {
                free(temp->element);
            }
            free(temp);
            (*btree)->root = NULL;
            break;
        }
    }
    if( NULL != (*btree)->sentinel )
    {
        free((*btree)->sentinel);
    }
    if( NULL == (*btree)->root ) 
    {
        free(*btree);
        *btree = NULL;
    }
}

int
check_tree(rbtree_t *btree, rbtree_node_t *node, int black_num, int *index)
{
    int index_tmp = 0;
    if( NULL == index )
    {
        index = &index_tmp;
        if( color_is_red(node) )
        {
            printf("root is not black\n");
            return -1;
        }
    }
    if( btree->sentinel != node && color_is_black(node) )
        black_num++;
    if( btree->sentinel != node )
        check_tree(btree, node->lchild, black_num, index);
    if( btree->sentinel == node )
    {
        if( 0 == *index )
            *index = black_num;
        //printf("[%d] num = %d\n", *index, black_num);
        if( *index != black_num )
        {
            printf("[%s] this red_black_tree is wrong, the right high is [%d], there is a wrong high is [%d]! \n", __func__, *index, black_num);
            exit(-1);
        }
        return 0;
    }
    if( btree->sentinel != node )
        check_tree(btree, node->rchild, black_num, index);
}
