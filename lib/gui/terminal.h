#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "cube.h"
#include "../filesystem.h"

enum
{
    PATH_SIZE = 100,
    INPUT_SIZE = 100,
    STDOUT_SIZE = 200,
};

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

void print_stdout(char *msg, TerminalState *state);

#endif