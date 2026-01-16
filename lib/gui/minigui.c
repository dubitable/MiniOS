#include <stdlib.h>

#include "cube.h"
#include "terminal.h"

#include "../games/pong.h"
#include "../command.h"
#include "../filesystem.h"

#include "raylib.h"

void initialize_window(Context *ctx)
{
    int W = ctx->W;
    int H = ctx->H;

    const int FPS = 60;

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(W, H, "Mini OS");

    SetTargetFPS(FPS);

    CubeState *cube_state = init_cube(FPS);
    ctx->active_data = cube_state;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            DrawText(":", W - 30, H - 30, 20, WHITE);

            if (IsKeyPressed(KEY_W))
            {
                free(ctx->active_data);
                CubeState *cube_state = init_cube(FPS);
                ctx->active_data = cube_state;
                GetCharPressed();
                ctx->active_window = WINDOW_WELCOME;
            }

            if (IsKeyPressed(KEY_T))
            {
                free(ctx->active_data);
                TerminalState *terminal_state = init_terminal(ctx);
                ctx->active_data = terminal_state;
                GetCharPressed();
                ctx->active_window = WINDOW_TERMINAL;
            }

            if (IsKeyPressed(KEY_P))
            {
                free(ctx->active_data);
                PongState *pong_state = init_pong(W, H);
                ctx->active_data = pong_state;
                GetCharPressed();
                ctx->active_window = WINDOW_PONG;
            }
        }

        switch (ctx->active_window)
        {
        case WINDOW_WELCOME:
            window_welcome(ctx->active_data, W, H);
            break;

        case WINDOW_TERMINAL:
            window_terminal(ctx->active_data);
            break;

        case WINDOW_PONG:
            window_pong(ctx->active_data);
            break;

        default:
            window_welcome(ctx->active_data, W, H);
            break;
        }
    }

    CloseWindow();
}