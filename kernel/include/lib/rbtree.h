#ifndef RBTREE_HEADER
#define RBTREE_HEADER 1

#include <stdint.h>
#include <stdbool.h>

#define BLACK   false
#define RED     true
#define CAST_RBTREEBASENODE(x) ((struct RBTreeBaseNode * ) (x))

struct RBTreeBaseNode {
    uint64_t key;
    bool color;
    void * left, * right, * parent;
};

struct RBTree {
    uint32_t count;
    void * root;
    void * nil;
    void * leftmost;
};

void initializeRBTree(struct RBTree * rbtree);
void insertRBTree(struct RBTree * rbtree, void * _item);
void deleteRBTree(struct RBTree * rbtree, void * _item);

#endif // RBTREE_HEADER
