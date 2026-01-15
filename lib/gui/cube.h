#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

typedef struct
{
    float x;
    float y;
} Point2D;

typedef struct
{
    float x;
    float y;
    float z;
} Point3D;

typedef struct
{
    float dt;
    float dz;
    float angle;
    Point3D points[8];
    int edges[12][2];
} CubeState;

CubeState init_cube(int FPS);
void window_welcome(CubeState *state, int W, int H);

#endif