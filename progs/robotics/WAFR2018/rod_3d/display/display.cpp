/*
 * display.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "display/display.h"

#include <math.h>
#include <unistd.h>
#include <ctime>
#include <iostream>

extern FILE* debug_ptr;
extern bool verbose;

// Global Variables
extern std::string working_dir;
extern std::string filename;
extern Point3d start;
extern Point3d goal;
extern Point3d start_rotation;
extern Point3d goal_rotation;
extern double epsilon;
extern double rod_length;
extern int search_type;
extern int seed;
extern bool no_path;
extern bool show_box;
extern bool show_box_boundary;
extern int animation_speed;
extern double box_width;

extern SoftSubdivisionSearch* sss;
extern int transparency;
extern double box_transparency;
extern double distance_z;
extern Point3d eye;
extern Point3d at;
extern Vector up;
extern Vector eye_vector;
extern float view_rotate_angles[3];
extern Point3d obj_pos;
extern float view_rotate[16];
extern float view_rotate_x[16];
extern float view_rotate_y[16];
extern float view_rotate_z[16];
extern std::vector<ConfBox3d*> path;
extern bool show_path;
extern bool show_trace;
extern bool generate_trace;
extern bool show_animation;
extern bool pause_animation;
extern bool replay_animation;
extern int incr;
extern unsigned int idx_path;
extern float idx_seg;
extern float seg_count;

// debug information
extern Point3d box_translation;
extern double box_translation_width;
extern Point3d box_rotation;
extern double box_rotation_width;
extern bool show_debug_poly;
extern Triangle3d debug_wall;
extern bool show_debug_wall;
extern Segment3d debug_edge;
extern bool show_debug_edge;
extern bool show_debug_corner;
extern int trace_dense;

// local Variables
std::vector<float> trace;
static GLuint sphere_list;
int render_steps = 1;
bool step = false;
Point3d preRot = start_rotation;

/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent) : QOpenGLWidget(parent) {
  x_rot = 0;
  y_rot = -45 * 16;
  z_rot = 0;

  distance_x = 0.0;
  distance_y = 0.0;
  distance_z = 900.0;

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
    setXRotation(x_rot + 8 * dy);
    setYRotation(y_rot + 8 * dx);

    update();
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(x_rot + 8 * dy);
    setZRotation(z_rot + 8 * dx);

    update();
  }

  lastPos = event->pos();
  event->accept();
}

void Display::wheelEvent(QWheelEvent* event) {
  int delta = event->delta();

  if (event->orientation() == Qt::Vertical) {
    if (delta < 0) {
      distance_z *= 1.01;
    } else if (delta > 0) {
      distance_z *= 0.99;
    }

    update();
  }

  emit zDistanceChanged(distance_z);

  event->accept();
}

void Display::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Up) {
    distance_y += 20.0f;
    update();
  } else if (event->key() == Qt::Key_Down) {
    distance_y -= 20.0f;
    update();
  } else if (event->key() == Qt::Key_Left) {
    distance_x -= 20.0f;
    update();
  } else if (event->key() == Qt::Key_Right) {
    distance_x += 20.0f;
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
  if (angle != x_rot) {
    x_rot = angle;
    // emit xRotationChanged(angle);
    update();
  }
}

void Display::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != y_rot) {
    y_rot = angle;
    // emit yRotationChanged(angle);
    update();
  }
}

void Display::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != z_rot) {
    z_rot = angle;
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

void Display::initializeGL() {
  glClearColor(0.8, 0.8, 0.8, 1.0);
  // glEnable(GL_CULL_FACE);
  // glShadeModel(GL_FLAT);

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
  // glDepthFunc(GL_EQUAL);

  eye_vector.set(eye.X() - at.X(), eye.Y() - at.Y(), eye.Z() - at.Z());
  resetRotationMatrix();
  obj_pos = eye;
}

void Display::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2]+distance, obj_pos[0] +
  // eye_vector[0], obj_pos[1] + eye_vector[1], obj_pos[2] + eye_vector[2],
  // up[0], up[1], up[2]);
  gluLookAt(0, 0, distance_z, 0.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0f);
  glTranslatef(distance_x, distance_y, 0);
  glRotatef(x_rot / 16.0, 1.0, 0.0, 0.0);
  glRotatef(y_rot / 16.0, 0.0, 1.0, 0.0);
  glRotatef(z_rot / 16.0, 0.0, 0.0, 1.0);

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
  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eye_vector[0],
  // obj_pos[1] + eye_vector[1], obj_pos[2] + eye_vector[2], up[0], up[1],
  // up[2]); glMultMatrixf(view_rotate);

  // double scale = 2.0f/box_width;
  // glScalef(scale, scale, scale);
  // gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eye_vector[0],
  // obj_pos[1] + eye_vector[1], obj_pos[2] + eye_vector[2], up[0], up[1],
  // up[2]); glMultMatrixf(view_rotate);
}

void Display::renderScene() {
  // Draw Obstacles
  drawObstacles(sss->root, transparency);

  drawAxisBoundingBox(box_width * 2);
  if (show_box || show_box_boundary) {
    drawTree(sss->root);
  }
  if (show_debug_poly) {
    glPushMatrix();
    drawPoly();
    glPopMatrix();
  }
  if (show_debug_wall) {
    glPushMatrix();
    drawDebugWall();
    glPopMatrix();
  }
  if (show_debug_edge) {
    glPushMatrix();
    drawDebugEdge();
    glPopMatrix();
  }
  if (show_debug_corner) {
    glPushMatrix();
    drawCorner();
    glPopMatrix();
  }

  // glPolygonMode(GL_FRONT, GL_LINE);

  float rot_mat[16];
  if (!no_path) {
    if (show_path) drawPath(path /*, start, goal*/);
    if (show_trace) {
      if (!generate_trace) {
        preRot = start_rotation;

        generate_trace = true;
        trace.clear();
        for (unsigned i = 0; i < path.size() - 1;) {
          // translational part
          float dx = path[i + 1]->center.X() - path[i]->center.X();
          float dy = path[i + 1]->center.Y() - path[i]->center.Y();
          float dz = path[i + 1]->center.Z() - path[i]->center.Z();
          float segLength = sqrt(dx * dx + dy * dy + dz * dz);
          if (segLength > 0)
            seg_count = segLength / epsilon;
          else
            seg_count = 8;
          float x = path[i]->center.X() + dx / seg_count * idx_seg;
          float y = path[i]->center.Y() + dy / seg_count * idx_seg;
          float z = path[i]->center.Z() + dz / seg_count * idx_seg;
          // rotational part
          bool recordCur = true;
          float cur_rot[3], next_rot[3];
          if (!path[i]->getRot(cur_rot)) {
            recordCur = false;
            cur_rot[0] = preRot.X();
            cur_rot[1] = preRot.Y();
            cur_rot[2] = preRot.Z();
          }
          if (!path[i + 1]->getRot(next_rot)) {
            for (int ii = 0; ii < 3; ++ii) next_rot[ii] = cur_rot[ii];
          }
          float rot[3];

          //          fprintf(debug_ptr, "center (%f, %f, %f) %f\n",
          //          path[i]->center.X(),
          //                  path[i]->center.Y(), path[i]->center.Z(),
          //                  path[i]->width);
          //          if (path[i]->rotation_width != -2) {
          //            fprintf(debug_ptr, "rot (%f, %f, %f) %f\n",
          //                    path[i]->rotation_center.X(),
          //                    path[i]->rotation_center.Y(),
          //                    path[i]->rotation_center.Z(),
          //                    path[i]->rotation_width);
          //          }

          Rot3d::interpolateRot(cur_rot, next_rot, idx_seg, seg_count, rot);
          Rot3d::rot2mat(rot[0], rot[1], rot[2], rot_mat);
          rot_mat[3] = x;
          rot_mat[7] = y;
          rot_mat[11] = z;
          for (int j = 0; j < 16; ++j) {
            trace.push_back(rot_mat[j]);
          }
          ++idx_seg;
          if (idx_seg > seg_count) {
            ++i;
            idx_seg = 1;
          }

          if (recordCur) {
            preRot.set(cur_rot[0], cur_rot[1], cur_rot[2]);
          }
        }
      } else {
        for (unsigned i = 0; i < trace.size(); i += trace_dense) {
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
          if (i > 0)
            rod(rod_length, x, y, z, rot_mat, 0.4 + 0.4 * (1.0f - rgb), 0,
                0.4 + 0.4 * (1.0f - rgb), 1.0f);
        }
        update();
      }
    }

    if (replay_animation) {
      replay_animation = false;
      show_animation = true;
      pause_animation = false;
      incr = 0;
    }

    if (show_animation) {
      if (idx_path >= path.size() - 1) {
        show_animation = false;
      } else {
        float dx = path[idx_path + 1]->center.X() - path[idx_path]->center.X();
        float dy = path[idx_path + 1]->center.Y() - path[idx_path]->center.Y();
        float dz = path[idx_path + 1]->center.Z() - path[idx_path]->center.Z();
        float segLength = sqrt(dx * dx + dy * dy + dz * dz);
        if (segLength > 0)
          seg_count = segLength / epsilon;
        else
          seg_count = 8;
        float x = path[idx_path]->center.X() + dx / seg_count * idx_seg;
        float y = path[idx_path]->center.Y() + dy / seg_count * idx_seg;
        float z = path[idx_path]->center.Z() + dz / seg_count * idx_seg;

        bool recordCur = true;
        float cur_rot[3], next_rot[3];
        if (!path[idx_path]->getRot(cur_rot)) {
          recordCur = false;
          cur_rot[0] = preRot.X();
          cur_rot[1] = preRot.Y();
          cur_rot[2] = preRot.Z();
        }
        if (!path[idx_path + 1]->getRot(next_rot)) {
          for (int i = 0; i < 3; ++i) next_rot[i] = cur_rot[i];
        }
        float rot[3];
        Rot3d::interpolateRot(cur_rot, next_rot, idx_seg, seg_count, rot);
        Rot3d::rot2mat(rot[0], rot[1], rot[2], rot_mat);

        double rgb = path.size();
        rgb = (double)idx_path / rgb;
        rod(rod_length, x, y, z, rot_mat, 0.4 + 0.4 * (1.0f - rgb), 0,
            0.4 + 0.4 * (1.0f - rgb), 1.0f);

        if (!pause_animation) {
          //                    fprintf(g_fptr, "box %f %f %f w %f\n",
          //                    path[idx_path]->x, path[idx_path]->y,
          //                    path[idx_path]->z, path[idx_path]->width);
          //                    if(path[idx_path]->rotation_width != -2)
          //                      fprintf(g_fptr, "rot %f %f %f w %f\n",
          //                      path[idx_path]->rot_x, path[idx_path]->rot_y,
          //                      path[idx_path]->rot_z,
          //                      path[idx_path]->rotation_width);
          //                    fprintf(g_fptr, "status %d\n",
          //                    path[idx_path]->status);

          usleep((99 - animation_speed) * 500);
          idx_seg++;
          if (idx_seg > seg_count) {
            idx_path++;
            idx_seg = 1;
          }
        }

        if (recordCur) {
          preRot.set(cur_rot[0], cur_rot[1], cur_rot[2]);
        }
      }
      update();
    } else {
      preRot = start_rotation;
    }
  }
  Rot3d::rot2mat(start_rotation.X(), start_rotation.Y(), start_rotation.Z(),
                 rot_mat);
  rod(rod_length, start.X(), start.Y(), start.Z(), rot_mat, 0.4, 0, 0.4,
      1);  // start
  Rot3d::rot2mat(goal_rotation.X(), goal_rotation.Y(), goal_rotation.Z(),
                 rot_mat);
  rod(rod_length, goal.X(), goal.Y(), goal.Z(), rot_mat, 0.8, 0, 0.8,
      1);  // goal
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

  glPushMatrix();
  glTranslated(256, 256, 256);
  drawCube(sss->root, 512, false, true);
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 * DRAW Edge
 */
void Display::drawObstacles(ConfBox3d* box, int transparency) {
  // glEnable(GL_BLEND);
  // glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  for (const auto& wall : box->walls) {
    if (strcmp(wall->name().c_str(), "Bound") == 0) continue;
    Vector n(wall->triangle().normal());
    n.normalize();
    glColor4f(0.5, 0.5, 0.5, transparency / 100.0);

    glBegin(GL_TRIANGLES);
    glNormal3d(n.X(), n.Y(), n.Z());
    glVertex3f(wall->a->point().X(), wall->a->point().Y(),
               wall->a->point().Z());
    glVertex3f(wall->b->point().X(), wall->b->point().Y(),
               wall->b->point().Z());
    glVertex3f(wall->c->point().X(), wall->c->point().Y(),
               wall->c->point().Z());
    glEnd();
  }
  glLineWidth(1.0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_DEPTH_TEST);
}

/*
 * DRAW Cube
 */
void Display::drawCube(ConfBox3d* box, double length, bool show_box,
                       bool sho_box_boundary) {
  if (show_box) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // front
    if (box->rotation_width == -2 || box->rotation_center.Z() == 1) {
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
    if (box->rotation_width == -2 || box->rotation_center.Y() == 1) {
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
    if (box->rotation_width == -2 || box->rotation_center.X() == 1) {
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
    if (box->rotation_width == -2 || box->rotation_center.X() == -1) {
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
    if (box->rotation_width == -2 || box->rotation_center.Z() == -1) {
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
    if (box->rotation_width == -2 || box->rotation_center.Y() == -1) {
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
  if (sho_box_boundary) {
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
void Display::drawOcts(ConfBox3d* box, double epsilon) {
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  switch (box->status) {
    case FREE:
      glLineWidth(1);
      glColor4f(0, 1, 0, box_transparency);
      break;
    case STUCK:
      glLineWidth(1);
      glColor4f(1, 0, 0, box_transparency);
      break;
    case MIXED:
      glLineWidth(1);
      glColor4f(1, 1, 0, box_transparency);
      if (box->width < epsilon) {
        glColor4f(0.5, 0.5, 0.5, box_transparency);
      }
      break;
    case UNKNOWN:
      std::cout << "UNKNOWN in drawQuad" << std::endl;
      break;
    default:
      std::cout << box->status << std::endl;
      std::cerr << "Wrong Box Type" << std::endl;
      exit(1);
  }

  if (box->status == FREE) {
    glPushMatrix();
    glTranslated(box->center.X(), box->center.Y(), box->center.Z());
    drawCube(box, box->width, show_box, show_box_boundary);
    glPopMatrix();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}

/*
 * DRAW Tree
 */
void Display::drawTree(ConfBox3d* box) {
  if (box) {
    if (box->isLeaf()) {
      drawOcts(box, epsilon);
    } else {
      for (auto&& child : box->children) {
        drawTree(child);
      }
    }
  }
}

/*
 * DRAW PATH
 */
void Display::drawPath(std::vector<ConfBox3d*>& path) {
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glColor3f(0, 1, 1);
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  for (unsigned int i = 0; i < path.size(); ++i) {
    glVertex3f(path[i]->center.X(), path[i]->center.Y(), path[i]->center.Z());
  }
  glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/*
 * DRAW Sphere
 */
// void Display::drawSpheres(ConfBox3d* b, int transparency) {
//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  for (list<Sphere*>::iterator iter = b->spheres.begin();
//       iter != b->spheres.end(); ++iter) {
//    Sphere* s = *iter;
//    filledSphere(s->radius, s->p.X(), s->p.Y(), s->p.Z(), 1.0, 1.0, 1.0,
//                 transparency / 100.0);
//  }
//}
void Display::filledSphere(double radius, double x, double y, double z,
                           double r, double g, double b,
                           double transparency = 1) {
  sphere_list = glGenLists(1);
  glNewList(sphere_list, GL_COMPILE);
  GLUquadricObj* quadObj = gluNewQuadric();
  gluQuadricDrawStyle(quadObj, GLU_FILL);
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  gluSphere(quadObj, radius, 100, 100);
  glEndList();

  glPushMatrix();
  glColor3f(r, g, b);
  glTranslated(x, y, z);
  glCallList(sphere_list);
  glPopMatrix();
}

/*
 * DRAW Rod with width
 */
void Display::rod(double length, double x, double y, double z,
                  float rot_mat[16], double r = 1, double g = 1, double b = 1,
                  double a = 1) {
  // rot_mat[3] = x; rot_mat[7] = y; rot_mat[11] = z;

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

  // filledSphere(2.0, x, y, z, 1, 1, 0);
}

void Display::drawPoly() {
  Point3d rotation_point[5], new_rotation_point[5];
  double du[4] = {-1, 1, 1, -1};
  double dv[4] = {-1, -1, 1, 1};
  for (int i = 0; i < 4; ++i) {
    if (1 - fabs(box_rotation.X()) < EPS) {
      rotation_point[i].set(box_rotation.X(),
                            box_rotation.Y() + du[i] * box_rotation_width / 2,
                            box_rotation.Z() + dv[i] * box_rotation_width / 2);
    } else if (1 - fabs(box_rotation.Y()) < EPS) {
      rotation_point[i].set(box_rotation.X() + du[i] * box_rotation_width / 2,
                            box_rotation.Y(),
                            box_rotation.Z() + dv[i] * box_rotation_width / 2);
    } else {
      rotation_point[i].set(box_rotation.X() + du[i] * box_rotation_width / 2,
                            box_rotation.Y() + dv[i] * box_rotation_width / 2,
                            box_rotation.Z());
    }
    rotation_point[i] = Rot3d::cube2sphere(rotation_point[i]);
  }
  rotation_point[4] = rotation_point[0];

  Plane3d* H[6];
  for (int i = 0; i < 4; ++i) {
    if (box_rotation.X() == 1 || box_rotation.Y() == -1 ||
        box_rotation.Z() == 1) {
      Vector vi(rotation_point[i] - Point3d(0, 0, 0));
      vi.normalize();
      new_rotation_point[i] = Point3d(
          vi.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      Vector vi1(rotation_point[i + 1] - Point3d(0, 0, 0));
      vi1.normalize();
      new_rotation_point[i + 1] = Point3d(
          vi1.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      // center at abstract box_translation
      Triangle3d t(new_rotation_point[i], Point3d(0, 0, 0),
                   new_rotation_point[i + 1]);
      t.normalize();
      // transfer to physical space
      Triangle3d T(
          box_translation + rotation_point[i] + t.normal() * box_rotation_width,
          box_translation + t.normal() * box_rotation_width,
          box_translation + rotation_point[i + 1] +
              t.normal() * box_rotation_width);
      H[i] = new Plane3d(T.toPlane());
    }
    if (box_rotation.X() == -1 || box_rotation.Y() == 1 ||
        box_rotation.Z() == -1) {
      Vector vi(rotation_point[i] - Point3d(0, 0, 0));
      vi.normalize();
      new_rotation_point[i] = Point3d(
          vi.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      Vector vi1(rotation_point[i + 1] - Point3d(0, 0, 0));
      vi1.normalize();
      new_rotation_point[i + 1] = Point3d(
          vi1.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      // center at abstract box_translation
      Triangle3d t(Point3d(0, 0, 0), new_rotation_point[i],
                   new_rotation_point[i + 1]);
      t.normalize();
      // transfer to physical space
      Triangle3d T(
          box_translation + t.normal() * box_rotation_width,
          box_translation + rotation_point[i] + t.normal() * box_rotation_width,
          box_translation + rotation_point[i + 1] +
              t.normal() * box_rotation_width);
      H[i] = new Plane3d(T.toPlane());
    }
  }
  H[4] = new Plane3d(*H[0]);

  // 2)
  Point3d box_rotation_on_sphere = Rot3d::cube2sphere(box_rotation);
  Vector n(box_rotation_on_sphere.X(), box_rotation_on_sphere.Y(),
           box_rotation_on_sphere.Z());
  n.normalize();
  Point3d bottom_H(box_translation.X() - n.X() * box_rotation_width,
                   box_translation.Y() - n.Y() * box_rotation_width,
                   box_translation.Z() - n.Z() * box_rotation_width);
  H[5] = new Plane3d(bottom_H, -n);
  debug_edge.setStartPt(bottom_H);
  Point3d tmp(debug_edge.startPt().X() - n.X() * 50,
              debug_edge.startPt().Y() - n.Y() * 50,
              debug_edge.startPt().Z() - n.Z() * 50);
  debug_edge.setStopPt(tmp);
  drawDebugEdge();

  //  Line3d l[4];
  //  Point3d base[5];
  //  Point3d apex[5];

  //  for (int i = 0; i < 4; ++i) {
  //    l[i] = *(Line3d*)H[i]->intersection(*H[(i + 1) % 4]);
  //  }
  //  for (int i = 0; i < 4; ++i) {
  //    base[i] = *(Point3d*)H[5]->intersection(l[i]);
  //  }
  //  for (int i = 0; i < 4; ++i) {
  //    Vector vl(l[i].direction());
  //    vl.normalize();
  //    if (vl * H[5]->normal() < 0) {
  //      apex[i] =
  //          base[i] + vl * (box_translation_width * sqrt(3) / 2.0 +
  //          rod_length);
  //    } else {
  //      apex[i] =
  //          base[i] - vl * (box_translation_width * sqrt(3) / 2.0 +
  //          rod_length);
  //    }
  //  }
  //  base[4] = base[0];
  //  apex[4] = apex[0];

  //  for (int i = 0; i < 4; ++i) {
  //    if (i == 0) glColor4f(1, 0, 0, 0.8);
  //    if (i == 1) glColor4f(0, 1, 0, 0.8);
  //    if (i == 2) glColor4f(0, 0, 1, 0.8);
  //    if (i == 3) glColor4f(1, 1, 0, 0.8);

  //    glBegin(GL_QUADS);
  //    glVertex3f(base[i].X(), base[i].Y(), base[i].Z());
  //    glVertex3f(base[i + 1].X(), base[i + 1].Y(), base[i + 1].Z());
  //    glVertex3f(apex[i + 1].X(), apex[i + 1].Y(), apex[i + 1].Z());
  //    glVertex3f(apex[i].X(), apex[i].Y(), apex[i].Z());
  //    glEnd();
  //  }

  for (int i = 0; i < 6; ++i) {
    delete H[i];
  }

  Point3d center[4];
  // four extended triangles
  for (int i = 0; i < 4; ++i) {
    if (box_rotation.X() == 1 || box_rotation.Y() == -1 ||
        box_rotation.Z() == 1) {
      Vector vi(rotation_point[i] - Point3d(0, 0, 0));
      vi.normalize();
      new_rotation_point[i] = Point3d(
          vi.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      Vector vi1(rotation_point[i + 1] - Point3d(0, 0, 0));
      vi1.normalize();
      new_rotation_point[i + 1] = Point3d(
          vi1.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      // center at abstract box_translation
      Triangle3d t(rotation_point[i], Point3d(0, 0, 0), rotation_point[i + 1]);
      t.normalize();
      // transfer to physical space
      Triangle3d T(box_translation + new_rotation_point[i] +
                       t.normal() * box_translation_width,
                   box_translation + t.normal() * box_translation_width,
                   box_translation + new_rotation_point[i + 1] +
                       t.normal() * box_translation_width);
      T.normalize();
      center[i] = center[i] + T.V1();
      center[i] = center[i] + T.V2();
      center[i] = center[i] + T.V3();
      center[i].setX(center[i].X() / 3.0);
      center[i].setY(center[i].Y() / 3.0);
      center[i].setZ(center[i].Z() / 3.0);
      if (verbose)
        fprintf(debug_ptr, "center %f %f %f normal %f %f %f\n", center[i].X(),
                center[i].Y(), center[i].Z(), T.normal().X(), T.normal().Y(),
                T.normal().Z());

      debug_edge.setStartPt(center[i]);
      Point3d tmp(center[i].X() + T.normal().X() * 50,
                  center[i].Y() + T.normal().Y() * 50,
                  center[i].Z() + T.normal().Z() * 50);
      debug_edge.setStopPt(tmp);
      drawDebugEdge();

      if (i == 0) glColor4f(1, 0, 0, 0.8);
      if (i == 1) glColor4f(0, 1, 0, 0.8);
      if (i == 2) glColor4f(0, 0, 1, 0.8);
      if (i == 3) glColor4f(1, 1, 0, 0.8);

      glBegin(GL_TRIANGLES);
      glVertex3f(T.V1().X(), T.V1().Y(), T.V1().Z());
      glVertex3f(T.V2().X(), T.V2().Y(), T.V2().Z());
      glVertex3f(T.V3().X(), T.V3().Y(), T.V3().Z());
      glEnd();
    }
    if (box_rotation.X() == -1 || box_rotation.Y() == 1 ||
        box_rotation.Z() == -1) {
      Vector vi(rotation_point[i] - Point3d(0, 0, 0));
      vi.normalize();
      new_rotation_point[i] = Point3d(
          vi.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      Vector vi1(rotation_point[i + 1] - Point3d(0, 0, 0));
      vi1.normalize();
      new_rotation_point[i + 1] = Point3d(
          vi1.X() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Y() * (box_translation_width * sqrt(3) / 2.0 + rod_length),
          vi1.Z() * (box_translation_width * sqrt(3) / 2.0 + rod_length));

      // center at abstract box_translation
      Triangle3d t(Point3d(0, 0, 0), rotation_point[i], rotation_point[i + 1]);
      t.normalize();
      // transfer to physical space
      Triangle3d T(box_translation + t.normal() * box_translation_width,
                   box_translation + new_rotation_point[i] +
                       t.normal() * box_translation_width,
                   box_translation + new_rotation_point[i + 1] +
                       t.normal() * box_translation_width);
      T.normalize();
      center[i] = center[i] + T.V1();
      center[i] = center[i] + T.V2();
      center[i] = center[i] + T.V3();
      center[i].setX(center[i].X() / 3.0);
      center[i].setY(center[i].Y() / 3.0);
      center[i].setZ(center[i].Z() / 3.0);
      if (verbose)
        fprintf(debug_ptr, "center %f %f %f normal %f %f %f\n", center[i].X(),
                center[i].Y(), center[i].Z(), T.normal().X(), T.normal().Y(),
                T.normal().Z());

      debug_edge.setStartPt(center[i]);
      Point3d tmp(center[i].X() + T.normal().X() * 50,
                  center[i].Y() + T.normal().Y() * 50,
                  center[i].Z() + T.normal().Z() * 50);
      debug_edge.setStopPt(tmp);
      drawDebugEdge();

      if (i == 0) glColor4f(1, 0, 0, 0.8);
      if (i == 1) glColor4f(0, 1, 0, 0.8);
      if (i == 2) glColor4f(0, 0, 1, 0.8);
      if (i == 3) glColor4f(1, 1, 0, 0.8);

      glBegin(GL_TRIANGLES);
      glVertex3f(T.V1().X(), T.V1().Y(), T.V1().Z());
      glVertex3f(T.V2().X(), T.V2().Y(), T.V2().Z());
      glVertex3f(T.V3().X(), T.V3().Y(), T.V3().Z());
      glEnd();
    }
  }
}

void Display::drawDebugWall() {
  Vector n(debug_wall.normal());
  n.normalize();
  glColor3f(0.2, 0.2, 1.0);
  glBegin(GL_TRIANGLES);
  glNormal3d(n.X(), n.Y(), n.Z());
  glVertex3f(debug_wall.V1().X(), debug_wall.V1().Y(), debug_wall.V1().Z());
  glVertex3f(debug_wall.V2().X(), debug_wall.V2().Y(), debug_wall.V2().Z());
  glVertex3f(debug_wall.V3().X(), debug_wall.V3().Y(), debug_wall.V3().Z());
  glEnd();
}

void Display::drawDebugEdge() {
  glColor3f(1, 0.2, 0.2);
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(debug_edge.startPt().X(), debug_edge.startPt().Y(),
             debug_edge.startPt().Z());
  glVertex3f(debug_edge.stopPt().X(), debug_edge.stopPt().Y(),
             debug_edge.stopPt().Z());
  glEnd();
}

void Display::drawCorner() {
  glColor3f(0.2, 0.2, 0.2);
  glPointSize(3.0);
  glBegin(GL_POINT);
  glVertex3f(debug_edge.startPt().X(), debug_edge.startPt().Y(),
             debug_edge.startPt().Z());
  glEnd();
}
