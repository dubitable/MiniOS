#include <math.h>
#include <string.h>

#include "raylib.h"
#include "cube.h"

void draw_point(Point2D point)
{
    DrawCircle(point.x, point.y, 2, RAYWHITE);
}

void draw_line(Point2D point1, Point2D point2)
{
    DrawLine(point1.x, point1.y, point2.x, point2.y, RAYWHITE);
}

Point2D screen(Point2D point, int w, int h)
{
    Point2D out;
    out.x = (point.x + 1) / 2 * w;
    out.y = (1 - (point.y + 1) / 2) * h;
    return out;
}

Point2D project(Point3D point)
{
    Point2D out;
    out.x = point.x / point.z;
    out.y = point.y / point.z;
    return out;
}

Point3D rotate_xz(Point3D point, float angle)
{
    Point3D out;
    float c = cos(angle);
    float s = sin(angle);

    out.x = point.x * c - point.z * s;
    out.y = point.y;
    out.z = point.x * s + point.z * c;
    return out;
}

Point3D sub(Point3D a, Point3D b)
{
    Point3D out;

    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;

    return out;
}

Point3D add(Point3D a, Point3D b)
{
    Point3D out;

    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;

    return out;
}

Point3D center = {0, 0, 2.5f};

CubeState init_cube(int FPS)
{
    CubeState out;
    out.dt = (0.25f / FPS);
    out.dz = 0;
    out.angle = 0;

    Point3D points[] = {
        {0.5f, 0.5f, 2},   // top right
        {-0.5f, 0.5f, 2},  // top left
        {0.5f, -0.5f, 2},  // bottom right
        {-0.5f, -0.5f, 2}, // bottom left

        {0.5f, 0.5f, 3},   // top right
        {-0.5f, 0.5f, 3},  // top left
        {0.5f, -0.5f, 3},  // bottom right
        {-0.5f, -0.5f, 3}, // bottom left
    };

    int edges[][2] = {
        {0, 1},
        {1, 3},
        {2, 3},
        {0, 2},
        {4, 5},
        {5, 7},
        {6, 7},
        {4, 6},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}};

    memcpy(out.points, points, sizeof(points));
    memcpy(out.edges, edges, sizeof(edges));

    return out;
}

void window_welcome(CubeState *state, int W, int H)
{

    BeginDrawing();

    ClearBackground(BLACK);

    state->angle += 2 * M_PI * state->dt;
    // dz += 1 * dt;

    for (int i = 0; i < (int)(sizeof(state->edges) / sizeof(int[2])); i++)
    {
        Point3D offset = {0, 0, state->dz};
        Point3D a = add(add(rotate_xz(sub(state->points[state->edges[i][0]], center), state->angle), center), offset);
        Point3D b = add(add(rotate_xz(sub(state->points[state->edges[i][1]], center), state->angle), center), offset);
        draw_line(screen(project(a), W, H), screen(project(b), W, H));
    }

    const char *welcome = "Welcome to MiniOS";
    int fontSize = 35;

    int textWidth = MeasureText(welcome, fontSize);
    int x = (W - textWidth) / 2;
    int y = (H / 2) - 150;

    DrawText(welcome, x, y, fontSize, RAYWHITE);

    const char *by = "A Pet Project by Pierre Quereuil";
    fontSize = 15;

    textWidth = MeasureText(by, fontSize);
    x = (W - textWidth) / 2;
    y = (H / 2) + 125;

    DrawText(by, x, y, fontSize, RAYWHITE);

    EndDrawing();
}