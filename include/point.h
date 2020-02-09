#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include "array_map.h"
#include "dynamic_string.h"
#include "file_reader.h"
#include "splitter.h"

enum SphereType { PLANES, VERTICES, NORMALS, HEIGHT_MAPS };

typedef struct {
  long double x, y, z;
  String toString;
} Point;

Point* new_Point(double x, double y, double z);
void free_Point(Point* this);
Array* getSphereVertices(double r, double lats, double longs,
                         enum SphereType type);

#endif
