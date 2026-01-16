#include <math.h>

#include "geometry.h"
#include "raylib.h"

void draw_point(Point2D point)
{
    DrawCircle(point.x, point.y, 2, RAYWHITE);
}

void draw_line(Point2D point1, Point2D point2)
{
    DrawLine(point1.x, point1.y, point2.x, point2.y, RAYWHITE);
}

Point2D point2(float x, float y)
{
    Point2D point = {
        .x = x,
        .y = y};

    return point;
}

Point3D point3(float x, float y, float z)
{
    Point3D point = {
        .x = x,
        .y = y,
        .z = z};

    return point;
}

Point2D screen(Point2D point, int w, int h)
{
    return point2((point.x + 1) / 2 * w, (1 - (point.y + 1) / 2) * h);
}

Point2D project(Point3D point)
{
    return point2(point.x / point.z, point.y / point.z);
}

Point3D rotate_xz(Point3D point, float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return point3(point.x * c - point.z * s, point.y, point.x * s + point.z * c);
}

Point3D sub3(Point3D a, Point3D b)
{
    return point3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point3D add3(Point3D a, Point3D b)
{
    return point3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point2D sub2(Point2D a, Point2D b)
{
    return point2(a.x - b.x, a.y - b.y);
}

Point2D add2(Point2D a, Point2D b)
{
    return point2(a.x + b.x, a.y + b.y);
}

Point2D mul2(Point2D a, float b)
{
    return point2(a.x * b, a.y * b);
}