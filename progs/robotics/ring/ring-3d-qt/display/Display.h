//==============================================================================
// Display.h
// =================
//
// AUTHOR: Bryant Curto     (bryantcurto@gmail.com)
//         Ching-Hsiang Hsu (chh415@nyu.edu)
// CREATED: 2015-07-28
// UPDATE : 2015-10-20
//
// Copyright (c) 2015 Bryant Curto, Ching-Hsiang Hsu
//==============================================================================

#ifndef __disc__Display__
#define __disc__Display__

// Custom
#include "MainWindow.h"
#include "subdivision/ConfBox3d.h"
#include "sss/SoftSubdivisionSearch.h"

// Qt & OpenGL
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include "glu.h"

// Standard Library
#include <vector>


class Display : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget* parent = 0);
    ~Display();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

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
    void drawObstacles(ConfBox3d* b, int transparency);
    void drawCube(ConfBox3d* b, double length, bool showBox, bool showBoxBoundary);
    void drawOcts(ConfBox3d* b, double epsilon);
    void drawTree(ConfBox3d* b);
    void drawPath(vector<ConfBox3d*>& path);
    void drawSpheres(ConfBox3d* b, int transparency);
    void filledSphere(double radius, double x, double y, double z, double r, double g, double b, double transparency);

    void rod(double length, double x, double y, double z, float rot_mat[16], double r, double g, double b, double a);
    void ring(double radius, double x, double y, double z, float rot_mat[16], double r, double g, double b, double a);


    void drawRing();
    void drawWall();
    void drawEdge();
    void drawCorner();

    int xRot, yRot, zRot;
    double distanceX, distanceY;

    QPoint lastPos;
};

#endif /* defined(__disc__Display__) */
