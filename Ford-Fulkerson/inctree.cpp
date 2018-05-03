#include "inctree.h"

INCTR* delete_tree(INCTR* tree)
{
    if(!tree)
        return tree;
    delete [] tree;
    return tree;
}
