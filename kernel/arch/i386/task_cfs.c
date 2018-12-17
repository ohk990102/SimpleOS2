#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arch/i386/utility.h>
#include <arch/i386/task_cfs.h>

#define  NICE_0_LOAD    1024

static struct CFSTCBPoolManager tcbPoolManager;

static struct CFSScheduler cfsScheduler;

unsigned int sysctl_sched_latency = 8000000ULL;
static unsigned int sched_nr_latency = 8;
unsigned int sysctl_sched_min_granularity = 1000000ULL;

static const int prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

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
        tcbPoolManager.tcbStartAddress[i].id = i;
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
        if((tcbPoolManager.tcbStartAddress[i].id >> 31 == 0)) {
            tcb = &(tcbPoolManager.tcbStartAddress[i]);
            break;
        }
    }
    if(tcb == 0)
        return 0;
    
    tcb->id = i | 0x80000000;
    tcbPoolManager.useCount++;
    tcbPoolManager.allocatedCount++;

    if(tcbPoolManager.allocatedCount == 0)
        tcbPoolManager.allocatedCount = 1;
    
    return tcb;
}

void freeTCB(uint32_t id) {
    int i = id & 0x7FFFFFFF;
    memset(&(tcbPoolManager.tcbStartAddress[i].context), 0, sizeof(struct context_t));
    tcbPoolManager.tcbStartAddress[i].id = i;
    tcbPoolManager.useCount--;
}

struct CFSTaskControlBlock * createTask(uint32_t flags, void * entryPointAddress, uint32_t nice) {
    struct CFSTaskControlBlock * task;
    void * stackAddress;

    task = allocateTCB();
    if(task == 0)
        return 0;
    
    stackAddress = (void * ) (TASK_STACKPOOLADDRESS + (TASK_STACKSIZE * (task->id & 0x7FFFFFFF)));
    setup_task(task, flags, entryPointAddress, stackAddress, TASK_STACKSIZE, nice);
    cfsScheduler.totalWeight += prio_to_weight[task->nice + 20];
    task->link.key = cfsScheduler.runningTask->link.key;
    addTaskToRunQueue(task);
    return task;
}
void initializeScheduler() {
    initializeTCBPool();
    initializeRBTree(&(cfsScheduler.runqueues));
    cfsScheduler.runningTask = allocateTCB();
    cfsScheduler.processorTime = 0;
    cfsScheduler.totalWeight += 1024;
}
void setRunningTask(struct CFSTaskControlBlock * task) {
    cfsScheduler.runningTask = task;
}
struct CFSTaskControlBlock * getRunningTask() {
    return cfsScheduler.runningTask;
}
struct CFSTaskControlBlock * getNextTaskToRun() {
    if(cfsScheduler.runqueues.count == 0)
        return 0;
    struct CFSTaskControlBlock * nextTask = cfsScheduler.runqueues.leftmost;
    deleteRBTree(&(cfsScheduler.runqueues), nextTask);
    return nextTask;
}
void addTaskToRunQueue(struct CFSTaskControlBlock * task) {
    insertRBTree(&(cfsScheduler.runqueues), task);
}

void schedule() {
    struct CFSTaskControlBlock * runningTask, *nextTask;
    bool previousFlag;

    if(cfsScheduler.runqueues.count == 0) {
        if(cfsScheduler.processorTime <= 0)
            cfsScheduler.processorTime = cfsScheduler.timeslice;
        return;
    }
    previousFlag = set_interrupt_flag(false);

    runningTask = cfsScheduler.runningTask;
    runningTask->link.key += NICE_0_LOAD * (cfsScheduler.timeslice - cfsScheduler.processorTime + 1000000) / prio_to_weight[runningTask->nice + 20];

    addTaskToRunQueue(runningTask);

    uint64_t period = sysctl_sched_latency;
    unsigned long nr_latency = sched_nr_latency;

    if (cfsScheduler.runqueues.count > nr_latency) {
        period = sysctl_sched_min_granularity;
        period *= cfsScheduler.runqueues.count + 1;
    }
    nextTask = getNextTaskToRun();

    if(nextTask == 0)
        abort();

    uint64_t slice = period * prio_to_weight[nextTask->nice + 20] / cfsScheduler.totalWeight;
    cfsScheduler.processorTime = slice;
    cfsScheduler.timeslice = slice;

    cfsScheduler.runningTask = nextTask;
    set_interrupt_flag(previousFlag);

    switch_context(&(runningTask->context), &(nextTask->context));   
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
    if(changeRing) {
        c->useresp = r->useresp;
        c->ss = r->ss;
    }
    else {
        c->useresp = r->esp;
        c->ss = 0x10;
    }
}
bool scheduleInInterrupt(struct registers_t * r) {
    struct CFSTaskControlBlock * runningTask, *nextTask;

    if(cfsScheduler.runqueues.count == 0) {
        cfsScheduler.processorTime = cfsScheduler.timeslice;
        return false;
    }
    runningTask = cfsScheduler.runningTask;
    runningTask->link.key += NICE_0_LOAD * (cfsScheduler.timeslice - cfsScheduler.processorTime) / prio_to_weight[runningTask->nice + 20];

    addTaskToRunQueue(runningTask);

    uint64_t period = sysctl_sched_latency;
    unsigned long nr_latency = sched_nr_latency;

    if (cfsScheduler.runqueues.count > nr_latency) {
        period = sysctl_sched_min_granularity;
        period *= cfsScheduler.runqueues.count + 1;
    }
    nextTask = getNextTaskToRun();

    if(nextTask == 0) {
        set_interrupt_flag(false);
        abort();
    }
    
    uint64_t slice = period * prio_to_weight[nextTask->nice + 20] / cfsScheduler.totalWeight;
    cfsScheduler.processorTime = slice;
    cfsScheduler.timeslice = slice;

    cfsScheduler.runningTask = nextTask;

    switch_context(&(runningTask->context), &(nextTask->context));
}
void decreaseProcessorTime() {
    if(cfsScheduler.processorTime > 0)
        cfsScheduler.processorTime -= 1000000;
}
bool isProcessorTimeExpired() {
    if(cfsScheduler.processorTime <= 0)
        return true;
    return false;
}