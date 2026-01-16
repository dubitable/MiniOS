#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED

typedef struct
{
    float y;
    float score;
} Player;

typedef struct
{
    Player p1;
    Player p2;
} PongState;

PongState init_pong();
void window_pong(PongState *);

#endif