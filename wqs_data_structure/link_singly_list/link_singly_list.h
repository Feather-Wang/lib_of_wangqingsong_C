#ifndef __LINK_SINGLY_LIST_H__
#define __LINK_SINGLY_LIST_H__

typedef struct node
{
    void *data;    // 存放的数据
    struct node *next;// 指向下一个节点的指针

}listnode;

#define linklist_foreach(H, entry)\
    for(entry = (H)->next; NULL != entry; entry = entry->next)

extern listnode* list_create();                             // create link list, fail return NULL, success return listnode point.
extern void list_destroy(listnode *H);

extern int list_empty(listnode *H);                         // judge whether the link list H is empty. when H is empty, return 1, H is not empty, return 0.

extern int list_locate(listnode *H, void *data, listnode **entry);            // to judge whether the link list H contains data. the H contains data, return 0, otherwise return -1.
extern int list_insert(listnode *H, void *data);       // insert data to the header of the link list H. fail return -1, success return 0.
extern int list_order_insert(listnode *H, void *data);      // insert data to link list H at specified location with > data and < data. fail return -1 and success return 0.
extern int list_delete_entry(listnode *H, listnode *entry);               // delete node form link list H. fail return -1 and success return 0.
extern int list_delete(listnode *H, void *entry);
extern int list_reverse(listnode *H);                       // reverse the link list H of order. success return 0.

#endif
