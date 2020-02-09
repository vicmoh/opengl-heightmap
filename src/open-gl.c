
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linux OpenGL Headers
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// MacOS OpenGL Headers
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// My libs
#include "array_map.h"
#include "point.h"

typedef struct {
  bool lineDrawing, lighting, drawNormals, heightmap, drawDots, smoothShade;
} ShowAttribute;

GLfloat BLUE[] = {0.0, 0.0, 1.0, 1.0};
GLfloat RED[] = {1.0, 0.0, 0.0, 1.0};
GLfloat DARK_RED[] = {0.3, 0.0, 0.0, 1.0};
GLfloat GREEN[] = {0.0, 1.0, 0.0, 1.0};
GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};
GLfloat GRAY[] = {0.8, 0.8, 0.8, 1.0};
GLfloat DARK_GRAY[] = {0.3, 0.3, 0.3, 1.0};

/* flags used to control the appearance of the image */
ShowAttribute g_attribute = {
    .lineDrawing = false,  // draw polygons as solid or lines
    .lighting = true,      // use diffuse and specular lighting
    .drawNormals = false,  // draw normals on object
    .heightmap = false,    // use heightmap to move vertices when ==
    .drawDots = false,     // draw only vertices when == 1
    .smoothShade =
        true};  // use normal vertices when ==1,surface normals when ==0

/* used to rotate object in update() */
float g_rotate = 0.0;

/* used for height map */
int g_iHeight, g_iWidth, g_iDepth;
int g_image[100][100];

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

void drawSphere(double r, int lats, int longs) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphereVertices():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices
  Array* points = getSphereVertices(r, lats, longs, false);
  Array* norms = getSphereVertices(r, lats, longs, true);
  glBegin(GL_QUAD_STRIP);
  for (int x = 0; x <= lats; x++) {
    for (int y = 0; y <= longs; y++) {
      Point* eachPoint = Array_get(points, x);
      Point* eachNorm = Array_get(norms, x);
      glNormal3f(eachNorm->x, eachNorm->y, eachNorm->z);
      glVertex3f(eachPoint->x, eachPoint->y, eachPoint->z);
      printf("%s vertex: %s\n", debug, eachPoint->toString);
    }
  }
  glEnd();

  // Free
  free_Array(norms);
  free_Array(points);
}

/*
. Calculating Normals
. -------------------
. You will also need to calculate the normals for each vertex.
. The normals can be calculated by creating a vector from the
. centre of the sphere to a vertex. If the sphere has a radius
. of 1.0 then the normal will already be one unit long so you wont
. need to normalize it (divide by the length of the normal in order
. to make it one unit long). In this case the vertex is the same
. as the normal.  If your sphere is larger than one unit
. in radius then you will need to divide each vertex (x,y,z)
. by the radius to make the normal one unit in size.
*/
void drawSphereNormals(double r, int lats, int longs) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphere():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);
}

void drawSphereVertices(double r, int lats, int longs) {
  const bool SHOW_PRINT = false;
  const char debug[] = "drawSphereVertices():";
  if (SHOW_PRINT) printf("%s Invoked.\n", debug);

  // Loop through the vertices
  Array* points = getSphereVertices(r, lats, longs, false);
  glBegin(GL_POINTS);
  for (int x = 0; x <= lats; x++) {
    for (int y = 0; y <= longs; y++) {
      Point* eachPoint = Array_get(points, x);
      glVertex3f(eachPoint->x, eachPoint->y, eachPoint->z);
    }
  }
  glEnd();

  // Free
  free_Array(points);
}

/*  Initialize material property and light source.  */
void init(void) {
  GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1.0};
  // GLfloat light_specular[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat light_ambient1[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse1[] = {0.4, 0.4, 0.4, 1.0};
  // GLfloat light_specular1[] = {0.9, 0.9, 0.9, 1.0};
  GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

  GLfloat light_position[] = {10.0, 10.0, 10.0, 0.0};
  GLfloat light_position2[] = {-5.0, -5.0, -5.0, 0.0};

  /* if lighting is turned on then use ambient, diffuse and specular
     lights, otherwise use ambient lighting only */
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

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* draw surfaces as smooth shaded */
  /* do not change this to GL_FLAT, use normals for flat shading */
  if (g_attribute.smoothShade == true)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);

  /* draw polygons as either solid or outlines */
  if (g_attribute.lineDrawing == true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* give all objects the same shininess value */
  glMaterialf(GL_FRONT, GL_SHININESS, 128);

  /* set starting location of objects */
  glPushMatrix();
  glTranslatef(0.0, 0.0, -7.0);
  /* rotate around the y axis, angle or rotation (rot) modified in
     the update() function */
  glRotatef(g_rotate, 0.0, 1.0, 0.0);

  /* set polygon colour */
  glMaterialfv(GL_FRONT, GL_AMBIENT, DARK_GRAY);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, GRAY);
  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
  /* set point size so vertices are visible */
  glPointSize(5.0);

  /* Your code goes here */
  const int numOfPoly = 50;
  if (g_attribute.drawDots == true) drawSphereNormals(1, numOfPoly, numOfPoly);
  if (g_attribute.drawDots == false) drawSphere(1, numOfPoly, numOfPoly);

  /* end draw a cone */

  glPopMatrix();
  glFlush();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyboardControl(unsigned char key, int x, int y) {
  switch (key) {
    // Esc key
    case 27:

    // Quit
    case 'q':
      exit(0);
      break;

    case '1':  // draw polygons as outlines
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = true;
      init();
      display();
      break;

    case '2':  // draw polygons as filled but not shaded (ambient only)
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.smoothShade = false;
      g_attribute.lighting = false;
      init();
      display();
      break;

    case '3':  // diffuse and specular lighting, smooth shadina
      g_attribute = resetAttribute();
      g_attribute.lineDrawing = false;
      g_attribute.lighting = true;
      g_attribute.smoothShade = true;
      init();
      display();
      break;

    case '4':  // draw vertices only, no polygons when ==1
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

    case '5':  // flat shade, use only one normal
      g_attribute = resetAttribute();
      if (g_attribute.smoothShade == false) {
        g_attribute.smoothShade = true;
      } else
        g_attribute.smoothShade = false;
      init();
      display();
      break;

    case '6':  // draw normals to points when ==1
      g_attribute = resetAttribute();
      if (g_attribute.drawNormals == false)
        g_attribute.drawNormals = true;
      else
        g_attribute.drawNormals = false;
      init();
      display();
      break;

    case '7':  // add height map to sphere when ==1
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

void update() {
  g_rotate += 1.0;
  display();
}

/*  Main Loop
 *  Open window with initial window size, title bar,
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv) {
  const bool SHOW_DEBUG = true;
  if (SHOW_DEBUG) printf("Running script...\n\n");
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
  if (SHOW_DEBUG) printf("\n\nScript complete.");
  return 0;
}
