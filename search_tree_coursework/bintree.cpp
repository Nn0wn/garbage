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

void fixRoots(BT* tree, BT* past)
{
    if (!NullBT(past)){
        tree->Root=past;
    }
    if (!NullBT(LeftBT(tree))){
        fixRoots(LeftBT(tree), tree);
    }
    if (!NullBT(RightBT(tree))){
        fixRoots(RightBT(tree), tree);
    }
}

BT* delete_tree(BT* tree)
{
    if(!tree)
        return tree;
    if(tree->Left)
        delete_tree(tree->Left);
    if(tree->Right)
        delete_tree(tree->Right);
    tree=nullptr;
    delete tree;
    return tree;
}

//BT* ConsBT(elem info, BT* Left, BT* Right, BT* Root){
//    BT* tree = new BT;
//    tree->info = info;
//    tree->Root = Root;
//    tree->Right = Right;
//    tree->Left = Left;
//    return tree;
//}

//int WayLength(BT* tree, int counter, elem point){
//    int l=-1, r=-1, min=-1;
//    if(tree->info==point)
//    {
//        counter++;
//        return counter;
//    }
//    if(tree->info!=point)
//    {
//        counter++;
//        if(tree->Left!=nullptr)
//        {
//            l=WayLength(tree->Left, counter, point);
//            if((min==-1||min>l)&&(l!=-1))
//                min=l;
//        }
//        if(tree->Right!=nullptr)
//        {
//            r=WayLength(tree->Right, counter, point);
//            if((min==-1||min>r)&&(r!=-1))
//                min=r;
//        }
//        if((tree->Left==nullptr)&&(tree->Right==nullptr)&&(tree->Root==nullptr))
//        {
//            return -1;
//        }
//    }
//    if(l!=-1||r!=-1)
//    {
//        if(l!=-1&&l<r)
//            min=l;
//        else if(r!=-1)
//            min=r;
//    }
//    return min;
//}

int getsize(BT* tree)
{
    if(!tree)
        return 0;
    return tree->size;
}

int fixsize(BT* tree)
{
    tree->size=getsize(tree->Left)+getsize(tree->Right)+1;
}

BT* rotateright(BT* tree)
{
    BT* child=tree->Left;
    if(!child)
        return tree;
    tree->Left=child->Right;
    child->Root=tree->Root;
    tree->Root=child;
    child->Right=tree;
    child->size=tree->size;
    fixsize(tree);
    //fixsize(child);
    return child;
}

BT* rotateleft(BT* tree)
{
    BT* child=tree->Right;
    if(!child)
        return tree;
    tree->Right=child->Left;
    child->Root=tree->Root;
    tree->Root=child;
    child->Left=tree;
    child->size=tree->size;
    //fixsize(child);
    fixsize(tree);
    return child;
}

BT* insertroot(BT* tree, BT *root, int key, QString key_ch)
{
    if(!tree)
    {
        return new BT(key, root, key_ch);
    }
    if(key<tree->key)
    {
        tree->Left=insertroot(tree->Left, tree, key, key_ch);

        return rotateright(tree);
    }
    else
    {
        tree->Right=insertroot(tree->Right, tree, key, key_ch);
        return rotateleft(tree);
    }
}

BT* insertrand(BT* tree, BT *root, int key, QString key_ch)
{
    QChar conv;
    if(!tree)
    {
        return new BT(key, root, key_ch);
    }
    if(rand()%(tree->size+1)==0)
        return tree=insertroot(tree, tree->Root, key, key_ch);
    if(tree->key>key)
    {
        tree->Left=insertrand(tree->Left, tree, key, key_ch);
    }
    else
    {
        tree->Right=insertrand(tree->Right, tree, key, key_ch);
    }
    fixsize(tree);
    return tree;
}

//BT* delet(BT* tree, int key)
//{
//    BT* temp;
//    if(!tree)
//        return nullptr;
//    if(tree->key==key)
//    {
//        if(tree->Right==nullptr)
//        {
//            if(tree->Left==nullptr)
//            {
//                if(tree->Root==nullptr)
//                    return nullptr;
//            }
//            else
//            {
//                if(tree->Root==nullptr)
//                {
//                    tree=tree->Left;
//                    delete tree->Root;
//                    tree->Root=nullptr;
//                    return tree;
//                }
//                else
//                {
//                    BT* temp=tree->Left;
//                    tree=tree->Root;
//                    delete temp->Root;
//                    temp->Root=tree;
//                    tree->Left=temp;
//                    return tree;
//                }
//            }
//        }
//        else
//        {
//            if(tree->Right->Left==nullptr)
//            {
//                if(tree->Root==nullptr)
//                {
//                    if(tree->Left==nullptr)
//                    {
//                        tree=tree->Right;
//                        delete tree->Root;
//                        tree->Root=nullptr;
//                        return tree;
//                    }
//                    else
//                    {
//                        temp=tree->Left;
//                        tree=tree->Right;
//                        delete tree->Root;
//                        tree->Root=nullptr;
//                        tree->Left=temp;
//                        return tree;
//                    }
//                }
//                else
//                {
//                    if(tree->Left==nullptr)
//                    {
//                        temp=tree->Right;
//                        tree=tree->Root;
//                        delete temp->Root;
//                        temp->Root=tree;
//                        tree->Right=temp;
//                        return tree;
//                    }
//                    else
//                    {
//                        tree->Right->Left=tree->Left;
//                        tree->Left->Root=tree->Right->Left;
//                        temp=tree->Right;
//                        tree=tree->Root;
//                        delete temp->Root;
//                        temp->Root=tree;
//                        tree->Left=temp;
//                        return tree;
//                    }
//                }
//            }
//            else
//            {
//                tree->Right->Left->Left=tree->Left;
//                tree->Right->Left->Right=tree->Right;
//                tree->Right->Root=tree->Right->Left;
//                temp=tree->Left;
//                tree->Right->Left->Root=tree->Root;
//                tree=tree->Root;
//                tree->Left=tree->Left->Right->Left;
//                delete temp->Root;
//                temp->Root=tree->Left;
//                return tree;
//            }
//        }
//    }
//    if(tree->key>key)
//        delet(tree->Left, key);
//    if(tree->key<key)
//        delet(tree->Right, key);
//}

BT* merge(BT* tree1, BT* tree2)
{
    if(!tree1)
        return tree2;
    if(!tree2)
        return tree1;
    if(rand()%(tree1->size+tree2->size) < tree1->size )
    {
        tree1->Right = merge(tree1->Right,tree2);
        tree1->Root=tree1->Root->Root;
        fixsize(tree1);
        return tree1;
    }
    else
    {
        tree2->Left = merge(tree1,tree2->Left);
        tree2->Root=tree2->Root->Root;
        fixsize(tree2);
        return tree2;
    }
}

BT* remove(BT* tree, int key)
{
    if( !tree )
        return tree;
    if( tree->key==key )
    {
        BT* temp = merge(tree->Left,tree->Right);
        delete tree;
        return temp;
    }
    else if( key<tree->key )
        tree->Left = remove(tree->Left,key);
    else
        tree->Right = remove(tree->Right,key);
    return tree;
}

BT* Find(BT* tree, int key)
{
    if(!tree)
        return nullptr;
    if(key==tree->key)
        return tree;
    if(key<tree->key)
        return Find(tree->Left, key);
    if(key>tree->key)
        return Find(tree->Right, key);
}
