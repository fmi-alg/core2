#ifndef __disc__Display__
#define __disc__Display__

// Custom
#include "MainWindow.h"
#include "subdivision/Box.h"
#include "subdivision/QuadTree.h"
#include "CORE/geom2d/point2d.h"
#include "CORE/geom2d/segment2d.h"
#include "CORE/geom2d/line2d.h"
#include "CORE/geom2d/circle2d.h"
#include "CORE/geom2d/triangle2d.h"
#include "CORE/geom2d/polygon2d.h"
#include "display/Color.h"


// Qt
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
    // Essential Functions Inherited from QOpenGLWidget
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderScene();

    /*********************************
     * Functions used to paint scene *
     *********************************/
    // Paint Shapes to Screen
    void putBoxes();

    void drawBoxes(Box* b, double epsilon);
    void drawObstacles();
    void drawPath(Point2d start, Point2d goal);
    void drawRobot(Box* b, Color c);
    void drawRobot(Point2d p, Color c);
    void drawTriangle(Triangle2d t, Color clr, bool draw_solid, bool draw_outline);
    void drawBoxesOutline(Point2d a, Point2d b, Point2d c, Point2d d);

    void drawCube(double length); // could be used to show the hidden boxes
};

#endif /* defined(__disc__Display__) */
