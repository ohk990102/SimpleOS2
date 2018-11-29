#include <stdbool.h>
#include <kernel/keyboard.h>

bool initialize_keyboard() {
    initializeQueue(&keyQueue, keyQueueBuffer, KEY_MAXQUEUECOUNT, sizeof(struct KeyDataStruct));
    return activate_keyboard();
}