#ifndef TASK_HEADER
#define TASK_HEADER 1

#include <stdint.h>
#include <lib/linkedList.h>
#include <arch/i386/interrupt.h>


#define TASK_TCBPOOLADDRESS     0xC1800000 
#define TASK_MAXCOUNT           1024

#define TASK_STACKPOOLADDRESS   0xC1000000
#define TASK_STACKSIZE          8192

#define TASK_INVALIDID          0xFFFFFFFF
#define TASK_PROCESSORTIME      5


struct __attribute__((__packed__)) context_t{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
};

struct RRTaskControlBlock {
    struct LinkedListBaseNode link;
    struct context_t context;
    uint32_t flags;

    void * stackAddress;
    uint32_t stackSize;
};
struct RRTCBPoolManager {
    struct RRTaskControlBlock * tcbStartAddress;
    uint32_t maxCount;
    uint32_t useCount;

    uint32_t allocatedCount;
};
struct RRScheduler {
    struct RRTaskControlBlock * runningTask;
    int processorTime;
    struct LinkedList readyList;
};

void switch_context(struct context_t *currentContext, struct context_t * nextContext);
void setup_task(struct RRTaskControlBlock *tcb, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize);
struct RRTaskControlBlock * createTask(uint32_t flags, void * entryPointAddress);
void initializeScheduler();
void setRunningTask(struct RRTaskControlBlock * task);
struct RRTaskControlBlock * getRunningTask();
struct RRTaskControlBlock * getNextTaskToRun();
void addTaskToReadyList(struct RRTaskControlBlock * task);
void schedule();
bool scheduleInInterrupt(struct registers_t * r);
void decreaseProcessorTime();
bool isProcessorTimeExpired();
#endif