#ifndef BINTREE_H
#define BINTREE_H
#include <QChar>
#include "node.h"

typedef QString elem;

typedef struct INCTR
{
    struct INCTR* prev=NULL;
    elem name;
    int level=0;
    int turn=0;
    int weight=1;
    int weight_cur=0;
    bool grey=false;
    struct INCTR* next=NULL;
    Node* node=nullptr;
}INCTR;

INCTR *delete_tree(INCTR* tree);

#endif // BINTREE_H
