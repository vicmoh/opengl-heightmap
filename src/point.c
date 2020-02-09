#include "point.h"

Point* new_Point(double x, double y, double z) {
  Point* this = malloc(sizeof(Point));
  this->x = x;
  this->y = y;
  this->z = z;
  this->toString =
      $("{x: ", _(this->x), ", y: ", _(this->y), ", z: ", _(this->z), "}");
  return this;
}

void free_Point(Point* this) {
  free(this->toString);
  free(this);
}

Array* getSphereVertices(double r, double lats, double longs, bool getNorm) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphereVertices():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  Array* points = new_Array(free_Point);

  for (int i = 0; i <= lats; i++) {
    double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
    double z0 = sin(lat0);
    double zr0 = cos(lat0);

    double lat1 = M_PI * (-0.5 + (double)i / lats);
    double z1 = sin(lat1);
    double zr1 = cos(lat1);

    for (int j = 0; j <= longs; j++) {
      double lng = 2 * M_PI * (double)(j - 1) / longs;
      double x = cos(lng);
      double y = sin(lng);
      if (!getNorm) {
        Array_add(points, new_Point(r * x * zr0, r * y * zr0, r * z0));
        Array_add(points, new_Point(r * x * zr1, r * y * zr1, r * z1));
      } else if (getNorm) {
        Array_add(points, new_Point(x * zr0, y * zr0, z0));
        Array_add(points, new_Point(x * zr1, y * zr1, z1));
      }
    }
  }
  return points;
}
