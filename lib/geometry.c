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