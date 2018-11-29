#ifndef PIT_HEADER
#define PIT_HEADER 1

#include <stdint.h>
#include <stdbool.h>

#define PIT_FREQUENCY   1193180
#define MSTOCOUNT(x)    (PIT_FREQUENCY * (x) / 1000)
#define USTOCOUNT(x)    (PIT_FREQUENCY * (x) / 1000000)

#define PIT_PORT_CONTROL    0x43
#define PIT_PORT_COUNTER0   0x40

#define PIT_CONTROL_COUNTER0    0x00
#define PIT_CONTROL_LSBMSBRW    0x30
#define PIT_CONTROL_LATCH       0x00
#define PIT_CONTROL_MODE0       0x00
#define PIT_CONTROL_MODE2       0x04
#define PIT_CONTROL_BINARYCOUNTER   0x00
#define PIT_CONTROL_BCDCOUNTER  0x01

#define PIT_COUNTER0_ONCE       (PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE0 | PIT_CONTROL_BINARYCOUNTER)
#define PIT_COUNTER0_PERIODIC   (PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE2 | PIT_CONTROL_BINARYCOUNTER)

void initialize_pit(uint16_t count, bool periodic);
uint16_t read_counter0();
void wait_using_pit(uint16_t count);

#endif