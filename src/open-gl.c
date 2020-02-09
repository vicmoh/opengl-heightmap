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

// The sphere vertices and normals global variables.
Array* g_sphereVertices = NULL;
Array* g_sphereNormals = NULL;
Array* g_sphereHeightMaps = NULL;
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

/**
 *  Draw sphere based of the type.
 * @param type.
 */
void drawSphere(enum SphereType type) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphere():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices.
  int next = 0;
  for (int x = 0; x <= g_sphereNumOfPoly; x++) {
    // Initialize and draw type based on the types.
    if (type == PLANES)
      glBegin(GL_QUAD_STRIP);
    else if (type == VERTICES)
      glBegin(GL_POINTS);

    // draw
    for (int y = 0; y <= g_sphereNumOfPoly; y++) {
      // Draw first vertices
      Point* point1 = Array_get(g_sphereVertices, next);
      Point* norm1 = Array_get(g_sphereNormals, next);
      glNormal3f(norm1->x, norm1->y, norm1->z);
      glVertex3f(point1->x, point1->y, point1->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point1->toString);
      next++;

      // Draw the second points
      Point* point2 = Array_get(g_sphereVertices, next);
      Point* norm2 = Array_get(g_sphereNormals, next);
      glNormal3f(norm2->x, norm2->y, norm2->z);
      glVertex3f(point2->x, point2->y, point2->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, point2->toString);
      next++;
    }

    // End drawing based on the types.
    if (type == PLANES || type == VERTICES) glEnd();
  }
}

void drawSphereHeightMaps() {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphere():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices
  int next = 0;
  glBegin(GL_LINES);
  glMaterialfv(GL_FRONT, GL_AMBIENT, GREEN);
  for (int x = 0; x <= g_sphereNumOfPoly; x++) {
    for (int y = 0; y <= g_sphereNumOfPoly; y++) {

      // Get the normals of and draw lines.
      Point* hmp = Array_get(g_sphereHeightMaps, next);
      glVertex3f(0, 0, 0);
      glVertex3f(hmp->x, hmp->y, hmp->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, hmp->toString);
      next++;

      // Get the normals of and draw lines.
      Point* hmp2 = Array_get(g_sphereHeightMaps, next);
      glVertex3f(0, 0, 0);
      glVertex3f(hmp2->x, hmp2->y, hmp2->z);
      if (SHOW_PRINT) printf("%s vertex: %s\n", debug, hmp2->toString);
      next++;
    }
  }
  glEnd();
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
  // Do not change this to GL_FLAT, use normals for flat shading.
  if (g_attribute.smoothShade == true)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);

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

  // Draw and render the sphere.
  if (g_attribute.drawNormals == true) {
    drawSphere(PLANES);
    drawSphereHeightMaps();
  } else if (g_attribute.drawDots == true)
    drawSphere(VERTICES);
  else if (g_attribute.drawDots == false)
    drawSphere(PLANES);

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
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = true;
      init();
      display();
      break;

    case '2':  // Draw polygons as filled but not shaded (ambient only).
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.smoothShade = false;
      g_attribute.lighting = false;
      init();
      display();
      break;

    case '3':  // Diffuse and specular lighting, smooth shadows.
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.lighting = true;
      g_attribute.smoothShade = true;
      init();
      display();
      break;

    case '4':  // Draw vertices only, no polygons when == 1.
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
      g_attribute = resetAttribute();
      if (g_attribute.smoothShade == false) {
        g_attribute.smoothShade = true;
      } else
        g_attribute.smoothShade = false;
      init();
      display();
      break;

    case '6':  // Draw normals to points when == 1.
      g_attribute = resetAttribute();
      if (g_attribute.drawNormals == false)
        g_attribute.drawNormals = true;
      else
        g_attribute.drawNormals = false;
      init();
      display();
      break;

    case '7':  // Add height map to sphere when == 1.
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
  const bool SHOW_DEBUG = true;
  if (SHOW_DEBUG) printf("Running script...\n\n");

  // Initialize the vertices for the sphere before hand.
  g_sphereVertices = getSphereVertices(g_sphereRadius, g_sphereNumOfPoly,
                                       g_sphereNumOfPoly, VERTICES);
  g_sphereNormals = getSphereVertices(g_sphereRadius, g_sphereNumOfPoly,
                                      g_sphereNumOfPoly, NORMALS);
  g_sphereHeightMaps = getSphereVertices(g_sphereRadius, g_sphereNumOfPoly,
                                         g_sphereNumOfPoly, HEIGHT_MAPS);

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
  free_Array(g_sphereVertices);
  free_Array(g_sphereNormals);
  free_Array(g_sphereHeightMaps);
  if (SHOW_DEBUG) printf("\n\nScript complete.");
  return 0;
}
