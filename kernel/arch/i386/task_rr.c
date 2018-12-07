#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <lib/linkedList.h>
#include <arch/i386/task_rr.h>
#include <arch/i386/utility.h>
#include <arch/i386/interrupt.h>

static struct RRScheduler rrScheduler;
static struct TCBPoolManager tcbPoolManager;

void setup_task(struct TaskControlBlock *tcb, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize) {
    memset(&(tcb->context), 0, sizeof(struct context_t));
    tcb->context.useresp = (uint32_t)stackAddress + stackSize;
    tcb->context.ebp = (uint32_t)stackAddress + stackSize;
    tcb->context.cs = 0x08;
    tcb->context.ss = 0x10;
    tcb->context.eip = (uint32_t)entryPointAddress;
    tcb->context.eflags |= 0x0200;
    tcb->stackAddress = stackAddress;
    tcb->stackSize = stackSize;
    tcb->flags = flags;
}

void initializeTCBPool() {
    memset(&(tcbPoolManager), 0, sizeof(tcbPoolManager));
    tcbPoolManager.tcbStartAddress = (void *) TASK_TCBPOOLADDRESS;
    memset((void *) TASK_TCBPOOLADDRESS, 0, sizeof(struct TaskControlBlock ) * TASK_MAXCOUNT);

    for(int i = 0; i < TASK_MAXCOUNT; i++) {
        tcbPoolManager.tcbStartAddress[i].link.id = i;
    }

    tcbPoolManager.maxCount = TASK_MAXCOUNT;
    tcbPoolManager.allocatedCount = 1;
}

struct TaskControlBlock * allocateTCB() {
    struct TaskControlBlock * tcb = 0;
    
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

struct TaskControlBlock * createTask(uint32_t flags, void * entryPointAddress) {
    struct TaskControlBlock * task;
    void * stackAddress;

    task = allocateTCB();
    if(task == 0)
        return 0;
    
    stackAddress = (void *) (TASK_STACKPOOLADDRESS + (TASK_STACKSIZE * (task->link.id & 0x7FFFFFFF)));
    setup_task(task, flags, entryPointAddress, stackAddress, TASK_STACKSIZE);
    addTaskToReadyList(task);

    return task;
}

void initializeScheduler() {
    initializeTCBPool();
    initializeLinkedList(&(rrScheduler.readyList));
    rrScheduler.runningTask = allocateTCB();
}

void setRunningTask(struct TaskControlBlock * task) {
    rrScheduler.runningTask = task;
}
struct TaskControlBlock * getRunningTask() {
    return rrScheduler.runningTask;
}
struct TaskControlBlock * getNextTaskToRun() {
    if(getLinkedListCount(&(rrScheduler.readyList)) == 0)
        return 0;
    return (struct TaskControlBlock * ) removeHeadOfLinkedList(&rrScheduler.readyList);
}
void addTaskToReadyList(struct TaskControlBlock * task) {
    insertTailOfLinkedList(&(rrScheduler.readyList), task);
}
void schedule() {
    struct TaskControlBlock * runningTask, *nextTask;
    bool previousFlag;

    if(getLinkedListCount(&(rrScheduler.readyList)) == 0)
        return;
    previousFlag = set_interrupt_flag(false);
    nextTask = getNextTaskToRun();
    if(nextTask == 0) {
        set_interrupt_flag(previousFlag);
        return;
    }

    runningTask = rrScheduler.runningTask;
    addTaskToReadyList(runningTask);

    rrScheduler.runningTask = nextTask;
    switch_context(&(runningTask->context), &(nextTask->context));

    rrScheduler.processorTime = TASK_PROCESSORTIME;
    set_interrupt_flag(previousFlag);
}
void copyContextToRegisters(struct registers_t * r, struct context_t * c, bool changeRing) {
    memcpy(&(r->edi), c, sizeof(uint32_t) * 8);
    r->eip = c->eip;
    r->cs = c->cs;
    r->eflags = c->eflags;
    if(changeRing) {
        r->useresp = c->useresp;
        r->ss = c->ss;
    }
}
void copyRegistersToContext(struct context_t * c, struct registers_t * r, bool changeRing) {
    memcpy(c, &(r->edi), sizeof(uint32_t) * 8);
    c->eip = r->eip;
    c->cs = r->cs;
    c->eflags = r->eflags;
    c->useresp = &(r->useresp);
    c->ss = 0x08;
}
bool scheduleInInterrupt(struct registers_t * r) {
    struct TaskControlBlock * runningTask, *nextTask;

    nextTask = getNextTaskToRun();
    if(nextTask == 0)
        return false;
    runningTask = rrScheduler.runningTask;

    copyRegistersToContext(&(runningTask->context), r, false);
    addTaskToReadyList(runningTask);
    rrScheduler.runningTask = nextTask;
    copyContextToRegisters(r, &(nextTask->context), false);
    rrScheduler.processorTime = TASK_PROCESSORTIME;
    return true;
}
void decreaseProcessorTime() {
    if(rrScheduler.processorTime > 0)
        rrScheduler.processorTime--;
}
bool isProcessorTimeExpired() {
    if(rrScheduler.processorTime <= 0)
        return true;
    return false;
}
