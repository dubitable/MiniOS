#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "command.h"
#include "filesystem.h"
#include "raylib.h"

void where(Directory *current)
{
    if (current == NULL)
    {
        return;
    }

    where(current->parent);
    printf("%s/", current->name);
}

typedef struct
{
    int argc;
    char **argv;
} Args;

Args *parse_line(char line[])
{
    Args *args = malloc(sizeof(Args));
    args->argc = 0;
    args->argv = NULL;

    char *pch = strtok(line, " ");
    while (pch != NULL)
    {
        args->argc++;

        args->argv = realloc(args->argv, args->argc * sizeof(char *));
        args->argv[args->argc - 1] = strdup(pch);

        pch = strtok(NULL, " ");
    }

    return args;
}

void free_args(Args *args)
{
    if (args == NULL)
    {
        return;
    }

    for (int i = 0; i < args->argc; i++)
    {
        free(args->argv[i]);
    }

    free(args->argv);
    free(args);
}

void mini_terminal(Context *ctx)
{
    int running = 1;

    while (running)
    {
        where(ctx->active);
        printf(" > ");

        char line[1024];

        if (fgets(line, sizeof(line), stdin))
        {
            line[strcspn(line, "\n")] = 0;
        }

        if (strcmp(line, "exit") == 0)
        {
            running = 0;
            break;
        }

        Args *args = parse_line(line);

        if (args->argc == 0)
        {
            continue;
        }

        Command cmd = command_from_string(args->argv[0]);

        handle_command(ctx, cmd, args->argc, args->argv);

        free_args(args);
    }
}

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

void spin_cube()
{
    const int W = 500;
    const int H = 500;
    const int FPS = 60;

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(W, H, "Mini Terminal");

    SetTargetFPS(FPS);

    float dt = (0.25f / FPS);
    float angle = 0;

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

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        angle += 2 * M_PI * dt;

        for (int i = 0; i < sizeof(edges) / sizeof(int[2]); i++)
        {
            Point3D a = add(rotate_xz(sub(points[edges[i][0]], center), angle), center);
            Point3D b = add(rotate_xz(sub(points[edges[i][1]], center), angle), center);
            draw_line(screen(project(a), W, H), screen(project(b), W, H));
        }

        const char *msg = "Welcome to MiniOS";
        int fontSize = 35;

        int textWidth = MeasureText(msg, fontSize);
        int x = (W - textWidth) / 2;
        int y = (H / 2) - 150;

        DrawText(msg, x, y, fontSize, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
}

void open_terminal(Context *ctx)
{
    spin_cube();
}