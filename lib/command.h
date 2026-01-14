#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <stdbool.h>

typedef enum
{
    COMMAND_MKFILE,
    COMMAND_MKDIR,
    COMMAND_PKDIR,
    COMMAND_GOTO,
    COMMAND_NONE
} Command;

void mini_terminal(Context *ctx);

#endif