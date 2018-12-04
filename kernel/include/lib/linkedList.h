#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER 1

#include <stdint.h>

struct LinkedListBaseNode {
    void * next;
    uint32_t id;
};
struct LinkedList {
    uint32_t count;
    void * head;
    void * tail;
};

#endif