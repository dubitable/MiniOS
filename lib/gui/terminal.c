#include <string.h>
#include <stdio.h>

#include "terminal.h"
#include "../command.h"
#include "../filesystem.h"
#include "raylib.h"

const int FONT_SIZE = 20;

void rec_peek_dir(TerminalState *state, Directory *dir, int depth, int max_depth)
{
    int DIRNAME_SIZE = strlen(dir->name) + 2;
    char dirname[DIRNAME_SIZE];
    snprintf(dirname, DIRNAME_SIZE, "%s\n", dir->name);
    print_stdout(dirname, state);

    if (max_depth == 0)
    {
        return;
    }

    for (int i = 0; i < dir->dir_count; i++)
    {
        for (int d = 0; d < depth; d++)
            print_stdout("    ", state);
        print_stdout("|-- ", state);
        rec_peek_dir(state, dir->directories[i], depth + 1, max_depth - 1);
    }

    for (int j = 0; j < dir->file_count; j++)
    {
        for (int d = 0; d < depth; d++)
            print_stdout("    ", state);

        int FILENAME_SIZE = strlen(dir->files[j]->name) + 2;
        char filename[FILENAME_SIZE];
        snprintf(filename, FILENAME_SIZE, "|-- %s\n", dir->files[j]->name);
        print_stdout(filename, state);
    }
}

void peek_dir(TerminalState *state, Directory *dir, int max_depth)
{
    rec_peek_dir(state, dir, 0, max_depth);
}

void clear_terminal(TerminalState *state)
{
    state->path[0] = '\0';
    where(state->ctx->active_dir, state->path, PATH_SIZE);

    Point2D point = {
        MeasureText(state->path, FONT_SIZE) * 1.2, 10};

    state->cursor = point;

    state->input_count = 0;
    state->input[0] = '\0';
}

void clear_std_out(TerminalState *state)
{
    snprintf(state->std_out, STDOUT_SIZE, "");
}

TerminalState init_terminal(Context *ctx)
{
    TerminalState out;

    out.ctx = ctx;

    clear_std_out(&out);
    clear_terminal(&out);

    return out;
}

void print_stdout(char *msg, TerminalState *state)
{
    snprintf(state->std_out, STDOUT_SIZE, "%s%s", state->std_out, msg);
}

void window_terminal(TerminalState *state)
{

    BeginDrawing();

    ClearBackground(BLACK);

    DrawText(state->path, 10, 10, FONT_SIZE, WHITE);
    DrawText(">", state->cursor.x, state->cursor.y, FONT_SIZE, WHITE);

    DrawText(state->input, state->cursor.x + MeasureText(">", FONT_SIZE) + 10, state->cursor.y, FONT_SIZE, WHITE);

    DrawText(state->std_out, 10, state->cursor.y + 30, FONT_SIZE, WHITE);

    int key = GetCharPressed();

    while (key > 0)
    {
        clear_std_out(state);

        if ((!IsKeyPressed(KEY_SEMICOLON)) && (key >= 32) && (key <= 125) && (state->input_count < INPUT_SIZE))
        {
            state->input[state->input_count] = (char)key;
            state->input[state->input_count + 1] = '\0';
            state->input_count++;
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        state->input_count--;
        if (state->input_count < 0)
        {
            state->input_count = 0;
        }

        state->input[state->input_count] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        Args *args = parse_line(state->input);
        Command cmd = command_from_string(state->input);
        handle_command(state, cmd, args->argc, args->argv);

        clear_terminal(state);
    }

    EndDrawing();
}