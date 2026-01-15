#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cube.h"
#include "terminal.h"

#include "../command.h"
#include "../filesystem.h"
#include "raylib.h"

void initialize_window(Context *ctx)
{
    const int W = 500;
    const int H = 500;
    const int FPS = 60;

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(W, H, "Mini OS");

    SetTargetFPS(FPS);

    CubeState cube_state = init_cube(FPS);
    TerminalState terminal_state = init_terminal(ctx);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            ctx->active_window = WINDOW_TERMINAL;
        }

        else if (IsKeyDown(KEY_LEFT))
        {
            ctx->active_window = WINDOW_WELCOME;
        }

        switch (ctx->active_window)
        {
        case WINDOW_WELCOME:
            window_welcome(&cube_state, W, H);
            break;

        case WINDOW_TERMINAL:
            window_terminal(&terminal_state);
            break;

        default:
            window_welcome(&cube_state, W, H);
            break;
        }
    }

    CloseWindow();
}