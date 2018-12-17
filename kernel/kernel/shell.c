#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <kernel/tty.h>
#include <arch/i386/task_cfs.h>
#include <arch/i386/vga.h>

struct ShellCommandEntry commandTable[] = {
    {"help", "Show Help", help},
    {"createTestTask", "Create Test Task", createTestTask}
};

void startShell() {
    char commandBuffer[1024];
    int commandBufferIndex = 0;
    unsigned char key;
    int x, y;

    printf(SHELL_PROMPT);

    while(1) {
        key = getchar();
        if(key == KEY_ENTER) {
            printf("\n");
            if(commandBufferIndex > 0) {
                commandBuffer[commandBufferIndex] = '\0';
                executeCommand(commandBuffer);
            }

            printf(SHELL_PROMPT);
            memset(commandBuffer, 0, 1024);
            commandBufferIndex = 0;
        }
        else if((key == KEY_LSHIFT) || (key == KEY_RSHIFT) || (key == KEY_CAPSLOCK) || (key == KEY_NUMLOCK) || (key == KEY_SCROLLLOCK)) {
            ;
        }
        else {
            if(key == KEY_TAB)
                key = ' ';
            if(commandBufferIndex < 1024) {
                commandBuffer[commandBufferIndex++] = key;
                printf("%c", key);
            }
        }
    }
}
void executeCommand(const char * commandBuffer) {
    int i;
    int spaceIndex;
    int commandBufferLength, commandLength;
    int count;

    commandBufferLength = strlen(commandBuffer);
    for(spaceIndex = 0; spaceIndex < commandBufferLength; spaceIndex++) {
        if(commandBuffer[spaceIndex] == ' ')
            break;
    }

    count = sizeof(commandTable) / sizeof(struct ShellCommandEntry);
    for(i = 0; i < count; i++) {
        commandLength = strlen(commandTable[i].command);
        if(commandLength == spaceIndex && memcmp(commandTable[i].command, commandBuffer, spaceIndex) == 0) {
            
            commandTable[i].function(commandBuffer + spaceIndex + 1);
            break;
        }
    }
    if(i >= count)
        printf("Command %s does not exist\n", commandBuffer);
}
void initializeParameter(struct ParameterList * list, const char * parameter) {
    list->buffer = parameter;
    list->length = strlen(parameter);
    list->currentPosition = 0;
}
int getNextParameter(struct ParameterList * list, char * parameter) {
    int i;
    int length;
    if(list->length <= list->currentPosition) {
        return 0;
    }
    for(i = list->currentPosition; i < list->length; i++) {
        if(list->buffer[i] == ' ') {
            break;
        }
    }
    length = i - list->currentPosition;
    memcpy(parameter, list->buffer + list->currentPosition, length);
    parameter[length] = '\0';

    list->currentPosition += length + 1;
    return length;

}
void help(const char * commandBuffer) {
    int i;
    int count;
    int x, y;
    int length, maxCommandLength = 0;

    count = sizeof(commandTable) / sizeof(struct ShellCommandEntry);
    for(i = 0; i < count; i++) {
        printf("%s", commandTable[i].command);
        printf(" - %s\n", commandTable[i].help);
    }
}
void testTask2() {
    int i = 0, offset;
    struct CFSTaskControlBlock * runningTask;
    runningTask = getRunningTask();
    offset = (runningTask->id & 0x7FFFFFFF);
    offset = VGA_HEIGHT * VGA_WIDTH - (offset % (VGA_HEIGHT * VGA_WIDTH));
    int x = offset % VGA_WIDTH, y = offset / VGA_WIDTH;
    char data[4] = {'-', '\\', '|', '/'};
    while(1) {
        terminal_putentryat(data[i], vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), x, y);
        i = (i+1) % 4;
        schedule();
    }
}
void testTask1() {
    uint8_t data;
    int i = 0, x = 0, y = 0, margin;
    struct CFSTaskControlBlock * runningTask;
    runningTask = getRunningTask();
    margin = (runningTask->id & 0x7FFFFFFF) % 10 + 1;
    while(1) {
        switch(i) {
        case 0:
            x++;
            if(x >= VGA_WIDTH - margin)
                i = 1;
            break;
        case 1:
            y++;
            if(y >= VGA_HEIGHT - margin)
                i = 2;
            break;
        case 2:
            x--;
            if(x < margin)
                i = 3;
            break;
        case 3:
            y--;
            if(y < margin)
                i = 0;
            break;
        }
        terminal_putentryat(data, vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), x, y);
        data++;
        schedule();
    }
}

void createTestTask(const char * commandBuffer) {
    struct ParameterList list;
    char typeString[30];
    char countString[30];
    char niceString[30];
    
    initializeParameter(&list, commandBuffer);
    getNextParameter(&list, typeString);
    getNextParameter(&list, countString);
    getNextParameter(&list, niceString);

    int type = atoi(typeString);
    int count = atoi(countString);
    int nice = atoi(niceString);
    
    switch(type) {
    case 1:
        for(int i = 0; i < count; i++) {
            if(createTask(0, testTask1, nice) == 0) {
                printf("[!] Something Wrong\n");
            }
        }
        printf("[+] Task1 %d Created\n", count);
        break;
    case 2:
        for(int i = 0; i < count; i++) {
            if(createTask(0, testTask2, nice) == 0) {
                printf("[!] Something Wrong\n");
            }
        }
        printf("[+] Task2 %d Created\n", count);
        break;
    default:
        printf("[!] There is no Task%d\n", type);
    }
}
