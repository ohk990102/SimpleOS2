#ifndef QUEUE_HEADER
#define QUEUE_HEADER 1
#include <stdbool.h>
struct Queue {
    int dataSize;
    int maxDataCount;
    void *queueBuffer;

    int putIndex;
    int getIndex;
    bool isLastOpPut;
};
void initializeQueue(struct Queue * queue, void * queueBuffer, int maxDataCount, int dataSize);
bool isQueueFull(const struct Queue * queue);
bool isQueueEmpty(const struct Queue * queue);
bool putQueue(struct Queue * queue, const void * data);
bool getQueue(struct Queue * queue, void * data);

#endif