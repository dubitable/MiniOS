#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "cube.h"
#include "../filesystem.h"

TerminalState init_terminal(Context *ctx);
void window_terminal(TerminalState *state);
void clear_terminal(TerminalState *state);

void print_stdout(char *msg, TerminalState *state);

void peek_dir(TerminalState *state, Directory *dir, int max_depth);

#endif