/*
 *  Display.h
 *  
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
 */

#ifndef __disc__Display__
#define __disc__Display__

// Custom
#include "MainWindow.h"
#include "Box.h"
#include "QuadTree.h"
#include "Polygon.h"
#include "Vertex.h"
#include "Triangle.h"
#include "triangulate.h"

// Qt/OpenGL
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
    Display(QWidget* parent = 0);
    virtual ~Display();


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);


private:
    void renderScene();

    /*********************************
     * Functions used to paint scene *
     *********************************/
    void putBoxes();

    void drawBoxes(Box* b, double epsilon);
    void drawBoxesBoundary(Pose a, Pose b, Pose c, Pose d);
    void drawPath();
    void drawRobot(Box* b, double gradient);
    void drawRobot(Pose p, double gradient);
    void drawLink(Pose a, Pose b, Color clr);
    void drawCircle(Pose a, Color clr);
    void drawQuadrilateral(Pose a, Pose b, Pose c, Pose d, Color clr);
    void drawQuadrilateralOutline(Pose a, Pose b, Pose c, Pose d, Color clr);
    void drawTriangle(Pose a, Pose b, Pose c, Color clr);

    void drawObstacles();
};

#endif /* defined(__disc__Display__) */
