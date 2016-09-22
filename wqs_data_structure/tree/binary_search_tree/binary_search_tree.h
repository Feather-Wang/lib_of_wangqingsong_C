#ifndef __BINARY_SEARCH_TREE_H__

#define __BINARY_SEARCH_TREE_H__

typedef struct node_s node_t;
typedef struct bstree_s bstree_t;

/*
 * return:
 * -1 : va1 < va2
 *  0 : va1 == va2
 *  1 : va1 > va2
 *  */
typedef int (*compare_t)(void *va1, void *va2);
typedef int (*delete_t)(node_t *node);

struct bstree_s
{
    node_t *root;
    compare_t compareTo;
    delete_t deleteTo;
};

struct node_s {
    void *element;
    node_t *parent;
    node_t *lchild;
    node_t *rchild;
};

extern void print_sorted_tree(node_t *node);

extern bstree_t *bstree_create(compare_t compare_f, delete_t delete_f);
extern void bstree_destroy(bstree_t **btree);
extern node_t *bstree_find(bstree_t *btree, void *value);
extern int bstree_delete(bstree_t *btree, node_t *node);
extern node_t *bstree_insert(bstree_t *btree, void *value);
#endif /* end of include guard: __BINARY_SEARCH_TREE_H__ */
