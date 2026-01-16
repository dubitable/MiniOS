#include <stdio.h>

#include "../filesystem.h"
#include "pong.h"
#include "raylib.h"

void window_pong(PongState *pong_state)
{
    BeginDrawing();
    ClearBackground(BLACK);

    char buffer[20];
    snprintf(buffer, 20, "%f\n", pong_state->p1.score);
    DrawText(buffer, 50, 50, 20, WHITE);

    EndDrawing();
}

PongState init_pong()
{

    PongState out;

    out.p1.score = 0;
    out.p2.score = 0;

    out.p1.y = 0;
    out.p2.y = 0;

    return out;
};
