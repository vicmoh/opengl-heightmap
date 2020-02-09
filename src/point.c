#include "point.h"

Point* new_Point(double x, double y, double z) {
  Point* this = malloc(sizeof(Point));
  this->x = x;
  this->y = y;
  this->z = z;
  this->toString = $("{x: ", _(this->x, 10), ", y: ", _(this->y, 10),
                     ", z: ", _(this->z, 10), "}");
  return this;
}

void free_Point(Point* this) {
  free(this->toString);
  free(this);
}

Array* getSphereVertices(double r, double lats, double longs,
                         enum SphereType type) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphereVertices():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  Array* points = new_Array(free_Point);

  for (int i = 0; i <= lats; i++) {
    const double stepSize = (type == HEIGHT_MAPS) ? 1.1 : 1;

    double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
    double z0 = sin(lat0) * stepSize;
    double zr0 = cos(lat0) * stepSize;

    double lat1 = M_PI * (-0.5 + (double)i / lats);
    double z1 = sin(lat1) * stepSize;
    double zr1 = cos(lat1) * stepSize;

    for (int j = 0; j <= longs; j++) {
      double lng = 2 * M_PI * (double)(j - 1) / longs;
      double x = cos(lng);
      double y = sin(lng);

      if (type == VERTICES) {
        Array_add(points, new_Point(r * x * zr0, r * y * zr0, r * z0));
        Array_add(points, new_Point(r * x * zr1, r * y * zr1, r * z1));
      } else if (type == NORMALS) {
        Array_add(points, new_Point((x * zr0), (y * zr0), z0));
        Array_add(points, new_Point((x * zr1), (y * zr1), z1));
      } else if (type == HEIGHT_MAPS) {
        Array_add(points, new_Point((x * zr0), (y * zr0), z0));
        Array_add(points, new_Point((x * zr1), (y * zr1), z1));
      }
    }
  }
  return points;
}

Array* getRGBFromPGM(String fileName) { 
  Array* listOfRGB = new_Array(free); 
  FileReader* fr = new_FileReader(fileName);

  // Loop through each line of the file.
  for_in(nextLine, fr){
    if (nextLine < 3) continue;
    String line = FileReader_getLineAt(fr, nextLine);

    // Split the line string to get each rgb color
    // in the same line.
    Splitter* tok = new_Splitter(line, " ");
    for_in(nexTok, tok) a
    free_FileReader(fr);
  }
  free_FileReader(fileName);
}