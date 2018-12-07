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

struct TaskControlBlock {
    struct LinkedListBaseNode link;
    struct context_t context;
    uint32_t flags;

    void * stackAddress;
    uint32_t stackSize;
};
struct TCBPoolManager {
    struct TaskControlBlock * tcbStartAddress;
    uint32_t maxCount;
    uint32_t useCount;

    uint32_t allocatedCount;
};
struct RRScheduler {
    struct TaskControlBlock * runningTask;
    int processorTime;
    struct LinkedList readyList;
};

void switch_context(struct context_t *currentContext, struct context_t * nextContext);
void setup_task(struct TaskControlBlock *tcb, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize);
struct TaskControlBlock * createTask(uint32_t flags, void * entryPointAddress);
void initializeScheduler();
void setRunningTask(struct TaskControlBlock * task);
struct TaskControlBlock * getRunningTask();
struct TaskControlBlock * getNextTaskToRun();
void addTaskToReadyList(struct TaskControlBlock * task);
void schedule();
bool scheduleInInterrupt(struct registers_t * r);
void decreaseProcessorTime();
bool isProcessorTimeExpired();
#endif