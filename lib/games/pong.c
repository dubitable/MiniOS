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
    p.active = 0;
    p.speed = 5;

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
    out->p1.type = PLAYER_1;

    out->p2 = init_player(point2(0.8, 0), W, H);
    out->p2.type = PLAYER_2;

    out->ball.pos = screen(point2(0, 0), W, H);

    out->ball.vel = point2(0, 0);
    out->ball.radius = 4;
    out->ball.dev = 2.0f;
    out->ball.acc = 1.0005f;

    out->W = W;
    out->H = H;

    out->is_start = 1;

    return out;
};

void draw_player(Player p, Color color)
{
    DrawRectangle(p.pos.x, p.pos.y, p.w, p.h, color);
}

void draw_ball(Ball ball)
{
    DrawCircle(ball.pos.x, ball.pos.y, ball.radius, WHITE);
}

void draw_score(Point2D pos, int score, Color color)
{
    char buffer[3];
    snprintf(buffer, 3, "%d", score);
    DrawText(buffer, pos.x, pos.y, 20, color);
}

void draw_scores(PongState *state)
{
    draw_score(screen(point2(-0.5, 0.9), state->W, state->H), state->p1.score, state->p1.active ? BLUE : WHITE);
    draw_score(screen(point2(0.5, 0.9), state->W, state->H), state->p2.score, state->p2.active ? RED : WHITE);
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
        float dev = rand_range(-ball->dev, ball->dev);
        ball->vel.x = -ball->vel.x;
        ball->vel.y += dev;
    }

    if (ball->pos.y - ball->radius <= 0 || ball->pos.y + ball->radius >= state->H)
    {
        float dev = rand_range(-ball->dev, ball->dev);
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
    ball->vel = mul2(ball->vel, ball->acc);
}

void auto_goto(Player *p, int goal)
{
    if (p->pos.y < goal)
    {
        p->pos.y += p->speed;
    }

    if (p->pos.y > goal)
    {
        p->pos.y -= p->speed;
    }
}

float predict_ball_y(Ball b, float target_x, int H)
{
    if (b.vel.x == 0)
        return b.pos.y;

    float time = (target_x - b.pos.x) / b.vel.x;

    float y = b.pos.y + b.vel.y * time;

    int period = 2 * H;
    float mod = fmodf(y, period);
    if (mod < 0)
        mod += period;

    if (mod > H)
        mod = period - mod;

    return mod;
}

void auto_player(Player *p, Ball b, int H)
{
    if ((b.vel.x > 0 && p->type == PLAYER_2) ||
        (b.vel.x < 0 && p->type == PLAYER_1))
    {
        float predicted = predict_ball_y(b, p->pos.x, H);

        float target = predicted - p->h / 2;

        auto_goto(p, target);
    }
}

char *cpu = "CPU";
void draw_cpu(Player p)
{
    DrawText(cpu, p.pos.x - MeasureText(cpu, 10) / 2 + p.w / 2, p.pos.y + p.h + 10, 10, WHITE);
}

char *space = "<Press Space to Start>";

void window_pong(PongState *state)
{
    BeginDrawing();
    ClearBackground(BLACK);

    bool ball_moving = (state->ball.vel.x != 0 || state->ball.vel.y != 0);

    draw_player(state->p1, state->p1.active ? BLUE : WHITE);
    draw_player(state->p2, state->p2.active ? RED : WHITE);

    if (state->is_start && !state->p1.active)
    {
        draw_cpu(state->p1);
    }

    if (state->is_start && !state->p2.active)
    {
        draw_cpu(state->p2);
    }

    draw_ball(state->ball);

    draw_scores(state);

    update_pong(state);

    if (!ball_moving)
    {
        int space_size = MeasureText(space, 20);
        DrawText(space, state->W / 2 - space_size / 2, state->H / 2 * 1.5, 20, WHITE);

        if (IsKeyDown(KEY_SPACE))
        {
            state->ball.vel = random_vel();
            state->is_start = 0;
        }
    }

    if (IsKeyDown(KEY_W))
    {
        if (!state->p1.active && state->is_start)
        {
            state->p1.active = 1;
        }
        if (state->p1.active)
        {
            state->p1.pos.y -= state->p1.speed;
        }
    }

    if (IsKeyDown(KEY_S))
    {
        if (!state->p1.active && state->is_start)
        {
            state->p1.active = 1;
        }

        if (state->p1.active)
        {
            state->p1.pos.y += state->p1.speed;
        }
    }

    if (IsKeyDown(KEY_UP))
    {
        if (!state->p2.active && state->is_start)
        {
            state->p2.active = 1;
        }

        if (state->p2.active)
        {
            state->p2.pos.y -= state->p2.speed;
        }
    }

    if (IsKeyDown(KEY_DOWN))
    {
        if (!state->p2.active && state->is_start)
        {
            state->p2.active = 1;
        }

        if (state->p2.active)
        {
            state->p2.pos.y += state->p2.speed;
        }
    }

    if (!state->p1.active)
    {
        if (ball_moving)
        {
            auto_player(&state->p1, state->ball, state->H);
        }
        else
        {
            auto_goto(&state->p1, state->H / 2 - state->p1.h / 2);
        }
    }

    if (!state->p2.active)
    {
        if (ball_moving)
        {
            auto_player(&state->p2, state->ball, state->H);
        }
        else
        {
            auto_goto(&state->p2, state->H / 2 - state->p2.h / 2);
        }
    }

    EndDrawing();
}