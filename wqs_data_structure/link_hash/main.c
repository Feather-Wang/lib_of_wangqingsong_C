#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkhash.h"

static void str_entry_free( struct lh_entry *e )
{
    free(e->k);
    free((char*)e->v);
}

/*遍历元素*/
void for_list(struct lh_table *hash)
{
    struct lh_entry *entry = NULL;
    struct lh_entry *tmp = NULL;
    lh_foreach_safe(hash, entry, tmp)
    //h_foreach(hash, entry)
    {
        printf("k = %s, v = %s\n", (char*)entry->k, (char*)entry->v);
    }
}

int main(int argc, const char *argv[])
{
    struct lh_table *hash = NULL;
    struct lh_entry *entry = NULL;
    char *pstr = NULL;

    /*创建hash表*/ 
    hash = lh_kchar_table_new(1024, "this is a hash", str_entry_free);
    
    /*插入元素*/
    lh_table_insert(hash, strdup("insert 1"), strdup("insert 1 1"));
    lh_table_insert(hash, strdup("insert 2"), strdup("insert 2 2"));

    /*查找指定名字的元素*/
    if( NULL == (entry = lh_table_lookup_entry(hash, "insert 1")) )
    {
        puts("lh_table_lookup is Error");
        return -1;
    }
    printf("k = %s, v = %s\n", (char*)entry->k, (char*)entry->v);
    if( NULL == (pstr = (char*)lh_table_lookup(hash, "insert 2")) )
    {
        puts("lh_table_lookup is Error");
        return -1;
    }
    printf("pstr = %s\n", pstr);

    /*遍历元素*/
    for_list(hash);

    /*删除指定元素*/
    if( -1 == lh_table_delete_entry(hash, entry) )
    {
        puts("lh_table_delete_entry is Error");
        return -1;
    }
    if( -1 == lh_table_delete(hash, "insert 2") )
    {
        puts("lh_table_delete is Error");
        return -1;
    }

    lh_table_free(hash);

    puts("End.............");

    return 0;
}
