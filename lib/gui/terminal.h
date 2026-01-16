#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "cube.h"
#include "../filesystem.h"

typedef struct
{
    Context *ctx;
    char path[PATH_SIZE];
    Point2D cursor;

    char input[INPUT_SIZE];
    int input_count;

    char std_out[STDOUT_SIZE];
} TerminalState;

TerminalState init_terminal(Context *ctx);
void window_terminal(TerminalState *state);
void clear_terminal(TerminalState *state);

void print(char *msg, TerminalState *state);

void peek_dir(TerminalState *state, Directory *dir, int max_depth);

#endif