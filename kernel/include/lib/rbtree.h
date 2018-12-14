#ifndef RBTREE_HEADER
#define RBTREE_HEADER 1

#include <stdint.h>
#include <stdbool.h>

#define BLACK   true
#define RED     false
#define CAST_RBTREEBASENODE(x) ((struct RBTreeBaseNode * ) (x))

struct RBTreeBaseNode {
    int id;
    int key;
    bool color;
    void * left, * right, * parent;
};

struct RBTree {
    uint32_t count;
    void * root;
    void * leftmost;
};

void initializeRBTree(struct RBTree * rbtree);
uint32_t getRBTreeCount(struct RBTree * rbtree);
void insertRBTree(struct RBTree * rbtree, void * _item);

#endif