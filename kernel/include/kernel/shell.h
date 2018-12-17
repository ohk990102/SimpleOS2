#ifndef SHELL_HEADER
#define SHELL_HEADER 1

#include <stdint.h>

#define SHELL_PROMPT    ">"
typedef void (* Command) (const char * parameter);

#pragma pack(push, 1)

struct ShellCommandEntry {
    char * command;
    char * help;
    Command function;
};

struct ParameterList {
    const char * buffer;
    int length;
    int currentPosition;
};

#pragma pack(pop)

void startShell();
void executeCommand(const char * commandBuffer);
void initializeParameter(struct ParameterList * list, const char * parameter);
int getNextParameter(struct ParameterList * list, char * parameter);

void help(const char * parameterBuffer);
void createTestTask(const char * commandBuffer);

#endif
