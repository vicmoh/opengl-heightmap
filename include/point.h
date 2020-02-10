#ifndef POINT_H
#define POINT_H
#define GL_NORMAL GL_FLAT

#include <stdio.h>
#include <stdlib.h>
#include "array_map.h"
#include "dynamic_string.h"
#include "file_reader.h"
#include "splitter.h"

enum SphereType { PLANES, VERTICES, NORMALS, NORMAL_LINES, HEIGHT_MAP };

/**
 * Attribute of the point class object.
 */
typedef struct {
  long double x, y, z;
  String toString;
} Point;

/**
 * Constructor for creating a new point.
 * @param x-axis of the vertex.
 * @param y-axis of the vertex.
 * @param z-axis of the vertex.
 * @return allocated point.
 */
Point* new_Point(double x, double y, double z);

/**
 * Destroyer for freeing the point object.
 */
void free_Point(Point* this);

/**
 * Calculate the height map based on the RGB
 * value.
 * @param rgbVal of the to be converted.
 * @return the new converted value
 */
double calcPointHeightBasedOnRGB(double rgbVal);

/**
 * Get the spehere vertices based of the latitude, longitude,
 * and the radius of the sphere.
 * @param radius of the sphere.
 * @param lats of the sphere.
 * @param longs of the sphere.
 * @param type of the vertex data to get.
 * @return an allocated array of vertices of the spheres.
 */
Array* getSphereVertices(double radius, double lats, double longs,
                         enum SphereType type, Array* rgb);

/**
 * Get the RGB value of the PGM file.
 * @param pgmFile include the path.
 * @return return allocated array of the RGB.
 */
Array* getRGBFromFile(String pgmFile);

#endif
