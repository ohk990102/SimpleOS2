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
void initializeLinkedList(struct LinkedList * linkedList);
uint32_t getLinkedListCount(struct LinkedList * linkedList);
void insertTailOfLinkedList(struct LinkedList * linkedList, void * item);
void insertHeadOfLinkedList(struct LinkedList * linkedList, void * item);
void * removeLinkedList(struct LinkedList * linkedList, uint32_t id);
void * removeHeadOfLinkedList(struct LinkedList * linkedList);
void * removeTailOfLinkedList(struct LinkedList * linkedList);
void * findLinkedList(const struct LinkedList * linkedList, uint32_t id);
void *getHeadOfLinkedList(const struct LinkedList * linkedList);
void *getTailOfLinkedList(const struct LinkedList * linkedList);
void * getNextFromLinkedList(const struct LinkedList * linkedList, void * _cu);

#endif