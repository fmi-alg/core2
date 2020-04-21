#include <iostream>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <GLUI/glui.h>
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

using namespace std;
void drawAxis(double length) {
  // Disable lighting to let the axis show color all the time
  glDisable(GL_LIGHTING);

  glLineWidth(3.0); // TODO: arrowhead
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(length, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, length, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, length);
  glEnd();

  glEnable(GL_LIGHTING);
}

void filledSphere(double radius, double x, double y, double z, double r, double g, double b, double transparency = 1) {
  glPushMatrix();
  glColor4f(r, g, b, transparency);
  glTranslated(x, y, z);
  glutSolidSphere(radius, 100, 100);
  glPopMatrix();
}

void drawEdges(Box* b, int transparency) {
  glEnable(GL_BLEND);
  glLineWidth(3.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (list<Wall*>::iterator iter = b->Walls.begin(); iter != b->Walls.end(); ++iter) {
    Wall* w = *iter;
    double A[3] = {w->b->x - w->a->x, w->b->y - w->a->y, w->b->z - w->a->z};
    double B[3] = {w->c->x - w->a->x, w->c->y - w->a->y, w->c->z - w->a->z};
    double normal[3] = {A[1]*B[2] - B[1]*A[2], B[0]*A[2] - A[0]*B[2], A[0]*B[1] - A[1]*B[0]};
    glColor4f(1.0, 1.0, 1.0, transparency / 100.0);
    glBegin(GL_TRIANGLES);
    glNormal3d(normal[0], normal[1], normal[2]);
    glVertex3f(w->a->x, w->a->y, w->a->z);
    glVertex3f(w->b->x, w->b->y, w->b->z);
    glVertex3f(w->c->x, w->c->y, w->c->z);
    glEnd();
  }
  glLineWidth(1.0);
}

void drawSpheres(Box* b, int transparency) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (list<pair<Corner*, int> >::iterator iter = b->spheres.begin(); iter != b->spheres.end(); ++iter) {
    pair<Corner*, int> c = *iter;
    filledSphere(c.second, c.first->x, c.first->y, c.first->z, 1.0, 1.0, 1.0, transparency / 100.0);
  }
}

void drawQuad(Box* b, double epsilon, int val) {
  glLineWidth(1);
  switch (b->status) {
  case Box::FREE:
    glColor3f(0.25, 1, 0.25);
    break;
  case Box::STUCK:
    glColor3f(1, 0.25, 0.25);
    break;
  case Box::MIXED:
    glColor3f(1, 1, 0.25);
    if (b->width < epsilon) {
      glColor3f(0.5, 0.5, 0.5);
    }
    break;
  case Box::UNKNOWN:
    std::cout << "UNKNOWN in drawQuad" << std::endl;
    break;
  default:
    std::cerr << "Wrong Box Type" << std::endl;
    exit(1);
  }

  if (!val) {
    glPushMatrix();
    glTranslated(b->x, b->y, b->z);
    glutWireCube(b->width);
    glPopMatrix();
  }
}

void drawPath(vector<Box*>& path, double alpha[3], double beta[3]) {
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(beta[0], beta[1], beta[2]);
  for (unsigned int i = 0; i < path.size(); ++i) {
    glVertex3f(path[i]->x, path[i]->y, path[i]->z);
  }
  glVertex3f(alpha[0], alpha[1], alpha[2]);
  glEnd();
  glLineWidth(1.0);
}

void setUpView(float xy_aspect, double scale) {
  glClearColor(0.6, 0.8, 1.0, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glScalef(scale, scale, scale);
}

void reshapeView(int windowID, float& viewXYAspect) {
  glutSetWindow(windowID);
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  glViewport(tx, ty, tw, th);

  viewXYAspect = static_cast<float>(tw) / static_cast<float>(th);

  glutPostRedisplay();
}

void setLightsForWindow(int windowID) {
  GLfloat light0_ambient[] =  {0.0, 0.0, 0.0};
  GLfloat light0_diffuse[] =  {1.0, 1.0, 1.0};
  GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
  GLfloat light0_specular[] = {1.0, 1.0, 1.0};

  glutSetWindow(windowID);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glEnable(GL_DEPTH_TEST);
}
