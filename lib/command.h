#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "filesystem.h"
#include "gui/terminal.h"

typedef enum
{
    COMMAND_MKFILE,
    COMMAND_MKDIR,
    COMMAND_PEEK,
    COMMAND_GOTO,
    COMMAND_NONE
} Command;

typedef struct
{
    int argc;
    char **argv;
} Args;

Args *parse_line(char line[]);

Command command_from_string(const char *str);
void handle_command(TerminalState *state, Command cmd, int argc, char **argv);

#endif