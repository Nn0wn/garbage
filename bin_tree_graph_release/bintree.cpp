#include "bintree.h"

BT* RootBT(BT* tree){
    return tree->Root;
}

BT* LeftBT(BT* tree){
    return tree->Left;
}

BT* RightBT(BT* tree){
    return tree->Right;
}

bool NullBT(BT* tree){
    if (tree == nullptr){
        return 1;
    }
    return 0;
}

BT* ConsBT(elem info, BT* Left, BT* Right, BT* Root){
    BT* tree = new BT;
    tree->info = info;
    tree->Root = Root;
    tree->Right = Right;
    tree->Left = Left;
    return tree;
}

int WayLength(BT* tree, int counter, elem point){
    int l=-1, r=-1, min=-1;
    if(tree->info==point)
    {
        counter++;
        return counter;
    }
    if(tree->info!=point)
    {
        counter++;
        if(tree->Left!=nullptr)
        {
            l=WayLength(tree->Left, counter, point);
            if((min==-1||min>l)&&(l!=-1))
                min=l;
        }
        if(tree->Right!=nullptr)
        {
            r=WayLength(tree->Right, counter, point);
            if((min==-1||min>r)&&(r!=-1))
                min=r;
        }
        if((tree->Left==nullptr)&&(tree->Right==nullptr)&&(tree->Root==nullptr))
        {
            return -1;
        }
    }
    if(l!=-1||r!=-1)
    {
        if(l!=-1&&l<r)
            min=l;
        else if(r!=-1)
            min=r;
    }
    return min;
}
