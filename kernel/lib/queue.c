#include <stdbool.h>
#include <string.h>
#include <lib/queue.h>


void initializeQueue(struct Queue * queue, void * queueBuffer, int maxDataCount, int dataSize) {
    queue->maxDataCount = maxDataCount;
    queue->dataSize = dataSize;
    queue->queueBuffer = queueBuffer;

    queue->putIndex = 0;
    queue->getIndex = 0;
    queue->isLastOpPut = false;
}

bool isQueueFull(const struct Queue * queue) {
    if((queue->getIndex == queue->putIndex) && (queue->isLastOpPut))
        return true;
    return false;
}
bool isQueueEmpty(const struct Queue * queue) {
    if((queue->getIndex == queue->putIndex) && !(queue->isLastOpPut))
        return true;
    return false;
}
bool putQueue(struct Queue * queue, const void * data) {
    if(isQueueFull(queue) == true)
        return false;
    
    memcpy(queue->queueBuffer + (queue->dataSize * queue->putIndex), data, queue->dataSize);
    queue->putIndex = (queue->putIndex + 1) % queue->maxDataCount;
    queue->isLastOpPut = true;
    return true;
}
bool getQueue(struct Queue * queue, void * data) {
    if(isQueueEmpty(queue) == true)
        return false;
    memcpy(data, queue->queueBuffer + (queue->getIndex * queue->dataSize), queue->dataSize);
    queue->getIndex = (queue->getIndex + 1) % queue->maxDataCount;
    queue->isLastOpPut = false;
    return true;
}
