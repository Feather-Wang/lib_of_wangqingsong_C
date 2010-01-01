#ifndef __B_TREE_H__

#define __B_TREE_H__

typedef int (*compare_t) (void *va1, void *va2);
typedef int (*delete_t) (void *key);

typedef struct bptree_node_s bptree_node_t;
typedef struct bptree_s bptree_t;

struct bptree_node_s
{
    unsigned int           keynum;
    int                    is_leaf;
    struct bptree_node_s  *parent;
    void                 **key;
    struct bptree_node_s **child;
};

struct bptree_s
{
    struct bptree_node_s    *root;
    unsigned int            M;
    compare_t               compareTo;
    delete_t                deleteTo;
};

extern bptree_t * bptree_create( compare_t compareTo, delete_t deleteTo );
extern void bptree_destory(bptree_t **btree);
extern int bptree_insert( bptree_t *btree, void *value );
extern int bptree_delete( bptree_t *btree, void *value);
extern int bptree_search( bptree_t *btree, void *value );

#endif /* end of include guard: __B_TREE_H__ */
