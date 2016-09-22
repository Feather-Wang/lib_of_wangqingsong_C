
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _RED_BLACK_TREE_H__
#define _RED_BLACK_TREE_H__


typedef struct rbtree_node_s  rbtree_node_t;

typedef int (*compare_t) (void *va1, void *va2);
typedef int (*delete_t) (rbtree_node_t *node);

struct rbtree_node_s {
    void              *element;
    rbtree_node_t     *lchild;
    rbtree_node_t     *rchild;
    rbtree_node_t     *parent;
    char             color;
};


typedef struct rbtree_s  rbtree_t;

struct rbtree_s {
    rbtree_node_t     *root;
    rbtree_node_t     *sentinel;
    compare_t          compareTo;
    delete_t           deleteTo;
};


#define color_set_red(node)               ((node)->color = 1)
#define color_set_black(node)             ((node)->color = 0)
#define color_is_red(node)            ((node)->color)
#define color_is_black(node)          (!color_is_red(node))
#define color_copy(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

static inline rbtree_node_t *
rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while( sentinel != node->lchild )
    {
        node = node->lchild;
    }

    return node;
}

extern int rbtree_insert(rbtree_t *btree, void *value);
extern void rbtree_delete(rbtree_t *btree, void *value);
extern rbtree_node_t *rbtree_find(rbtree_t *btree, void *value);
extern rbtree_t *rbtree_create(compare_t compareTo, delete_t deleteTo);
extern void rbtree_destroy(rbtree_t **btree);
extern int check_tree(rbtree_t *btree, rbtree_node_t *node, int black_num, int *index);

#endif /* _RED_BLACK_TREE_H__ */
