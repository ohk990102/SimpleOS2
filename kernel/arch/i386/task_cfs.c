#include <stdint.h>
#include <arch/i386/task_cfs.h>

static struct CFSTCBPoolManager tcbPoolManager;

static struct CFSScheduler cfsScheduler;

void setup_task(struct CFSTaskControlBlock *tcb, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize, uint32_t nice) {
    memset(&(tcb->context), 0, sizeof(struct context_t));
    tcb->context.useresp = (uint32_t) stackAddress + stackSize;
    tcb->context.ebp = (uint32_t) stackAddress + stackSize;
    tcb->context.cs = 0x08;
    tcb->context.ss = 0x10;
    tcb->context.eip = (uint32_t) entryPointAddress;
    tcb->context.eflags |= 0x0200;
    tcb->stackAddress = stackAddress;
    tcb->stackSize = stackSize;
    tcb->flags = flags;
    tcb->nice = nice;
}

void initializeTCBPool() {
    memset(&(tcbPoolManager), 0, sizeof(tcbPoolManager));
    tcbPoolManager.tcbStartAddress = (void *) TASK_TCBPOOLADDRESS;
    memset((void *) TASK_TCBPOOLADDRESS, 0, sizeof(struct CFSTaskControlBlock) * TASK_MAXCOUNT);

    for(int i = 0; i < TASK_MAXCOUNT; i++) {
        tcbPoolManager.tcbStartAddress[i].link.id = i;
    }
    tcbPoolManager.maxCount = TASK_MAXCOUNT;
    tcbPoolManager.allocatedCount = 1;
}
struct CFSTaskControlBlock * allocateTCB() {
    struct CFSTaskControlBlock * tcb = 0;
    if(tcbPoolManager.useCount == tcbPoolManager.maxCount) {
        return 0;
    }
    uint32_t i;
    for(i = 0; i < tcbPoolManager.maxCount; i++) {
        if((tcbPoolManager.tcbStartAddress[i].link.id >> 31 == 0)) {
            tcb = &(tcbPoolManager.tcbStartAddress[i]);
            break;
        }
    }
    if(tcb == 0)
        return 0;
    
    tcb->link.id = i | 0x80000000;
    tcbPoolManager.useCount++;
    tcbPoolManager.allocatedCount++;

    if(tcbPoolManager.allocatedCount == 0)
        tcbPoolManager.allocatedCount = 1;
    
    return tcb;
}

void freeTCB(uint32_t id) {
    int i = id & 0x7FFFFFFF;
    memset(&(tcbPoolManager.tcbStartAddress[i].context), 0, sizeof(struct context_t));
    tcbPoolManager.tcbStartAddress[i].link.id = i;
    tcbPoolManager.useCount--;
}

struct CFSTaskControlBlock * createTask(uint32_t flags, void * entryPointAddress, uint32_t nice) {
    struct CFSTaskControlBlock * task;
    void * stackAddress;

    task = allocateTCB();
    if(task == 0)
        return 0;
    
    stackAddress = (void * ) (TASK_STACKPOOLADDRESS + (TASK_STACKSIZE * (task->link.id & 0x7FFFFFFF)));
    setup_task(task, flags, entryPointAddress, stackAddress, TASK_STACKSIZE, nice);

    return task;
}
void initializeScheduler() {
    initializeTCBPool();
    initializeRBTree(&(cfsScheduler.runqueues));
    cfsScheduler.runningTask = allocateTCB();
}
void setRunningTask(struct CFSTaskControlBlock * task) {
    cfsScheduler.runningTask = task;
}
struct CFSTaskControlBlock * getRunningTask() {
    return cfsScheduler.runningTask;
}