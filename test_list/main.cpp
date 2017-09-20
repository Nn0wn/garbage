#include <iostream>

using namespace std;

struct elem{
    elem* next;
    int value;
};

int count3(elem* head);
void modify3(elem* head);

void main()
{
    int i, d;
    elem* head=(elem*)malloc(sizeof(elem));
    elem* tmp=(elem*)malloc(sizeof(elem));
    head->next=tmp;
    tmp->next=NULL;
    tmp->value=-20;
    for(i=-19;i<21;i++)
    {
        elem* tmp2=(elem*)malloc(sizeof(elem));
        tmp->next=tmp2;
        tmp2->next=NULL;
        tmp2->value=i;
        tmp2=tmp;
    }
    while(head){
        printf("%d\n",head->value);
        head=head->next;
    }
    //return 0;
}
