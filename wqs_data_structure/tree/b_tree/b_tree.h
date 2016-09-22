#ifndef __B_TREE_H__

#define __B_TREE_H__

typedef int (*compare_t) (void *va1, void *va2);
typedef int (*delete_t) (void *key);

typedef struct btree_node_s btree_node_t;
typedef struct btree_s btree_t;

struct btree_node_s
{
    unsigned int         keynum;
    int                  is_leaf;
    void               **key;
    struct btree_node_s**child;
};

struct btree_s
{
    struct btree_node_s    *root;
    unsigned int            M;
    compare_t               compareTo;
    delete_t                deleteTo;
};

extern btree_t * btree_create( compare_t compareTo, delete_t deleteTo );
extern void btree_destory(btree_t **btree);
extern int btree_insert( btree_t *btree, void *value );
extern int btree_delete( btree_t *btree, void *value);
extern int btree_search( btree_t *btree, void *value );

#endif /* end of include guard: __B_TREE_H__ */
