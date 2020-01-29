
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <math.h>
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

/* flags used to control the appearance of the image */
int g_lineDrawing = 0;  // draw polygons as solid or lines
int g_lighting = 1;     // use diffuse and specular lighting
int g_drawNormals = 0;  // draw normals on object
int g_heightmap = 0;    // use heightmap to move vertices when == 1
int g_drawDots = 0;     // draw only vertices when == 1
int g_smoothShade = 1;  // use normal vertices when ==1,surface normals when ==0

/* used to rotate object in update() */
float g_rotate = 0.0;

/* used for height map */
int g_iHeight, g_iWidth, g_iDepth;
int g_image[100][100];

/*  Initialize material property and light source.  */
void init(void) {
  GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat light_specular[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat light_ambient1[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_diffuse1[] = {0.4, 0.4, 0.4, 1.0};
  GLfloat light_specular1[] = {0.9, 0.9, 0.9, 1.0};
  GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

  GLfloat light_position[] = {10.0, 10.0, 10.0, 0.0};
  GLfloat light_position2[] = {-5.0, -5.0, -5.0, 0.0};

  /* if lighting is turned on then use ambient, diffuse and specular
     lights, otherwise use ambient lighting only */
  if (g_lighting == 1) {
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
  GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
  GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
  GLfloat darkred[] = {0.3, 0.0, 0.0, 1.0};
  GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
  GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat gray[] = {0.8, 0.8, 0.8, 1.0};
  GLfloat darkgray[] = {0.3, 0.3, 0.3, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* draw surfaces as smooth shaded */
  /* do not change this to GL_FLAT, use normals for flat shading */
  glShadeModel(GL_SMOOTH);

  /* draw polygons as either solid or outlines */
  if (g_lineDrawing == 1)
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
  glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  /* set point size so vertices are visible */
  glPointSize(5.0);

  /* Your code goes here */

  /* sample drawing - draw a cone */
  /* remove the cone code when you add your code above */
  /* set colour of cone */
  glMaterialfv(GL_FRONT, GL_AMBIENT, darkred);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  /* move to location for object then draw it */
  glutSolidCone(1.0, 2.0, 15, 15);
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

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
    case '1':  // draw polygons as outlines
      g_lineDrawing = 1;
      g_lighting = 0;
      init();
      display();
      break;
    case '2':  // draw polygons as filled but not shaded (ambient only)
      g_lineDrawing = 0;
      g_lighting = 0;
      init();
      display();
      break;
    case '3':  // diffuse and specular lighting, smooth shading
      g_lineDrawing = 0;
      g_lighting = 1;
      init();
      display();
      break;
    case '4':  // draw vertices only, no polygons when ==1
      if (g_drawDots == 0)
        g_drawDots = 1;
      else
        g_drawDots = 0;
      break;
    case '5':  // flat shade, use only one normal
      if (g_smoothShade == 0)
        g_smoothShade = 1;
      else
        g_smoothShade = 0;
      break;
    case '6':  // draw normals to points when ==1
      if (g_drawNormals == 0)
        g_drawNormals = 1;
      else
        g_drawNormals = 0;
      break;
    case '7':  // add height map to sphere when ==1
      if (g_heightmap == 0)
        g_heightmap = 1;
      else
        g_heightmap = 0;
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
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(1024, 768);
  glutCreateWindow(argv[0]);
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(update);
  glutMainLoop();
  return 0;
}
