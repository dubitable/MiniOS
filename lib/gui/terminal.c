#include <stdio.h>
#include <stdlib.h>

#include "terminal.h"
#include "../command.h"
#include "../filesystem.h"

#include "raylib.h"

const int FONT_SIZE = 20;

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

TerminalState *init_terminal(Context *ctx)
{
    TerminalState *out = malloc(sizeof(TerminalState));
    out->ctx = ctx;

    clear_std_out(out);
    clear_terminal(out);

    return out;
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

        if ((key >= 32) && (key <= 125) && (state->input_count < INPUT_SIZE))
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

        handle_command(cmd, args->argc, args->argv, state->ctx, state->std_out, STDOUT_SIZE);

        clear_terminal(state);
    }

    EndDrawing();
}