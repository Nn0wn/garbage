#ifndef BINTREE_H
#define BINTREE_H
#include <QChar>
#include "node.h"

typedef QString elem;

typedef struct BT{
    struct BT* Root;
    struct BT* Left;
    struct BT* Right;
    elem info;
    int key;
    int size;
    Node* node;
    BT(int k, BT* root, elem inf) {key=k; Root=root; info=inf; Left=Right=nullptr; size=1;}
}BT;

BT* RootBT(BT* tree);
BT* LeftBT(BT* tree);
BT* RightBT(BT* tree);
bool NullBT(BT* tree);
//BT* ConsBT(elem info, BT* Left, BT* Right, BT* Root);
//int WayLength(BT* tree, int counter, elem point);
int getsize(BT* tree);
int fixsize(BT* tree);
BT* rotateright(BT* tree);
BT* rotateleft(BT* tree);
BT* insertroot(BT* tree, BT* root, int key, QString key_ch);
BT* insertrand(BT* tree, BT* root, int key, QString key_ch);
BT* Find(BT* tree, int key);
BT *delete_tree(BT* tree);
void fixRoots(BT* tree, BT* past);
BT* merge(BT* tree1, BT* tree2);
BT* remove(BT* tree, int key);
//BT* delet(BT* tree, int key);


#endif // BINTREE_H
