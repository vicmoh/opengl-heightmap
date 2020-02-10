// Derived from scene.c in the The OpenGL Programming Guide.

// Linux OpenGL Headers.
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// Standard library
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MacOS OpenGL Headers.
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// My library.
#include "array_map.h"
#include "dynamic_string.h"
#include "point.h"

/* -------------------------------------------------------------------------- */
/*                        Structs and object attribute                        */
/* -------------------------------------------------------------------------- */

typedef struct {
  bool lineDrawing, lighting, drawNormals, heightmap, drawDots, smoothShade;
} ShowAttribute;

/* -------------------------------------------------------------------------- */
/*                              Global Variables                              */
/* -------------------------------------------------------------------------- */

/// Global colors.
const GLfloat BLUE[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat RED[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat DARK_RED[] = {0.3, 0.0, 0.0, 1.0};
const GLfloat GREEN[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};
const GLfloat GRAY[] = {0.8, 0.8, 0.8, 1.0};
const GLfloat DARK_GRAY[] = {0.3, 0.3, 0.3, 1.0};

// flags used to control the appearance of the image.
ShowAttribute g_attribute = {
    .lineDrawing = false,  // Draw polygons as solid or lines.
    .lighting = true,      // Use diffuse and specular lighting.
    .drawNormals = false,  // Draw normals on object.
    .heightmap = false,    // Use heightmap to move vertices when == 1.
    .drawDots = false,     // Draw only vertices when == 1.
    .smoothShade = true};  // Use normal vertices when ==1.

// Used to rotate object in update().
float g_rotate = 0.0;

// Options selections
int g_optionSelected = 1;

// The sphere vertices and normals global variables.
Array* g_rgbValues = null;
Array* g_sphereHeightMap = null;
Array* g_sphereHeightMapNormal = null;
Array* g_sphereHeightMapNormalLines = null;
Array* g_sphereVertices = null;
Array* g_sphereNormals = null;
Array* g_sphereNormalLines = null;
const int g_sphereRadius = 1;
const double g_sphereNumOfPoly = 50;

/* -------------------------------------------------------------------------- */
/*                              OpenGL Functions                              */
/* -------------------------------------------------------------------------- */

/**
 * Function to reset the attribute of the material.
 */
ShowAttribute resetAttribute() {
  ShowAttribute new = {
      .lineDrawing = false,
      .lighting = true,
      .drawNormals = false,
      .heightmap = false,
      .drawDots = false,
      .smoothShade = true,
  };
  return new;
}

void drawShadedSphere(double r, double lats, double longs, bool isSmooth) {
  double minU = 0;
  double maxU = 2 * M_PI;
  double minV = -1 * M_PI / 2;
  double maxV = M_PI / 2;
  double offsetU = (minU + maxU) / 2;
  double offsetV = (minV + maxV) / 2;
  // Loop through the latitude.
  loop(i, 0, lats) {
    loop(j, 0, longs) {
      double u = minU + (i * offsetU);
      double v = minV + (j * offsetV);
      double x1 = r * cos(v) * cos(u);
      double y1 = r * cos(v) * sin(u);
      double z1 = r * sin(v);
      double x2 = r * cos(v) * cos(u + offsetU);
      double y2 = r * cos(v) * sin(u + offsetU);
      double z2 = r * sin(v);
      double x3 = r * cos(v + offsetV) * cos(u + offsetU);
      double y3 = r * cos(v + offsetV) * sin(u + offsetU);
      double z3 = r * sin(v + offsetV);
      double x4 = r * cos(v + offsetV) * cos(u);
      double y4 = r * cos(v + offsetV) * sin(u);
      double z4 = r * sin(v + offsetV);
      // Draw the sphere.
      glBegin(GL_QUADS);
      // First
      glNormal3f(x1, y1, z1);
      glVertex3f(x1, y1, z1);
      // Second
      if (isSmooth) glNormal3f(x2, y2, z2);
      glVertex3f(x2, y2, z2);
      // Third
      if (isSmooth) glNormal3f(x3, y3, z3);
      glVertex3f(x3, y3, z3);
      // Fourth
      if (isSmooth) glNormal3f(x4, y4, z4);
      glVertex3f(x4, y4, z4);
      glEnd();
    }
  }
}

/**
 *  Draw sphere based of the type.
 * @param type.
 */
void drawSphere(enum SphereType type) {
  const bool SHOW_PRINT = false;
  String debug = $("drawSphere():");
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices.
  int next = 0;
  loop(x, 0, g_sphereNumOfPoly) {
    // Initialize and draw type based on the types.
    if (type == PLANES || type == HEIGHT_MAP)
      glBegin(GL_QUAD_STRIP);
    else if (type == VERTICES)
      glBegin(GL_POINTS);
    glMaterialfv(GL_FRONT, GL_AMBIENT, DARK_GRAY);

    // draw
    loop(y, 0, g_sphereNumOfPoly) {
      // Determine which to draw depending.
      Array* vertices =
          (type == HEIGHT_MAP) ? g_sphereHeightMap : g_sphereVertices;
      Array* normals =
          (type == HEIGHT_MAP) ? g_sphereHeightMapNormal : g_sphereNormals;

      // Draw first vertices.
      Point* point1 = Array_get(vertices, next);
      Point* norm1 = Array_get(normals, next);
      glNormal3f(norm1->x, norm1->y, norm1->z);
      glVertex3f(point1->x, point1->y, point1->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point1->toString);
      next++;

      // Draw the second vertices.
      Point* point2 = Array_get(vertices, next);
      Point* norm2 = Array_get(normals, next);
      glNormal3f(norm2->x, norm2->y, norm2->z);
      glVertex3f(point2->x, point2->y, point2->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point2->toString);
      next++;
    }

    // End drawing based on the types.
    if (type == PLANES || type == VERTICES || type == HEIGHT_MAP) glEnd();
  }
  dispose(debug);
}

void drawSphereNormalLines(Array* vertices) {
  const bool SHOW_PRINT = false;
  String debug = $("drawSphere():");
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices
  int next = 0;
  glBegin(GL_LINES);
  glMaterialfv(GL_FRONT, GL_AMBIENT, GREEN);
  loop(x, 0, g_sphereNumOfPoly) {
    loop(y, 0, g_sphereNumOfPoly) {
      // Get the normals of and draw lines.
      Point* point1 = Array_get(vertices, next);
      glVertex3f(0, 0, 0);
      glVertex3f(point1->x, point1->y, point1->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point1->toString);
      next++;

      // Get the normals of and draw lines.
      Point* point2 = Array_get(vertices, next);
      glVertex3f(0, 0, 0);
      glVertex3f(point2->x, point2->y, point2->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point2->toString);
      next++;
    }
  }
  glEnd();
  dispose(debug);
}

/**
 * Initialize material property and light source.
 */
void init(void) {
  GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1.0};
  // GLfloat light_specular[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat light_ambient1[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse1[] = {0.4, 0.4, 0.4, 1.0};
  // GLfloat light_specular1[] = {0.9, 0.9, 0.9, 1.0};
  GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};
  // Initialize the light positions.
  GLfloat light_position[] = {10.0, 10.0, 10.0, 0.0};
  GLfloat light_position2[] = {-5.0, -5.0, -5.0, 0.0};

  // If lighting is turned on then use ambient, diffuse and specular
  // lights, otherwise use ambient lighting only.
  if (g_attribute.lighting == true) {
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_full_on);
  } else {
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_off);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_full_on);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_full_off);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_full_off);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_full_on);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_full_off);
  }
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

  // Enable lightings.
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
}

/**
 * Display function to be called as the callback
 * for render the OpenGL in a loop.
 */
void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Draw surfaces as smooth shaded.
  if (g_attribute.smoothShade == true)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_NORMAL);
  // Draw polygons as either solid or outlines.
  if (g_attribute.lineDrawing == true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Give all objects the same shininess value.
  glMaterialf(GL_FRONT, GL_SHININESS, 128);
  // Set starting location of objects.
  glPushMatrix();
  glTranslatef(0.0, 0.0, -7.0);
  // Rotate around the y axis, angle or rotation (rot) modified in
  // the update() function.
  glRotatef(g_rotate, 0.0, 1.0, 0.0);
  // Set polygon colour.
  glMaterialfv(GL_FRONT, GL_AMBIENT, DARK_GRAY);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, GRAY);
  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
  // set point size so vertices are visible.
  glPointSize(5.0);

  if (g_optionSelected == 1)
    drawSphere(PLANES);
  else if (g_optionSelected == 2)
    drawSphere(PLANES);
  else if (g_optionSelected == 3)
    drawSphere(PLANES);
  else if (g_optionSelected == 4)
    drawSphere(VERTICES);
  else if (g_optionSelected == 5)
    drawSphere(PLANES);
  else if (g_optionSelected == 6) {
    drawSphere(PLANES);
    drawSphereNormalLines(g_sphereNormalLines);
  } else if (g_optionSelected == 7) {
    drawSphere(HEIGHT_MAP);
    drawSphereNormalLines(g_sphereHeightMapNormalLines);
  }

  // Flush and pop matrix.
  glPopMatrix();
  glFlush();
}

/**
 * Reshape for the windows size callback.
 * @param w is the width of the window.
 * @param h is the heigh of the window.
 */
void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/**
 * Function for the keyboard control callback.
 * @param key is the button pressed.
 * @param x-axis of the mouse location.
 * @param y-axis of the mouse location.
 */
void keyboardControl(unsigned char key, int x, int y) {
  switch (key) {
    // Esc key
    case 27:

    // Quit
    case 'q':
      exit(0);
      break;

    case '1':  // Draw polygons as outlines.
      g_optionSelected = 1;
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = true;
      init();
      display();
      break;

    case '2':  // Draw polygons as filled but not shaded (ambient only).
      g_optionSelected = 2;
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.smoothShade = false;
      g_attribute.lighting = false;
      init();
      display();
      break;

    case '3':  // Diffuse and specular lighting, smooth shadows.
      g_optionSelected = 3;
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.lighting = true;
      g_attribute.smoothShade = true;
      init();
      display();
      break;

    case '4':  // Draw vertices only, no polygons when == 1.
      g_optionSelected = 4;
      g_attribute = resetAttribute();
      if (g_attribute.drawDots == false) {
        g_attribute.drawDots = true;
        g_attribute.lighting = false;
        g_attribute.smoothShade = false;
      } else
        g_attribute.drawDots = false;
      init();
      display();
      break;

    case '5':  // Flat shade, use only one normal.
      g_optionSelected = 5;
      g_attribute = resetAttribute();
      if (g_attribute.smoothShade == false) {
        g_attribute.smoothShade = true;
      } else
        g_attribute.smoothShade = false;
      init();
      display();
      break;

    case '6':  // Draw normals to points when == 1.
      g_optionSelected = 6;
      g_attribute = resetAttribute();
      if (g_attribute.drawNormals == false)
        g_attribute.drawNormals = true;
      else
        g_attribute.drawNormals = false;
      init();
      display();
      break;

    case '7':  // Add height map to sphere when == 1.
      g_optionSelected = 7;
      g_attribute = resetAttribute();
      if (g_attribute.heightmap == false)
        g_attribute.heightmap = true;
      else
        g_attribute.heightmap = false;
      init();
      display();
      break;
  }
}

/**
 * Function to rotate the spheres.
 */
void update() {
  g_rotate += 1.0;
  display();
}

/* -------------------------------------------------------------------------- */
/*                                Main Function                               */
/* -------------------------------------------------------------------------- */

/**
 * Main Loop.
 * Open window with initial window size, title bar,
 * RGBA display mode, and handle input events.
 * @param argc is the number of arguments.
 * @param argv is list of argument values.
 */
int main(int argc, char** argv) {
  const bool SHOW_DEBUG = false;
  printf("Running script...\n\n");

  // Check if argument exist.
  if (argc == 0 || argv[1] == null) {
    printf(
        "NO ARGUMENT FOUND! PLEASE SPECIFY ARGUMENT. Please read the README "
        "provided for more information.\n");
    printf("\nScript complete.\n");
    return 0;
  }

  // Print the RGB values for debugging.
  printf("Running on file \"%s\".\n\n", argv[1]);
  g_rgbValues = getRGBFromFile(argv[1]);
  if (SHOW_DEBUG)
    for_in(next, g_rgbValues)
        printf("RGB[%d]: %f\n", next, *(double*)Array_get(g_rgbValues, next));

  // Initialize the vertices for the sphere before hand.
  g_sphereVertices = getSphereVertices(g_sphereRadius, g_sphereNumOfPoly,
                                       g_sphereNumOfPoly, VERTICES, null);
  g_sphereNormals = getSphereVertices(g_sphereRadius, g_sphereNumOfPoly,
                                      g_sphereNumOfPoly, NORMALS, null);

  // Sphere with the normal lines.
  g_sphereNormalLines = getSphereVertices(
      g_sphereRadius, g_sphereNumOfPoly, g_sphereNumOfPoly, NORMAL_LINES, null);

  // Height map for the plane.
  g_sphereHeightMap =
      getSphereVertices(g_sphereRadius, g_sphereNumOfPoly, g_sphereNumOfPoly,
                        HEIGHT_MAP, g_rgbValues);

  g_sphereHeightMapNormal =
      getSphereVertices(g_sphereRadius, g_sphereNumOfPoly, g_sphereNumOfPoly,
                        NORMALS, g_rgbValues);

  // Height map with normal lines.
  g_sphereHeightMapNormalLines =
      getSphereVertices(g_sphereRadius, g_sphereNumOfPoly, g_sphereNumOfPoly,
                        NORMAL_LINES, g_rgbValues);

  // Render the OpenGL.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(1024, 768);
  glutCreateWindow(argv[0]);
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboardControl);
  glutIdleFunc(update);
  glutMainLoop();

  // Free and exit.
  free_Array(g_rgbValues);
  free_Array(g_sphereVertices);
  free_Array(g_sphereNormals);
  free_Array(g_sphereNormalLines);
  free_Array(g_sphereHeightMap);
  free_Array(g_sphereHeightMapNormalLines);
  printf("\nScript complete.\n");
  return 0;
}
