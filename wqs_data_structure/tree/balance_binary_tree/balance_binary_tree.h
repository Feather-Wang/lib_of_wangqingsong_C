#ifndef __BALANCE_BINARY_TREE_H__

#define __BALANCE_BINARY_TREE_H__

typedef struct node_s node_t;
typedef struct bbtree_s bbtree_t;

typedef int (*compare_t)(void *va1, void *va2);
typedef int (*delete_t)(node_t *node);

struct node_s
{
    void *element;
    int bf;
    struct node_s *lchild;
    struct node_s *rchild;
};

struct bbtree_s
{
    node_t *root;
    compare_t compareTo;
    delete_t deleteTo;
};

extern bbtree_t *bbtree_create(compare_t compareTo, delete_t deleteTo);
extern void bbtree_destroy(bbtree_t **btree, node_t **node);
extern node_t *bbtree_find(bbtree_t *btree, void *value);
extern int bbtree_delete(bbtree_t *btree, node_t **node, void *value);
extern int bbtree_insert(bbtree_t *btree, node_t **node, void *value);

#endif
