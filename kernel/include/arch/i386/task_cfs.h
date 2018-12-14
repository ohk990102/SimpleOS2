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
};