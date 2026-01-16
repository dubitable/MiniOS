#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "../geometry.h"

typedef struct
{
    float dt;
    float dz;
    float angle;
    Point3D points[8];
    int edges[12][2];
    Point3D center;
} CubeState;

CubeState *init_cube(int FPS);
void window_welcome(CubeState *state, int W, int H);

#endif