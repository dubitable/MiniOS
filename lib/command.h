#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

typedef enum
{
    COMMAND_MKFILE,
    COMMAND_MKDIR,
    COMMAND_PKDIR,
    COMMAND_GOTO,
    COMMAND_NONE
} CommandName;

typedef struct
{
    CommandName name;
    void (*action)(Context *ctx, int argc, char **argv);
} Command;

void mini_terminal(Context *ctx);

#endif