#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include "array_map.h"
#include "dynamic_string.h"

typedef struct {
  long double x, y, z;
  String toString;
} Point;

Point* new_Point(long double x, long double y, long double z);
void free_Point(Point* this);
Array* getSphereVertices(double r, double lats, double longs);

#endif
