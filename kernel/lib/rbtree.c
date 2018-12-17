#include <stdint.h>
#include <stdbool.h>
#include <lib/rbtree.h>

static struct RBTreeBaseNode nil;

void initializeRBTree(struct RBTree * rbtree) {
    rbtree->count = 0;
    rbtree->root = &nil;
    rbtree->nil = &nil;
    nil.color = BLACK;
}
uint32_t getRBTreeCount(struct RBTree * rbtree) {
    return rbtree->count;
}
void rotateRight(struct RBTree * rbtree, void * _item) {
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    struct RBTreeBaseNode * temp = item->left;

    item->left = temp->right;

    if(item->left != rbtree->nil)
        CAST_RBTREEBASENODE(item->left)->parent = item;

    temp->parent = item->parent;

    if(item->parent == rbtree->nil)
        rbtree->root = temp;
    else if(item == CAST_RBTREEBASENODE(item->parent)->left)
        CAST_RBTREEBASENODE(item->parent)->left = temp;
    else
        CAST_RBTREEBASENODE(item->parent)->right = temp;

    temp->right = item;
    item->parent = temp;
}
void rotateLeft(struct RBTree * rbtree, void * _item) {
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    struct RBTreeBaseNode * temp = item->right;

    item->right = temp->left;
    if(item->right != rbtree->nil)
        CAST_RBTREEBASENODE(item->right)->parent = item;

    temp->parent = item->parent;

    if(item->parent == rbtree->nil)
        rbtree->root = temp;
    else if(item == CAST_RBTREEBASENODE(item->parent)->left)
        CAST_RBTREEBASENODE(item->parent)->left = temp;
    else
        CAST_RBTREEBASENODE(item->parent)->right = temp;

    temp->left = item;
    item->parent = temp;
}
void fixViolation(struct RBTree * rbtree, void * _item) {
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    struct RBTreeBaseNode * u = 0;

    while(CAST_RBTREEBASENODE(item->parent)->color == RED) {
        if(item->parent == CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->left) {
            u = CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->right;

            if(u->color == RED) {
                CAST_RBTREEBASENODE(item->parent)->color = BLACK;
                u->color = BLACK;
                CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->color = RED;

                item = CAST_RBTREEBASENODE(item->parent)->parent;
            }
            else {
                if(item == CAST_RBTREEBASENODE(item->parent)->right) {
                    item = item->parent;
                    rotateLeft(rbtree, item);
                }
                CAST_RBTREEBASENODE(item->parent)->color = BLACK;
                CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->color = RED;

                rotateRight(rbtree, CAST_RBTREEBASENODE(item->parent)->parent);
            }
        }
        else {
            u = CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->left;
            if(u->color == RED) {
                CAST_RBTREEBASENODE(item->parent)->color = BLACK;
                u->color = BLACK;
                CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->color = RED;

                item = CAST_RBTREEBASENODE(item->parent)->parent;
            }
            else {
                if(item == CAST_RBTREEBASENODE(item->parent)->left) {
                    item = item->parent;
                    rotateRight(rbtree, item);
                }
                CAST_RBTREEBASENODE(item->parent)->color = BLACK;
                CAST_RBTREEBASENODE(CAST_RBTREEBASENODE(item->parent)->parent)->color = RED;

                rotateLeft(rbtree, CAST_RBTREEBASENODE(item->parent)->parent);
            }
        }
    }

    CAST_RBTREEBASENODE(rbtree->root)->color = BLACK;
}

void insertRBTree(struct RBTree * rbtree, void * _item) {
    struct RBTreeBaseNode * target = rbtree->root;
    struct RBTreeBaseNode * parent = rbtree->nil;
    struct RBTreeBaseNode * item = CAST_RBTREEBASENODE(_item);
    bool leftmost = true;

    item->left = rbtree->nil;
    item->right = rbtree->nil;
    item->color = RED;

    while(target != rbtree->nil) {
        parent = target;
        if(item->key > target->key) {
            target = target->right;
            leftmost = false;
        }
        else
            target = target->left;
    }
    item->parent = parent;
    if(leftmost)
        rbtree->leftmost = item;

    if(parent == rbtree->nil)
        rbtree->root = item;
    else if(item->key > parent->key)
        parent->right = item;
    else
        parent->left = item;


    fixViolation(rbtree, item);

    rbtree->count++;
}
void tree_transplant(struct RBTree * rbtree, void * _t, void * _c) {
    struct RBTreeBaseNode * t = _t;
    struct RBTreeBaseNode * c = _c;
    if(t->parent == rbtree->nil)
        rbtree->root = c;
    else if(t == CAST_RBTREEBASENODE(t->parent)->left)
        CAST_RBTREEBASENODE(t->parent)->left = c;
    else
        CAST_RBTREEBASENODE(t->parent)->right = c;

    c->parent = t->parent;
}
void fixViolationDelete(struct RBTree * rbtree, void * _x) {
    struct RBTreeBaseNode * x = _x;
    struct RBTreeBaseNode * s = 0;

    while((x != rbtree->root) && (x->color == BLACK)) {
        if(x == CAST_RBTREEBASENODE(x->parent)->left) {
            s = CAST_RBTREEBASENODE(x->parent)->right;
            if(s->color == RED) {
                s->color = BLACK;
                CAST_RBTREEBASENODE(x->parent)->color = RED;
                rotateLeft(rbtree, x->parent);
                s = CAST_RBTREEBASENODE(x->parent)->right;
            }
            if(CAST_RBTREEBASENODE(s->left)->color == BLACK && CAST_RBTREEBASENODE(s->right)->color == BLACK) {
                s->color = RED;
                x = x->parent;
            }
            else if(CAST_RBTREEBASENODE(s->left)->color == RED && CAST_RBTREEBASENODE(s->right)->color == BLACK) {
                s->color = RED;
                CAST_RBTREEBASENODE(s->left)->color = BLACK;
                rotateRight(rbtree, s);
                s = CAST_RBTREEBASENODE(x->parent)->right;
            }

            if(CAST_RBTREEBASENODE(s->right)->color == RED) {
                s->color = CAST_RBTREEBASENODE(x->parent)->color;
                CAST_RBTREEBASENODE(s->right)->color = BLACK;
                CAST_RBTREEBASENODE(x->parent)->color = BLACK;
                rotateLeft(rbtree, x->parent);

                x = rbtree->root;
            }
        }
        else {
            s = CAST_RBTREEBASENODE(x->parent)->left;
            if(s->color == RED) {
                s->color = BLACK;
                CAST_RBTREEBASENODE(x->parent)->color = RED;
                rotateRight(rbtree, x->parent);
                s = CAST_RBTREEBASENODE(x->parent)->left;
            }
            if(CAST_RBTREEBASENODE(s->left)->color == BLACK && CAST_RBTREEBASENODE(s->right)->color == BLACK) {
                s->color = RED;
                x = x->parent;
            }
            else if(CAST_RBTREEBASENODE(s->left)->color == BLACK && CAST_RBTREEBASENODE(s->right)->color == RED) {
                s->color = RED;
                CAST_RBTREEBASENODE(s->right)->color = BLACK;
                rotateLeft(rbtree, s);
                s = CAST_RBTREEBASENODE(x->parent)->left;
            }
            if(CAST_RBTREEBASENODE(s->left)->color == RED) {
                s->color = CAST_RBTREEBASENODE(x->parent)->color;
                CAST_RBTREEBASENODE(s->left)->color = BLACK;
                CAST_RBTREEBASENODE(x->parent)->color = BLACK;
                rotateRight(rbtree, x->parent);

                x = rbtree->root;
            }


        }
    }
    x->color = BLACK;
}
void deleteRBTree(struct RBTree * rbtree, void * _item) {
    struct RBTreeBaseNode * item = _item;
    struct RBTreeBaseNode * temp = 0;
    struct RBTreeBaseNode * x = 0;
    bool color = item->color;


    if(item->right != rbtree->nil) {
        temp = item->right;
        while(temp->left != rbtree->nil)
            temp = temp->left;
        rbtree->leftmost = temp;
    }
    else
        rbtree->leftmost = item->parent;

    if(item->left == rbtree->nil) {
        x = item->right;
        tree_transplant(rbtree, item, item->right);
    }
    else if(item->right == rbtree->nil) {
        x = item->left;
        tree_transplant(rbtree, item, item->left);
    }
    else {
        temp = item->right;
        while(temp->left != rbtree->nil)
            temp = temp->left;

        color = temp->color;
        x = temp->right;

        tree_transplant(rbtree, temp, temp->right);
        temp->right = item->right;
        CAST_RBTREEBASENODE(temp->right)->parent = temp;

        tree_transplant(rbtree, item, temp);
        temp->left = item->left;
        CAST_RBTREEBASENODE(temp->left)->parent = temp;
        temp->color = item->color;
    }

    if(color == BLACK)
        fixViolationDelete(rbtree, x);
    
    rbtree->count--;
}
