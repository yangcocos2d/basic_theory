#include <stdio.h>

typedef struct node
{
    int array_index;
    int node_no;
    //struct node* pre;
    struct node* next;
    int is_occupy;
}node;

//list container.
#define     LIST_VAR_LEN            10
node        list_var[LIST_VAR_LEN];

node* list_malloc(void)
{
    int i = 0;
    for(i = 0;i<LIST_VAR_LEN;i++)
    {
        if(list_var[i].is_occupy == 0)
        {
            list_var[i].is_occupy = 1;
            return &list_var[i];
        }
    }

    return NULL;
}

void list_free(node *n)
{
    if(n!=NULL)
    {
        n->is_occupy = 0;
        n->node_no = -1;
        n->next = NULL;
    }
}

node* list_foreach(node* list_head,int find_no)
{
    node *cur = list_head;
    node *last = NULL;
    node *find = NULL;
    printf("\n");

    while(cur!=NULL)
    {
        printf("[%d %d] -> ",cur->array_index, cur->node_no);

        if(cur->node_no == find_no)
        {
            find = last;
        }
        last = cur;
        cur = cur->next;
    }
    printf("NULL\n");

    return find;
}

void list_insert_after(node *list_head,node *new_node)
{
    node *list_tail = list_head;
    if(list_head != NULL && new_node!= NULL)
    {
        while(list_tail->next != NULL)
        {
            list_tail = list_tail->next;
        }

        list_tail->next = new_node;
    }
}

node* list_del_node(node *nod_pre)
{
    node* del_node = NULL;
    if(nod_pre != NULL)
    {
        del_node = nod_pre->next;
        nod_pre->next = nod_pre->next->next;
    }
    return del_node;
}

void main(void)
{
    node* list_head = NULL;
    node* list_node =NULL;
    int i = 0;

    //array index init.
    for(i=0;i<LIST_VAR_LEN;i++)
    {
        list_var[i].array_index = i;
        list_var[i].node_no = -1;
    }

    //create list head node.
    list_head = list_malloc();
    list_head->next = NULL;
    list_head->node_no = 0;

    while(1)
    {
        char select = 0;

        printf("input select:");
        select = getch();
        //add node.
        if(select == 'a')
        {    int add_no = 0;
            printf("add");
            //add node.
            printf("\ninput node for addition:");
            scanf("%d",&add_no);
            printf("\npre\n");
            list_node = list_foreach(list_head,add_no);
            list_node = list_malloc();
            list_node->node_no = add_no;
            list_node->next = NULL;
            list_insert_after(list_head, list_node);
            printf("\nnow\n");
            list_node = list_foreach(list_head,add_no);
        }
        //delete node.
        if(select == 'd')
        {    int del_no = 0;
            printf("del");
            //delete node
            printf("\ninput node for delete:");
            scanf("%d",&del_no);
            printf("\npre\n");
            list_node = list_foreach(list_head,del_no);
            list_node = list_del_node(list_node);
            list_free(list_node);
            printf("\nnow\n");
            list_node = list_foreach(list_head,del_no);
        }
        //print list var information.
        if(select == 'p')
        {
            printf("\nlist_var info");
            for(i=0;i<LIST_VAR_LEN;i++)
            {
                printf("\nindex = %d    node_no = %d    is_occupy = %d",list_var[i].array_index,list_var[i].node_no, list_var[i].is_occupy);
            }
            printf("\n");
        }
    }
}
