#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

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

Point2D point2(float x, float y);
Point3D point3(float x, float y, float z);

Point2D add2(Point2D a, Point2D b);
Point2D sub2(Point2D a, Point2D b);
Point2D mul2(Point2D a, float b);

Point3D add3(Point3D a, Point3D b);
Point3D sub3(Point3D a, Point3D b);

Point3D rotate_xz(Point3D point, float angle);

Point2D project(Point3D point);
Point2D screen(Point2D point, int w, int h);

void draw_line(Point2D point1, Point2D point2);
void draw_point(Point2D point);

#endif