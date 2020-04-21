#ifndef __disc__Display__
#define __disc__Display__

// Custom
#include "MainWindow.h"
#include "Box.h"
#include "QuadTree.h"
#include "Vertex.h"
#include "triangulate.h"
#include "Triangle.h"


// Qt
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QtOpenGL>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


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
    void drawPath(Pose start, Pose goal);
    void drawRobot(Box* b, Color c);
    void drawRobot(Pose p, Color c);
    void drawTriangle(Pose a, Pose b, Pose c, Color clr, bool draw_solid, bool draw_outline);
    void drawBoxesOutline(Pose a, Pose b, Pose c, Pose d);

    void drawCube(double length); // could be used to show the hidden boxes
};

#endif /* defined(__disc__Display__) */
