#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED

#include "../geometry.h"

typedef struct
{
    Point2D pos;
    int w;
    int h;
    float score;
} Player;

typedef struct
{
    Point2D pos;
    Point2D vel;
    int radius;
} Ball;

typedef struct
{
    Player p1;
    Player p2;
    Ball ball;
    int W;
    int H;
} PongState;

PongState *init_pong(int, int);
void window_pong(PongState *);

#endif