#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

typedef enum
{
    COMMAND_MKFILE,
    COMMAND_MKDIR,
    COMMAND_PEEK,
    COMMAND_GOTO,
    COMMAND_NONE
} Command;

void mini_terminal(Context *ctx);

#endif