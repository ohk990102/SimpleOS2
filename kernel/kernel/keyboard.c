#include <stdint.h>
#include <kernel/keyboard.h>



uint8_t initialize_keyboard() {
    initializeQueue(&keyQueue, keyQueueBuffer, KEY_MAXQUEUECOUNT, sizeof(struct KeyDataStruct));
    activate_keyboard();
}