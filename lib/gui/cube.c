#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "../geometry.h"

#include "raylib.h"

CubeState *init_cube(int FPS)
{
    CubeState *out = malloc(sizeof(CubeState));
    out->dt = (0.25f / FPS);
    out->dz = 0;
    out->angle = 0;

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

    Point3D center = {0, 0, 2.5f};
    out->center = center;

    memcpy(out->points, points, sizeof(points));
    memcpy(out->edges, edges, sizeof(edges));

    return out;
}

Point3D handle_rotate(Point3D point, Point3D center, Point3D offset, float angle)
{
    Point3D subbed = sub3(point, center);
    Point3D rotated = rotate_xz(subbed, angle);
    Point3D added = add3(rotated, center);
    Point3D offsetted = add3(added, offset);

    return offsetted;
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

        Point3D a = handle_rotate(state->points[state->edges[i][0]], state->center, offset, state->angle);
        Point3D b = handle_rotate(state->points[state->edges[i][1]], state->center, offset, state->angle);

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