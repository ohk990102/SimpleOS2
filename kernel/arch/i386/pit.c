#include <stdint.h>
#include <stdbool.h>
#include <arch/i386/pit.h>
#include <arch/i386/utility.h>
#include <arch/i386/task_rr.h>
#include <arch/i386/interrupt.h>

volatile uint32_t tickCount = 0;

void initialize_pit(uint16_t count, bool periodic) {
    outb(PIT_PORT_CONTROL, PIT_COUNTER0_ONCE);

    if(periodic)
        outb(PIT_PORT_CONTROL, PIT_COUNTER0_PERIODIC);
    
    outb(PIT_PORT_COUNTER0, count & 0xFF);
    outb(PIT_PORT_COUNTER0, count >> 8);
}
uint16_t read_counter0() {
    uint8_t high, low;
    uint16_t tmp = 0;

    outb(PIT_PORT_CONTROL, PIT_CONTROL_LATCH);
    low = inb(PIT_PORT_COUNTER0);
    high = inb(PIT_PORT_COUNTER0);

    tmp = high;
    tmp = (tmp << 8) | low;
    return tmp;
}
void wait_using_pit(uint16_t count) {
    uint16_t lastCounter0, currentCounter0;
    initialize_PIC(0, true);
    lastCounter0 = read_counter0();
    while(1) {
        currentCounter0 = read_counter0();
        if(((lastCounter0 - currentCounter0) & 0xFFFF) >= count)
            break;
    }
}
void timer_handler(struct registers_t * r) {
    send_EOI_to_PIC(r->int_no);
    tickCount++;
    decreaseProcessorTime();
    if(isProcessorTimeExpired())
        scheduleInInterrupt(r);
}
uint32_t getTickCount() {
    return tickCount;
}