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

double calcPointHeightBasedOnRGB(double rgbVal) {
  double toBeReturn = rgbVal / 512;
  return toBeReturn;
}

Array* getSphereVertices(double r, double lats, double longs,
                         enum SphereType type, Array* rgb) {
  // Determine to show debugger.
  const bool SHOW_PRINT = false;
  String debug = $("drawSphereVertices():");
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Initialize and array of points.
  Array* points = new_Array(free_Point);
  int nextRGB = -1;

  // Loop through to create the vertices of the sphere.
  // Calculate and added to the array of points.
  for (int i = 0; i <= lats; i++) {
    // Calculate and add the points to the array
    // depending the type of sphere created.
    for (int j = 0; j <= longs; j++) {
      /// Determine if the normals is showed for the height map.
      nextRGB++;
      double stepSize = 1;
      if (type == NORMAL_LINES) stepSize = 1.1;
      if (type == NORMAL_LINES && rgb != null)
        stepSize =
            calcPointHeightBasedOnRGB(*(double*)Array_get(rgb, nextRGB)) + 1.1;

      // Calculate the size of the circle.
      double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
      double z0 = sin(lat0) * stepSize;
      double zr0 = cos(lat0) * stepSize;

      // Calculate the other half
      double lat1 = M_PI * (-0.5 + (double)i / lats);
      double z1 = sin(lat1) * stepSize;
      double zr1 = cos(lat1) * stepSize;

      // Calculate the x, y value
      double lng = 2 * M_PI * (double)(j - 1) / longs;
      double x = cos(lng);
      double y = sin(lng);

      // Case for the spehere.
      if (type == VERTICES) {
        Array_add(points, new_Point(r * x * zr0, r * y * zr0, r * z0));
        Array_add(points, new_Point(r * x * zr1, r * y * zr1, r * z1));
      } else if (type == NORMALS || type == NORMAL_LINES ||
                 type == HEIGHT_MAP) {
        Array_add(points, new_Point((x * zr0), (y * zr0), z0));
        Array_add(points, new_Point((x * zr1), (y * zr1), z1));
      }
    }
  }

  // Return the points.
  dispose(debug);
  return points;
}

Array* getRGBFromFile(String pgmFile) {
  // Debugger
  const bool SHOW_PRINT = false;
  String debug = $("getRGBFromFile(): ");

  // Initialize the arrays and file reader
  Array* listOfRGB = new_Array(free);
  FileReader* fr = new_FileReader(pgmFile);

  // Check if file exist.
  if (fr == null) {
    printf("Could not find the file. Please re-enter the FILE argument.\n");
    free_FileReader(fr);
    free_Array(listOfRGB);
    exit(0);
    return null;
  }

  // Loop through each line of the file.
  for_in(nextLine, fr) {
    if (nextLine < 3) continue;
    String line = FileReader_getLineAt(fr, nextLine);

    // Split the line string to get each rgb color
    // in the same line.
    Splitter* tok = new_Splitter(line, " ");
    for_in(nextTok, tok)
        Array_add(listOfRGB, new_Number(atof(tok->list[nextTok])));

    // Free objects.
    dispose(line);
    free_Splitter(tok);
  }

  // Print the list of RGB of the array from the file.
  if (SHOW_PRINT)
    for_in(next, listOfRGB) printf("%s RGB[%d]: %f\n", debug, next,
                                   *(double*)Array_get(listOfRGB, next));

  // Free and return.
  dispose(debug);
  free_FileReader(fr);
  return listOfRGB;
}