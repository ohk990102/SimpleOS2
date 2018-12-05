#include <stdint.h>
#include <string.h>
#include <arch/i386/task.h>

void setup_task(struct TaskControlBlock *tcb, uint32_t id, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize) {
    memset(&(tcb->context), 0, sizeof(struct context_t));
    tcb->context.useresp = (uint32_t)stackAddress + stackSize;
    tcb->context.ebp = (uint32_t)stackAddress + stackSize;
    tcb->context.cs = 0x08;
    tcb->context.ss = 0x10;
    tcb->context.eip = (uint32_t)entryPointAddress;
    tcb->context.eflags |= 0x0200;
    tcb->id = id;
    tcb->stackAddress = stackAddress;
    tcb->stackSize = stackSize;
    tcb->flags = flags;
}