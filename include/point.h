#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long double x, y, z;
} Point;

Point* new_Point(long double x, long double y, long double z);
Point* getSphereVertices();

#endif
