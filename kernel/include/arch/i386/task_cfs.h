#ifndef TASK_CFS_HEADER
#define TASK_CFS_HEADER 1

#include <stdint.h>
#include <lib/rbtree.h>
#include <arch/i386/interrupt.h>


#define TASK_TCBPOOLADDRESS     0xC1800000
#define TASK_MAXCOUNT           1024

#define TASK_STACKPOOLADDRESS   0xC1000000
#define TASK_STACKSIZE          8192

#define TASK_INVALIDID          5


struct __attribute__((__packed__)) context_t{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
};

struct CFSTaskControlBlock {
    struct RBTreeBaseNode link;
    uint32_t id;
    struct context_t context;
    uint32_t flags;
    void * stackAddress;
    uint32_t stackSize;
    uint32_t nice;
};
struct CFSTCBPoolManager {
    struct CFSTaskControlBlock * tcbStartAddress;
    uint32_t maxCount;
    uint32_t useCount;

    uint32_t allocatedCount;
};
struct CFSScheduler {
    struct CFSTaskControlBlock * runningTask;
    struct RBTree runqueues;
    int64_t processorTime;
    int64_t timeslice;
    uint64_t totalWeight;
};
void initializeScheduler();
void schedule();
void setup_task(struct CFSTaskControlBlock *tcb, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize, uint32_t nice);
void switch_context(struct context_t *currentContext, struct context_t * nextContext);
struct CFSTaskControlBlock * createTask(uint32_t flags, void * entryPointAddress, uint32_t nice);
void decreaseProcessorTime();
bool isProcessorTimeExpired();
struct CFSTaskControlBlock * getRunningTask();

#endif