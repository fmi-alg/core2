/*
 * display.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 *  Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef DISPLAY_H
#define DISPLAY_H

// Standard Library
#include <vector>

// Qt & OpenGL
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include "glu.h"

// Custom
#include "main_window.h"
#include "sss/soft_subdivision_search.h"
#include "subdivision/configuration_box3d.h"

class Display : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit Display(QWidget* parent = 0);
  ~Display();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);
  void keyPressEvent(QKeyEvent* event);

 public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);

 signals:
  void zDistanceChanged(int val);

 private:
  void renderScene();

  /*********************************
   * Functions used to paint scene *
   *********************************/
  // Paint Shapes to Screen
  void drawAxisBoundingBox(double length);
  void drawObstacles(ConfBox3d* box, int transparency);
  void drawCube(ConfBox3d* box, double length, bool show_box,
                bool show_box_boundary);
  void drawOcts(ConfBox3d* box, double epsilon);
  void drawTree(ConfBox3d* box);
  void drawPath(std::vector<ConfBox3d*>& path);
  // void drawSpheres(ConfBox3d* box, int transparency);
  void filledSphere(double radius, double x, double y, double z, double r,
                    double g, double b, double transparency);

  void rod(double length, double x, double y, double z, float rot_mat[16],
           double r, double g, double b, double a);

  void drawPoly();
  void drawDebugWall();
  void drawDebugEdge();
  void drawCorner();

  int x_rot, y_rot, z_rot;
  double distance_x, distance_y, distance_z;

  QPoint lastPos;
};

#endif  // DISPLAY_H
