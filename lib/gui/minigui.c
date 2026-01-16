#include "cube.h"
#include "terminal.h"

#include "../games/pong.h"
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
    TerminalState terminal_state;
    PongState pong_state;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            DrawText(":", W - 30, H - 30, 20, WHITE);

            if (IsKeyPressed(KEY_W))
            {
                ctx->active_window = WINDOW_WELCOME;
                GetCharPressed();
            }

            if (IsKeyPressed(KEY_T))
            {
                ctx->active_window = WINDOW_TERMINAL;
                terminal_state = init_terminal(ctx);
                GetCharPressed();
            }

            if (IsKeyPressed(KEY_P))
            {
                ctx->active_window = WINDOW_PONG;
                pong_state = init_pong();
                GetCharPressed();
            }
        }

        switch (ctx->active_window)
        {
        case WINDOW_WELCOME:
            window_welcome(&cube_state, W, H);
            break;

        case WINDOW_TERMINAL:
            window_terminal(&terminal_state);
            break;

        case WINDOW_PONG:
            window_pong(&pong_state);
            break;

        default:
            window_welcome(&cube_state, W, H);
            break;
        }
    }

    CloseWindow();
}