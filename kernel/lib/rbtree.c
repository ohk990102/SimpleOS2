#include <stdint.h>
#include <stdbool.h>
#include <lib/rbtree.h>


void initializeRBTree(struct RBTree * rbtree) {
    rbtree->count = 0;
    rbtree->root = 0;
}
uint32_t getRBTreeCount(struct RBTree * rbtree) {
    return rbtree->count;
}
void * insertBST(void * _root, void * _item, void ** left) {
    struct RBTreeBaseNode *root = CAST_RBTREEBASENODE(_root);
    struct RBTreeBaseNode *item = CAST_RBTREEBASENODE(_item);
    if(root == 0) {
        *left = item;
        return item;
    }
    else if(item->key <= root->key) {
        root->left = insertBST(root->left, _item, left);
    }
    else {
        root->right = insertBST(root->right, _item, 0);
    }
    return root;
}
void * rotateRight(void * _root, void * _item) {
    struct RBTreeBaseNode * root = CAST_RBTREEBASENODE(_root);
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    struct RBTreeBaseNode * left = CAST_RBTREEBASENODE(item->left);

    item->left = left->right;;
    if(item->left != 0)
        CAST_RBTREEBASENODE(item->left)->parent = item;
    
    left->parent = item->parent;

    if(item->parent == 0)
        root = left;
    else if(item == CAST_RBTREEBASENODE(item->parent)->left)
        CAST_RBTREEBASENODE(item->parent)->left = left;
    else
        CAST_RBTREEBASENODE(item->parent)->right = left;
    
    left->right = item;
    item->parent = left;

    return root;
}
void * rotateLeft(void * _root, void * _item) {
    struct RBTreeBaseNode * root = CAST_RBTREEBASENODE(_root);
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    struct RBTreeBaseNode * right = CAST_RBTREEBASENODE(item->right);
    item->right = right->left;
    if(item->right != 0)
        CAST_RBTREEBASENODE(item->right)->parent = item;
    
    right->parent = item->parent;

    if(item->parent == 0)
        root = right;
    else if(item == CAST_RBTREEBASENODE(item->parent)->left)
        CAST_RBTREEBASENODE(item->parent)->left = right;
    else
        CAST_RBTREEBASENODE(item->parent)->right = right;
    
    right->left = item;
    item->parent = right;

    return root;
}
void * fixViolation(void * _root, void * _item) {
    struct RBTreeBaseNode * parent = 0;
    struct RBTreeBaseNode * grandparent = 0;
    struct RBTreeBaseNode * root = CAST_RBTREEBASENODE(_root);
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);

    while((item != root) && (item->color != BLACK) && (CAST_RBTREEBASENODE(item->parent)->color == RED)) {
        parent = CAST_RBTREEBASENODE(item->parent);
        grandparent = CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent))->parent;
        if(parent == grandparent->left) {
            struct RBTreeBaseNode * uncle = CAST_RBTREEBASENODE(grandparent->right);

            if(uncle != 0 && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                item = grandparent;
            }
            else {
                if(item == parent->right) {
                    root = rotateLeft(root, parent);
                    item = parent;
                    parent = item->parent;
                }

                root = rotateRight(root, grandparent);
                bool tmp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tmp;
                item = parent;
            }
        }
        else {
            struct RBTreeBaseNode * uncle = CAST_RBTREEBASENODE(grandparent->left);
            if(uncle != 0 && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                item = grandparent;
            }
            else {
                if(item == parent->left) {
                    root = rotateRight(root, parent);
                    item = parent;
                    parent = item->parent;
                }
                root = rotateLeft(root, grandparent);
                bool tmp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tmp;
                item = parent;
            }
        }
    }
    root->color = BLACK;
}
void insertRBTree(struct RBTree * rbtree, void * _item) {
    rbtree->root = insertBST(rbtree->root, _item, &(rbtree->leftmost));
    rbtree->root = fixViolation(rbtree->root, _item);
}
