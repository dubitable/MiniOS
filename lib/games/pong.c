#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pong.h"
#include "../geometry.h"

#include "raylib.h"

Player init_player(Point2D point, int W, int H)
{
    Player p;
    p.pos = screen(point, W, H);

    p.w = 8;
    p.h = 80;

    p.pos.x -= p.w / 2;
    p.pos.y -= p.h / 2;

    p.score = 0;

    return p;
}

float rand_range(float min, float max)
{
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

Point2D random_vel()
{
    int side = (int)(rand() % 2);

    double angle = rand_range(-M_PI * 0.25, M_PI * 0.25);

    Point2D vel = point2(cos(angle), sin(angle));

    vel = mul2(vel, 4);

    if (side)
        vel.x = -vel.x;

    return vel;
}

PongState *init_pong(int W, int H)
{

    PongState *out = malloc(sizeof(PongState));

    out->p1 = init_player(point2(-0.8, 0), W, H);
    out->p2 = init_player(point2(0.8, 0), W, H);

    out->ball.pos = screen(point2(0, 0), W, H);

    out->ball.vel = point2(0, 0);
    out->ball.radius = 4;

    out->W = W;
    out->H = H;

    return out;
};

void draw_player(Player p)
{
    DrawRectangle(p.pos.x, p.pos.y, p.w, p.h, WHITE);
}

void draw_ball(Ball ball)
{
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, WHITE);
}

void draw_score(Point2D pos, int score)
{
    char buffer[3];
    snprintf(buffer, 3, "%d", score);
    DrawText(buffer, pos.x, pos.y, 20, WHITE);
}

void draw_scores(PongState *state)
{
    draw_score(screen(point2(-0.5, 0.9), state->W, state->H), state->p1.score);
    draw_score(screen(point2(0.5, 0.9), state->W, state->H), state->p2.score);
}

int intersect(Player p, Ball b)
{
    float closestX = fmaxf(p.pos.x, fminf(b.pos.x, p.pos.x + p.w / 2));
    float closestY = fmaxf(p.pos.y, fminf(b.pos.y, p.pos.y + p.h));

    float dx = b.pos.x - closestX;
    float dy = b.pos.y - closestY;

    return (dx * dx + dy * dy) <= (b.radius * b.radius);
}

void update_pong(PongState *state)
{
    Ball *ball = &state->ball;

    Point2D next = add2(ball->pos, ball->vel);

    Ball predicted = *ball;
    predicted.pos = next;

    if (intersect(state->p1, predicted) || intersect(state->p2, predicted))
    {
        float dev = rand_range(-2.0f, 2.0f);
        ball->vel.x = -ball->vel.x;
        ball->vel.y += dev;
    }

    if (ball->pos.y - ball->radius <= 0 || ball->pos.y + ball->radius >= state->H)
    {
        float dev = rand_range(-2.0f, 2.0f);
        ball->vel.y = -ball->vel.y;
        ball->vel.x += dev;
    }

    int outLeft = ball->pos.x - ball->radius <= 0;
    int outRight = ball->pos.x + ball->radius >= state->H;

    if (outLeft || outRight)
    {
        if (outLeft)
        {
            state->p2.score += 1;
        };

        if (outRight)
        {
            state->p1.score += 1;
        }

        ball->pos = screen(point2(0, 0), state->W, state->H);
        ball->vel = point2(0, 0);
    }

    ball->pos = add2(ball->pos, ball->vel);
    ball->vel = mul2(ball->vel, 1.0005f);
}

char *space = "<Press Space to Start>";

void window_pong(PongState *state)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_player(state->p1);
    draw_player(state->p2);

    draw_ball(state->ball);

    draw_scores(state);

    update_pong(state);

    if (state->ball.vel.x == 0 && state->ball.vel.y == 0)
    {
        int space_size = MeasureText(space, 20);
        DrawText(space, state->W / 2 - space_size / 2, state->H / 2 * 1.5, 20, WHITE);

        if (IsKeyDown(KEY_SPACE))
        {
            state->ball.vel = random_vel();
        }
    }

    if (IsKeyDown(KEY_W))
    {
        state->p1.pos.y -= 5;
    }

    if (IsKeyDown(KEY_S))
    {
        state->p1.pos.y += 5;
    }

    if (IsKeyDown(KEY_UP))
    {
        state->p2.pos.y -= 5;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        state->p2.pos.y += 5;
    }

    EndDrawing();
}