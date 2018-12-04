#ifndef TASK_HEADER
#define TASK_HEADER 1

#include <stdint.h>

struct __attribute__((__packed__)) context_t{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
};

struct TaskControlBlock {
    struct context_t context;

    uint32_t id;
    uint32_t flags;

    void * stackAddress;
    uint32_t stackSize;
};

void switch_context(struct context_t *currentContext, struct context_t * nextContext);
void setup_task(struct TaskControlBlock *tcb, uint32_t id, uint32_t flags, void * entryPointAddress, void * stackAddress, uint32_t stackSize);

#endif