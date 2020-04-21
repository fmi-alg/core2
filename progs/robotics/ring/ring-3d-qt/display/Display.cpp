//==============================================================================
// Display.cpp
// =================
// This class is the widget on ControlWindow that renders the scene.
// Display generates and paints boxes, walls, and circles using OpenGL
// to allow the user to get a full understanding of the program's output.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
//         Ching-Hsiang Hsu (chhsu@nyu.edu)
// CREATED: 2015-08-02
// UPDATE:  2017-07-07
// Copyright (c) 2015 Bryant Curto, Ching-Hsiang Hsu
//==============================================================================

#include "display/Display.h"

#include <math.h>
#include <unistd.h>
#include <ctime>
#include <iostream>

extern FILE* g_fptr;

// Global Variables
extern string workingDir;
extern string fileName;
extern Point3d start;
extern Point3d goal;
extern Point3d startRot;
extern Point3d goalRot;
extern double epsilon;
extern double R;
extern int searchType;
extern int seed;
extern bool noPath;
extern bool showBox;
extern bool showBoxBoundary;
extern int animationSpeed;
extern double boxWidth;
extern double boxHeight;

extern SoftSubdivisionSearch* sss;
extern int transparency;
extern double boxTransparency;
extern double distanceZ;
extern Point3d eye;
extern Point3d at;
extern Vector up;
extern Vector eyeVector;
extern float view_rotate_angles[3];
extern Point3d obj_pos;
extern float view_rotate[16];
extern float view_rotate_x[16];
extern float view_rotate_y[16];
extern float view_rotate_z[16];
extern vector<ConfBox3d*> path;
extern bool showPath;
extern bool showTrace;
extern bool generateTrace;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern int incr;
extern unsigned int iPathSeg;
extern float inSegCount;
extern float segCount;

extern Point3d Bt;
extern Point3d Br;
extern double Btw;
extern double Brw;
extern bool showRing;
extern Triangle3d W;
extern bool showWall;
extern Segment3d E;
extern bool showEdge;
extern bool showCorner;
extern int traceDense;

// local Variables
vector<float> trace;
static GLuint sphereList;
int renderSteps = 1;
bool step = false;
Point3d preRot = startRot;

/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent) : QOpenGLWidget(parent) {
  xRot = 0;
  yRot = -45 * 16;
  zRot = 0;

  distanceX = 0.0;
  distanceY = 0.0;
  distanceZ = 900.0;

  // focus on key press event
  setFocusPolicy(Qt::StrongFocus);
}

/*
 * DESTRUCTOR
 */
Display::~Display() {}

void Display::mousePressEvent(QMouseEvent* event) {
  lastPos = event->pos();
  event->accept();
}

void Display::mouseMoveEvent(QMouseEvent* event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRot + 8 * dy);
    setYRotation(yRot + 8 * dx);

    update();
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRot + 8 * dy);
    setZRotation(zRot + 8 * dx);

    update();
  }

  lastPos = event->pos();
  event->accept();
}

void Display::wheelEvent(QWheelEvent* event) {
  int delta = event->delta();

  if (event->orientation() == Qt::Vertical) {
    if (delta < 0) {
      distanceZ *= 1.01;
    } else if (delta > 0) {
      distanceZ *= 0.99;
    }

    update();
  }

  emit zDistanceChanged(distanceZ);

  event->accept();
}

void Display::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Up) {
    distanceY += 20.0f;
    update();
  } else if (event->key() == Qt::Key_Down) {
    distanceY -= 20.0f;
    update();
  } else if (event->key() == Qt::Key_Left) {
    distanceX -= 20.0f;
    update();
  } else if (event->key() == Qt::Key_Right) {
    distanceX += 20.0f;
    update();
  }

  event->accept();
}

static void qNormalizeAngle(int& angle) {
  while (angle < 0) angle += 360 * 16;
  while (angle > 360) angle -= 360 * 16;
}

void Display::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != xRot) {
    xRot = angle;
    // emit xRotationChanged(angle);
    update();
  }
}

void Display::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != yRot) {
    yRot = angle;
    // emit yRotationChanged(angle);
    update();
  }
}

void Display::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != zRot) {
    zRot = angle;
    // emit zRotationChanged(angle);
    update();
  }
}

void resetViewRotate(float* view_rotate) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      view_rotate[i * 4 + j] = 0;
    }
    view_rotate[i * 4 + i] = 1;
  }
}

void multiplyRotationMatrix(float* view_rotate_x) {
  float tmp[16];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tmp[i * 4 + j] = 0;
      for (int k = 0; k < 4; k++) {
        tmp[i * 4 + j] += view_rotate[i * 4 + k] * view_rotate_x[k * 4 + j];
      }
    }
  }

  for (int i = 0; i < 16; i++) {
    view_rotate[i] = tmp[i];
  }
}

void resetRotationMatrix() {
  resetViewRotate(view_rotate);

  float xAng = 0;
  float yAng = 0;
  float zAng = 0;

  //    xAng = view_rotate_angles[0];
  //    yAng = view_rotate_angles[1];
  //    zAng = view_rotate_angles[2];

  // row 2:
  view_rotate_x[5] = cos(xAng);
  view_rotate_x[6] = -sin(xAng);
  // row 3:
  view_rotate_x[9] = sin(xAng);
  view_rotate_x[10] = cos(xAng);

  view_rotate_y[0] = cos(yAng);
  view_rotate_y[2] = sin(yAng);
  // row 3:
  view_rotate_y[8] = -sin(yAng);
  view_rotate_y[10] = cos(yAng);

  // row 1:
  view_rotate_z[0] = cos(zAng);
  view_rotate_z[1] = -sin(zAng);
  // row 2:
  view_rotate_z[4] = sin(zAng);
  view_rotate_z[5] = cos(zAng);

  multiplyRotationMatrix(view_rotate_z);
  multiplyRotationMatrix(view_rotate_y);
  multiplyRotationMatrix(view_rotate_x);
}

GLuint theTorus;

/* Draw a torus */
static void torus(int numc, int numt) {
  int i, j, k;
  double s, t, x, y, z, twopi;

  double r = R / 2000.0f;  // radius of the tube

  twopi = 2 * M_PI;
  for (i = 0; i < numc; i++) {
    glBegin(GL_QUAD_STRIP);
    for (j = 0; j <= numt; j++) {
      for (k = 1; k >= 0; k--) {
        s = (i + k) % numc + 0.5;
        t = j % numt;

        x = (1 + r * cos(s * twopi / numc)) * cos(t * twopi / numt);
        y = r * sin(s * twopi / numc);
        z = (1 + r * cos(s * twopi / numc)) * sin(t * twopi / numt);
        glVertex3f(x, y, z);
      }
    }
    glEnd();
  }
}

/* Create display list with Torus and initialize state */
static void initTorus(void) {
  theTorus = glGenLists(1);
  glNewList(theTorus, GL_COMPILE);
  torus(8, 25);
  glEndList();

  glShadeModel(GL_FLAT);
}
void Display::initializeGL() {
  glClearColor(0.8, 0.8, 0.8, 1.0);

  initTorus();

  GLfloat light0_ambient[] = {0.0, 0.0, 0.0};
  GLfloat light0_diffuse[] = {1.0, 1.0, 1.0};
  GLfloat light0_position[] = {512, 512, 512, 0.0f};
  GLfloat light0_specular[] = {1.0, 1.0, 1.0};

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glEnable(GL_DEPTH_TEST);

  eyeVector.set(eye.X() - at.X(), eye.Y() - at.Y(), eye.Z() - at.Z());
  resetRotationMatrix();
  obj_pos = eye;
}

void Display::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2]+distance, obj_pos[0] +
  // eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0],
  // up[1], up[2]);
  gluLookAt(0, 0, distanceZ, 0.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0f);
  glTranslatef(distanceX, distanceY, 0);
  glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

  renderScene();

  glFlush();
}

void Display::resizeGL(int w, int h) {
  if (h == 0) h = 1;

  glViewport(0, 0, w, h);       // reset the current view port
  glMatrixMode(GL_PROJECTION);  // select the model view matrix
  glLoadIdentity();             // reset the model view matrix

  // calculate the aspect ratio of the window
  gluPerspective(
      60.0f, (GLfloat)w / h, 0.001f,
      10000.0f);  // fovy : view angle, (0.1f, 10000.0f) : drawing area
  glMatrixMode(GL_MODELVIEW);  // select model view matrix
  glLoadIdentity();            // reset the model view matrix

  // glViewport(0, 0, w, h);

  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();

  // GLdouble dWt;
  // GLdouble dHt;

  // if(w > h) {
  //    dHt = 1;
  //    dWt = ((double)w) / h;
  //} else {
  //    dHt = ((double)h) / w;
  //    dWt = 1;
  //}

  // glFrustum(-dWt*0.04, dWt*0.04, -0.04, 0.04, 0.1, 15.0);
  // glOrtho(-dWt, dWt, -dHt, dHt, 5.0, 100.0);

  // gluLookAt(5.0, 2.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0],
  // obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
  // glMultMatrixf(view_rotate);

  // double scale = 2.0f/boxWidth;
  // glScalef(scale, scale, scale);
  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0],
  // obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
  // glMultMatrixf(view_rotate);
}

void Display::renderScene() {
  // Draw Obstacles
  drawObstacles(sss->pRoot, transparency);

  drawAxisBoundingBox(boxWidth * 2);
  if (showBox || showBoxBoundary) {
    drawTree(sss->pRoot);
  }
  if (showRing) {
    glPushMatrix();
    drawRing();
    glPopMatrix();
  }
  if (showWall) {
    glPushMatrix();
    drawWall();
    glPopMatrix();
  }
  if (showEdge) {
    glPushMatrix();
    drawEdge();
    glPopMatrix();
  }
  if (showCorner) {
    glPushMatrix();
    drawCorner();
    glPopMatrix();
  }

  // glPolygonMode(GL_FRONT, GL_LINE);

  float rot_mat[16];
  if (!noPath) {
    if (showPath) drawPath(path /*, start, goal*/);
    if (showTrace) {
      if (!generateTrace) {
        preRot = startRot;

        generateTrace = true;
        trace.clear();
        for (unsigned i = 0; i < path.size() - 1;) {
          // translational part
          float dx = path[i + 1]->mB.X() - path[i]->mB.X();
          float dy = path[i + 1]->mB.Y() - path[i]->mB.Y();
          float dz = path[i + 1]->mB.Z() - path[i]->mB.Z();
          float segLength = sqrt(dx * dx + dy * dy + dz * dz);
          if (segLength > 0)
            segCount = segLength / epsilon;
          else
            segCount = 8;
          float x = path[i]->mB.X() + dx / segCount * inSegCount;
          float y = path[i]->mB.Y() + dy / segCount * inSegCount;
          float z = path[i]->mB.Z() + dz / segCount * inSegCount;
          // rotational part
          bool recordCur = true;
          float curRot[3], nextRot[3];
          if (!path[i]->getRot(curRot)) {
            recordCur = false;
            curRot[0] = preRot.X();
            curRot[1] = preRot.Y();
            curRot[2] = preRot.Z();
          }
          if (!path[i + 1]->getRot(nextRot)) {
            for (int ii = 0; ii < 3; ++ii) nextRot[ii] = curRot[ii];
          }
          float rot[3];

          fprintf(stderr, "mB (%f, %f, %f) %f\n", path[i]->mB.X(),
                  path[i]->mB.Y(), path[i]->mB.Z(), path[i]->width);
          if (path[i]->rot_width != -2) {
            fprintf(stderr, "rot (%f, %f, %f) %f\n", path[i]->rot_mB.X(),
                    path[i]->rot_mB.Y(), path[i]->rot_mB.Z(),
                    path[i]->rot_width);
          }

          Rot3dSide::interpolateRot(curRot, nextRot, inSegCount, segCount, rot);
          Rot3dSide::rot2mat(rot[0], rot[1], rot[2], rot_mat);
          rot_mat[3] = x;
          rot_mat[7] = y;
          rot_mat[11] = z;
          for (int j = 0; j < 16; ++j) {
            trace.push_back(rot_mat[j]);
          }
          inSegCount++;
          if (inSegCount > segCount) {
            ++i;
            inSegCount = 1;
          }

          if (recordCur) {
            preRot.set(curRot[0], curRot[1], curRot[2]);
          }
        }
      } else {
        for (int i = 0; i < (int)trace.size(); i += traceDense) {
          double x = trace[i + 3];
          double y = trace[i + 7];
          double z = trace[i + 11];
          float rot_mat[16];
          for (int j = 0; j < 16; ++j) {
            if (j == 3 || j == 7 || j == 11)
              rot_mat[j] = 0;
            else
              rot_mat[j] = trace[i + j];
          }
          double rgb = trace.size();
          rgb = (double)i / rgb;
          if (i > 0) {
            // rod(R, x, y, z, rot_mat,
            // 0.4+0.4*(1.0f-rgb),0,0.4+0.4*(1.0f-rgb),1.0f);
            ring(R, x, y, z, rot_mat, 0.4 + 0.4 * (1.0f - rgb), 0,
                 0.4 + 0.4 * (1.0f - rgb), 1.0f);
          }
        }
        update();
      }
    }

    if (replayAnim) {
      replayAnim = false;
      showAnim = true;
      pauseAnim = false;
      incr = 0;
    }

    if (showAnim) {
      if (iPathSeg >= path.size() - 1) {
        showAnim = false;
      } else {
        float dx = path[iPathSeg + 1]->mB.X() - path[iPathSeg]->mB.X();
        float dy = path[iPathSeg + 1]->mB.Y() - path[iPathSeg]->mB.Y();
        float dz = path[iPathSeg + 1]->mB.Z() - path[iPathSeg]->mB.Z();
        float segLength = sqrt(dx * dx + dy * dy + dz * dz);
        if (segLength > 0)
          segCount = segLength / epsilon;
        else
          segCount = 8;
        float x = path[iPathSeg]->mB.X() + dx / segCount * inSegCount;
        float y = path[iPathSeg]->mB.Y() + dy / segCount * inSegCount;
        float z = path[iPathSeg]->mB.Z() + dz / segCount * inSegCount;

        bool recordCur = true;
        float curRot[3], nextRot[3];
        if (!path[iPathSeg]->getRot(curRot)) {
          recordCur = false;
          curRot[0] = preRot.X();
          curRot[1] = preRot.Y();
          curRot[2] = preRot.Z();
        }
        if (!path[iPathSeg + 1]->getRot(nextRot)) {
          for (int i = 0; i < 3; ++i) nextRot[i] = curRot[i];
        }
        float rot[3];
        Rot3dSide::interpolateRot(curRot, nextRot, inSegCount, segCount, rot);
        Rot3dSide::rot2mat(rot[0], rot[1], rot[2], rot_mat);
        // rod(R, x, y, z, rot_mat, 0.85,0,0.85,0.5);
        ring(R, x, y, z, rot_mat, 0.85, 0, 0.85, 0.5);

        if (!pauseAnim) {
          //                    fprintf(g_fptr, "box %f %f %f w %f\n",
          //                    path[iPathSeg]->x, path[iPathSeg]->y,
          //                    path[iPathSeg]->z, path[iPathSeg]->width);
          //                    if(path[iPathSeg]->rot_width != -2)
          //                      fprintf(g_fptr, "rot %f %f %f w %f\n",
          //                      path[iPathSeg]->rot_x, path[iPathSeg]->rot_y,
          //                      path[iPathSeg]->rot_z,
          //                      path[iPathSeg]->rot_width);
          //                    fprintf(g_fptr, "status %d\n",
          //                    path[iPathSeg]->status);

          usleep((99 - animationSpeed) * 500);
          inSegCount++;
          if (inSegCount > segCount) {
            iPathSeg++;
            inSegCount = 1;
          }
        }

        if (recordCur) {
          preRot.set(curRot[0], curRot[1], curRot[2]);
        }
      }
      update();
    } else {
      preRot = startRot;
    }
  }
  Rot3dSide::rot2mat(startRot.X(), startRot.Y(), startRot.Z(), rot_mat);
  // rod(R, start.X(), start.Y(), start.Z(), rot_mat, 0.4, 0, 0.4, 1);  // start
  ring(R, start.X(), start.Y(), start.Z(), rot_mat, 0.4, 0, 0.4, 1);  // start
  Rot3dSide::rot2mat(goalRot.X(), goalRot.Y(), goalRot.Z(), rot_mat);
  // rod(R, goal.X(), goal.Y(), goal.Z(), rot_mat, 0.8, 0, 0.8, 1);  // goal
  ring(R, goal.X(), goal.Y(), goal.Z(), rot_mat, 0.8, 0, 0.8, 1);  // goal
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//
/*
 * DRAW AXIS
 */
void Display::drawAxisBoundingBox(double length) {
  // Disable lighting to let the axis show color all the time
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);
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

  glDisable(GL_LIGHTING);
  glPushMatrix();
  glTranslated(256, 256, 256);
  drawCube(sss->pRoot, 512, false, true);
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 * DRAW Edge
 */
void Display::drawObstacles(ConfBox3d* b, int transparency) {
  // glDisable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (list<Wall*>::iterator iter = b->Walls.begin(); iter != b->Walls.end();
       ++iter) {
    Wall* w = *iter;
    if (strcmp(w->name.c_str(), "Bound") == 0) continue;
    Vector n(w->tri.normal());
    n.normalize();
    glColor4f(1.0, 1.0, 1.0, transparency / 100.0);

    //        if(w->a->p == Point3d(428.000000, 428.000000, 428.000000) &&
    //           w->b->p == Point3d(128.000000, 428.000000, 428.000000) &&
    //           w->c->p == Point3d(428.000000, 128.000000, 428.000000))
    //          glColor3f(1,0,0);

    glBegin(GL_TRIANGLES);
    glNormal3d(n.X(), n.Y(), n.Z());
    glVertex3f(w->a->p.X(), w->a->p.Y(), w->a->p.Z());
    glVertex3f(w->b->p.X(), w->b->p.Y(), w->b->p.Z());
    glVertex3f(w->c->p.X(), w->c->p.Y(), w->c->p.Z());
    glEnd();
  }
  glLineWidth(1.0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_DEPTH_TEST);
}

/*
 * DRAW Cube
 */
void Display::drawCube(ConfBox3d* b, double length, bool showBox,
                       bool showBoxBoundary) {
  if (showBox) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // front
    if (b->rot_width == -2 || b->rot_mB.Z() == 1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, -length / 2, length / 2);
      glVertex3f(length / 2, -length / 2, -length / 2);
      glVertex3f(length / 2, length / 2, -length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, length / 2, length / 2);
      glVertex3f(length / 2, -length / 2, length / 2);
      glVertex3f(length / 2, length / 2, -length / 2);
      glEnd();
    }

    // top
    if (b->rot_width == -2 || b->rot_mB.Y() == 1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, -length / 2, length / 2);
      glVertex3f(length / 2, -length / 2, length / 2);
      glVertex3f(length / 2, length / 2, length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, length / 2, length / 2);
      glVertex3f(-length / 2, -length / 2, length / 2);
      glVertex3f(length / 2, length / 2, length / 2);
      glEnd();
    }

    // right
    if (b->rot_width == -2 || b->rot_mB.X() == 1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, length / 2, length / 2);
      glVertex3f(length / 2, length / 2, -length / 2);
      glVertex3f(-length / 2, length / 2, -length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, length / 2, length / 2);
      glVertex3f(length / 2, length / 2, length / 2);
      glVertex3f(-length / 2, length / 2, -length / 2);
      glEnd();
    }

    // left
    if (b->rot_width == -2 || b->rot_mB.X() == -1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, -length / 2, -length / 2);
      glVertex3f(length / 2, -length / 2, length / 2);
      glVertex3f(-length / 2, -length / 2, -length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, -length / 2, length / 2);
      glVertex3f(-length / 2, -length / 2, length / 2);
      glVertex3f(-length / 2, -length / 2, -length / 2);
      glEnd();
    }

    // back
    if (b->rot_width == -2 || b->rot_mB.Z() == -1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, length / 2, -length / 2);
      glVertex3f(-length / 2, -length / 2, -length / 2);
      glVertex3f(-length / 2, -length / 2, length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, length / 2, length / 2);
      glVertex3f(-length / 2, length / 2, -length / 2);
      glVertex3f(-length / 2, -length / 2, length / 2);
      glEnd();
    }

    // bottom
    if (b->rot_width == -2 || b->rot_mB.Y() == -1) {
      glBegin(GL_TRIANGLES);
      glVertex3f(length / 2, -length / 2, -length / 2);
      glVertex3f(-length / 2, -length / 2, -length / 2);
      glVertex3f(length / 2, length / 2, -length / 2);
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(-length / 2, -length / 2, -length / 2);
      glVertex3f(-length / 2, length / 2, -length / 2);
      glVertex3f(length / 2, length / 2, -length / 2);
      glEnd();
    }
  }
  if (showBoxBoundary) {
    glColor3f(0, 0, 0);
    glLineWidth(1);
    // front
    glBegin(GL_LINE_LOOP);
    glVertex3f(length / 2, -length / 2, length / 2);
    glVertex3f(length / 2, -length / 2, -length / 2);
    glVertex3f(length / 2, length / 2, -length / 2);
    glVertex3f(length / 2, length / 2, length / 2);
    glEnd();
    // top
    glBegin(GL_LINE_LOOP);
    glVertex3f(-length / 2, -length / 2, length / 2);
    glVertex3f(length / 2, -length / 2, length / 2);
    glVertex3f(length / 2, length / 2, length / 2);
    glVertex3f(-length / 2, length / 2, length / 2);
    glEnd();
    // right
    glBegin(GL_LINE_LOOP);
    glVertex3f(length / 2, length / 2, length / 2);
    glVertex3f(length / 2, length / 2, -length / 2);
    glVertex3f(-length / 2, length / 2, -length / 2);
    glVertex3f(-length / 2, length / 2, length / 2);
    glEnd();
    // left
    glBegin(GL_LINE_LOOP);
    glVertex3f(length / 2, -length / 2, length / 2);
    glVertex3f(length / 2, -length / 2, -length / 2);
    glVertex3f(-length / 2, -length / 2, -length / 2);
    glVertex3f(-length / 2, -length / 2, length / 2);
    glEnd();
    // back
    glBegin(GL_LINE_LOOP);
    glVertex3f(-length / 2, -length / 2, length / 2);
    glVertex3f(-length / 2, -length / 2, -length / 2);
    glVertex3f(-length / 2, length / 2, -length / 2);
    glVertex3f(-length / 2, length / 2, length / 2);
    glEnd();
    // bottom
    glBegin(GL_LINE_LOOP);
    glVertex3f(length / 2, length / 2, -length / 2);
    glVertex3f(length / 2, -length / 2, -length / 2);
    glVertex3f(-length / 2, -length / 2, -length / 2);
    glVertex3f(-length / 2, length / 2, -length / 2);
    glEnd();
  }
}

/*
 * DRAW Quads
 */
void Display::drawOcts(ConfBox3d* b, double epsilon) {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  switch (b->status) {
    case FREE:
      glLineWidth(1);
      glColor4f(0, 1, 0, boxTransparency);
      break;
    case STUCK:
      glLineWidth(1);
      glColor4f(1, 0, 0, boxTransparency);
      break;
    case MIXED:
      glLineWidth(1);
      glColor4f(1, 1, 0, boxTransparency);
      if (b->width < epsilon) {
        glColor4f(0.5, 0.5, 0.5, boxTransparency);
      }
      break;
    case UNKNOWN:
      std::cout << "UNKNOWN in drawQuad" << std::endl;
      break;
    default:
      std::cout << b->status << std::endl;
      std::cerr << "Wrong Box Type" << std::endl;
      exit(1);
  }

  if (b->status == FREE) {
    glPushMatrix();
    glTranslated(b->mB.X(), b->mB.Y(), b->mB.Z());
    drawCube(b, b->width, showBox, showBoxBoundary);
    glPopMatrix();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}

/*
 * DRAW Tree
 */
void Display::drawTree(ConfBox3d* b) {
  if (b) {
    if (b->isLeaf()) {
      drawOcts(b, epsilon);
    } else {
      for (int i = 0; i < b->children.size(); ++i) {
        drawTree(b->children[i]);
      }
    }
  }
}

/*
 * DRAW PATH
 */
void Display::drawPath(vector<ConfBox3d*>& path) {
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glColor3f(0, 1, 1);
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  for (unsigned int i = 0; i < path.size(); ++i) {
    glVertex3f(path[i]->mB.X(), path[i]->mB.Y(), path[i]->mB.Z());
  }
  glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 * DRAW Sphere
 */
void Display::drawSpheres(ConfBox3d* b, int transparency) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (list<Sphere*>::iterator iter = b->spheres.begin();
       iter != b->spheres.end(); ++iter) {
    Sphere* s = *iter;
    filledSphere(s->radius, s->p.X(), s->p.Y(), s->p.Z(), 1.0, 1.0, 1.0,
                 transparency / 100.0);
  }
}
void Display::filledSphere(double radius, double x, double y, double z,
                           double r, double g, double b,
                           double transparency = 1) {
  sphereList = glGenLists(1);
  glNewList(sphereList, GL_COMPILE);
  GLUquadricObj* quadObj = gluNewQuadric();
  gluQuadricDrawStyle(quadObj, GLU_FILL);
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  gluSphere(quadObj, radius, 100, 100);
  glEndList();

  glPushMatrix();
  glColor3f(r, g, b);
  glTranslated(x, y, z);
  glCallList(sphereList);
  glPopMatrix();
}

/*
 * DRAW Rod with width
 */
void Display::rod(double length, double x, double y, double z,
                  float rot_mat[16], double r = 1, double g = 1, double b = 1,
                  double a = 1) {
  glLineWidth(2.0);
  glPushMatrix();
  glTranslatef(x, y, z);
  glMultMatrixf(rot_mat);
  glBegin(GL_LINES);
  glColor4f(r, g, b, a);
  glVertex3f(0, 0, 0);
  glVertex3f(0, length, 0);
  glEnd();
  glPopMatrix();

  filledSphere(3.0, x, y, z, 1, 1, 0);
}
void Display::ring(double radius, double x, double y, double z,
                   float rot_mat[16], double r = 1, double g = 1, double b = 1,
                   double a = 1) {
  glPushMatrix();
  glColor4f(r, g, b, a);
  glTranslatef(x, y, z);
  glMultMatrixf(rot_mat);
  glScalef(radius, radius, radius);
  glCallList(theTorus);
  glPopMatrix();
}

void Display::drawRing() {
  float rot_mat[16], rot[3];
  rot[0] = Br.X();
  rot[1] = Br.Y();
  rot[2] = Br.Z();
  Rot3dSide::rot2mat(rot[0], rot[1], rot[2], rot_mat);
  rod(R, Bt.X(), Bt.Y(), Bt.Z(), rot_mat, 0.2, 1, 0.5, 1);
  ring(R, Bt.X(), Bt.Y(), Bt.Z(), rot_mat, 0.2, 1, 0.5, 1);
  ring(R - Btw / 2 * sqrt(2), Bt.X(), Bt.Y(), Bt.Z(), rot_mat, 0.2, 1, 0.2, 1);
  ring(R + Btw / 2 * sqrt(2), Bt.X(), Bt.Y(), Bt.Z(), rot_mat, 0.2, 1, 0.7, 1);

  // 0)
  Vector circle_n(Rot3dSide::cube2sphere(Br) - Point3d(0, 0, 0));
  circle_n.normalize();

  // 1)
  double sin = ((Brw / 2) * sqrt(2)) / 3;
  double cos = sqrt(1 - (Brw / 2) * (Brw / 2) * 2 / 9);
  double rcos = R * cos;
  double rsin = R * sin;
  Circle3d topCircle(Point3d(Bt + circle_n * rsin), rcos, circle_n);
  Circle3d botCircle(Point3d(Bt - circle_n * rsin), rcos, circle_n);

  ring(rcos, topCircle.P().X(), topCircle.P().Y(), topCircle.P().Z(), rot_mat,
       0.5, 0.2, 1, 1);
  ring(rcos, botCircle.P().X(), botCircle.P().Y(), botCircle.P().Z(), rot_mat,
       1, 0.5, 0.2, 1);
}

void Display::drawWall() {
  Vector n(W.normal());
  n.normalize();
  glColor3f(0.2, 0.2, 1.0);
  glBegin(GL_TRIANGLES);
  glNormal3d(n.X(), n.Y(), n.Z());
  glVertex3f(W.V1().X(), W.V1().Y(), W.V1().Z());
  glVertex3f(W.V2().X(), W.V2().Y(), W.V2().Z());
  glVertex3f(W.V3().X(), W.V3().Y(), W.V3().Z());
  glEnd();
}

void Display::drawEdge() {
  glColor3f(1, 0.2, 0.2);
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(E.startPt().X(), E.startPt().Y(), E.startPt().Z());
  glVertex3f(E.stopPt().X(), E.stopPt().Y(), E.stopPt().Z());
  glEnd();
}

void Display::drawCorner() {
  glColor3f(0.2, 0.2, 0.2);
  glPointSize(3.0);
  glBegin(GL_POINT);
  glVertex3f(E.startPt().X(), E.startPt().Y(), E.startPt().Z());
  glEnd();
}
