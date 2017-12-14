#ifndef BINTREE_H
#define BINTREE_H
#include <QChar>
#include "node.h"

typedef QChar elem;

typedef struct BT{
    struct BT* Root;
    struct BT* Left;
    struct BT* Right;
    elem info;
    Node* node;
}BT;

BT* RootBT(BT* tree);
BT* LeftBT(BT* tree);
BT* RightBT(BT* tree);
bool NullBT(BT* tree);
BT* ConsBT(elem info, BT* Left, BT* Right, BT* Root);
int WayLength(BT* tree, int counter, elem point);

#endif // BINTREE_H
