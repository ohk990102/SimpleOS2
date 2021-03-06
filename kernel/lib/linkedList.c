#include <stdint.h>
#include <lib/linkedList.h>

void initializeLinkedList(struct LinkedList * linkedList) {
    linkedList->count = 0;
    linkedList->head = 0;
    linkedList->tail = 0;
}
uint32_t getLinkedListCount(struct LinkedList * linkedList) {
    return linkedList->count;
}
void insertTailOfLinkedList(struct LinkedList * linkedList, void * item) {
    struct LinkedListBaseNode * cu;
    cu = (struct LinkedListBaseNode * ) item;
    cu->next = 0;
    if(linkedList->head == 0) {
        linkedList->head = item;
        linkedList->tail = item;
        linkedList->count = 1;
        return;
    }

    cu = linkedList->tail;
    cu->next = item;
    linkedList->tail = item;
    linkedList->count++;
}
void insertHeadOfLinkedList(struct LinkedList * linkedList, void * item) {
    struct LinkedListBaseNode * cu;
    cu = (struct LinkedListBaseNode *) item;
    cu->next = linkedList->head;

    if(linkedList->head == 0) {
        linkedList->head = item;
        linkedList->tail = item;
        linkedList->count = 1;
        return;
    }
    linkedList->head = item;
    linkedList->count++;
}
void * removeLinkedList(struct LinkedList * linkedList, uint32_t id) {
    struct LinkedListBaseNode * cu;
    struct LinkedListBaseNode * prev;

    prev = (struct LinkedListBaseNode * ) linkedList->head;
    for(cu = prev; cu != 0; cu = cu->next) {
        if(cu->id == id) {
            if((cu == linkedList->head) && (cu == linkedList->tail)) {
                linkedList->head = 0;
                linkedList->tail = 0;
            }
            else if(cu == linkedList->head) {
                linkedList->head = cu->next;
            }
            else if(cu == linkedList->tail) {
                linkedList->tail = prev;
            }
            else {
                prev->next = cu->next;
            }
            linkedList->count--;
            return cu;
        }
        prev = cu;
    }
    return 0;
}
void * removeHeadOfLinkedList(struct LinkedList * linkedList) {
    struct LinkedListBaseNode * cu;

    if(linkedList->count == 0)
        return 0;
    
    cu = (struct LinkedListBaseNode * ) linkedList->head;
    return removeLinkedList(linkedList, cu->id);
}

void * removeTailOfLinkedList(struct LinkedList * linkedList) {
    struct LinkedListBaseNode * cu;
    if(linkedList->count == 0)
        return 0;
    
    cu = (struct LinkedListBaseNode * ) linkedList->tail;
    return removeLinkedList(linkedList, cu->id);
}

void * findLinkedList(const struct LinkedList * linkedList, uint32_t id) {
    struct LinkedListBaseNode * cu;
    for(cu = (struct LinkedListBaseNode * ) linkedList->head; cu != 0; cu = cu->next) {
        if(cu->id == id)
            return cu;
    }
    return 0;
}
void *getHeadOfLinkedList(const struct LinkedList * linkedList) {
    return linkedList->head;
}
void *getTailOfLinkedList(const struct LinkedList * linkedList) {
    return linkedList->tail;
}
void * getNextFromLinkedList(const struct LinkedList * linkedList, void * _cu) {
    struct LinkedListBaseNode * cu = (struct LinkedListBaseNode * ) _cu;
    return cu->next;
}