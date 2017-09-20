#include <stdio.h>
#include <stdlib.h>

struct elem{
    struct elem* next;
    int value;
} elem;

int count3(struct elem* head);
struct elem* modify3(struct elem* head);
void count(struct elem* head);

int main()
{
    int i;
    struct elem* head=(struct elem*)malloc(sizeof(struct elem));
    struct elem* tmp=(struct elem*)malloc(sizeof(struct elem));
    head->next=tmp;
    head->value=-27;
    tmp->next=NULL;
    tmp->value=-6;
    for(i=-18;i<21;i++)
    {
        struct elem* tmp2=(struct elem*)malloc(sizeof(struct elem));
        tmp->next=tmp2;
        tmp2->next=NULL;
        tmp2->value=i;
        tmp=tmp2;
    }
    count(head);
    printf("\nNumber of negative numbers is %d\n",count3(head));
    head=modify3(head);
    count(head);

    return 0;
}

void count(struct elem* head)
{
    while(head)
    {
        if(head->value%5==0)
        printf("\n");
        printf("[%d]",head->value);
        head=head->next;

    }
    printf("\n");
}

int count3(struct elem* head)
{
    int k=0;
    while(head){
        if((head->value)<0){
            k++;
        }
        head=head->next;
    }
    return k;
}

struct elem* modify3(struct elem* head)
{
    struct elem* tmp=(struct elem*)malloc(sizeof(struct elem));
    struct elem* tmp2=(struct elem*)malloc(sizeof(struct elem));
    struct elem* tmp3=(struct elem*)malloc(sizeof(struct elem));
    while(head&&((head->value)<0)&&((head->value%2)!=0))
    {
        tmp=head;
        head=head->next;
        tmp->next=NULL;
    }
    tmp3=head;
    tmp=tmp3;
    while(tmp->next)
    {
        if(((tmp->next->value)<0)&&((tmp->next->value%2)!=0))
        {
            tmp2=tmp;
            tmp=tmp->next;
            tmp2->next=NULL;
        }
        else if(tmp->next!=NULL)
        {
            tmp=tmp->next;
            head->next=tmp;
            head=head->next;
        }

    }
    return tmp3;
}
