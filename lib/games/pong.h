#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED

#include "../geometry.h"

typedef enum
{
    PLAYER_1,
    PLAYER_2
} PlayerType;

typedef struct
{
    Point2D pos;
    int w;
    int h;
    float score;
    int active;
    int speed;
    PlayerType type;
} Player;

typedef struct
{
    Point2D pos;
    Point2D vel;
    int radius;
    float dev;
    float acc;
} Ball;

typedef struct
{
    Player p1;
    Player p2;
    Ball ball;

    int is_start;
    int W;
    int H;
} PongState;

PongState *init_pong(int, int);
void window_pong(PongState *);

#endif