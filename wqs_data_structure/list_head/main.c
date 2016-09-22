/****************** main --> test function *******************************/
#include "list_head.h" 
#include <stdio.h> 
#include <string.h>
#include <malloc.h>

#define MAX_NAME_LEN 32
#define MAX_ID_LEN 10

typedef struct stud
{
    char name[MAX_NAME_LEN];
    char stu_number[MAX_ID_LEN];
    struct list_head list;
}STUD;

STUD *head;

//清空链表
static int clear_list()
{
    struct list_head *pLIST;
    struct list_head *n;
    STUD *entry = NULL;

    list_for_each_safe(pLIST, n, &head->list)
    {
        entry = list_entry(pLIST, STUD, list);
        if( NULL != entry )
        {
            list_del(&entry->list);
            free(entry);
        }
    }

    return 0;
}

int main(void)
{
    STUD *stu_1;
    STUD *stu_2;

    head = (STUD*)malloc(sizeof(STUD)); 
    stu_1 = (STUD*)malloc(sizeof(STUD)); 
    stu_2 = (STUD*)malloc(sizeof(STUD)); 

    INIT_LIST_HEAD(&head->list);
    strcpy(stu_1->name,"lisi");
    strcpy(stu_1->stu_number,"10000000");

    strcpy(stu_2->name,"zhangsan");
    strcpy(stu_2->stu_number,"10000001");
    list_add(&stu_1->list,&head->list);
    list_add(&stu_2->list,&head->list);
    
    //循环遍历
    struct list_head *p = NULL;
    STUD *entry = NULL;
    list_for_each(p,&head->list)
    {
        entry=list_entry(p, STUD,list);
        printf("name: %s\n",entry->name);
        printf("stu_number: %s\n",entry->stu_number);
    }

    //清空链表
    clear_list();

    if( list_empty(&head->list) )
    {
        puts("the list is empty");
    }

    return 0;
}
/**********************************************************************/
