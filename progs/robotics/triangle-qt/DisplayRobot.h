#ifndef DISPLAYROBOT
#define DISPLAYROBOT

// Custom
#include "MainWindow.h"
#include "Box.h"
#include "QuadTree.h"
#include "Vertex.h"


// Qt
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include "glu.h"

// Standard Library
#include <vector>


class DisplayRobot : public QOpenGLWidget
{
    Q_OBJECT
public:
    DisplayRobot(QWidget* parent = 0);
    virtual ~DisplayRobot();


protected:
    // Essential Functions Inherited from QOpenGLWidget
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderScene();

    void drawRobot(Pose p, Color c);
    void drawLine(Pose p);
    void drawTriangle(Pose a, Pose b, Pose c, Color clr, bool draw_solid, bool draw_outline);
    void drawCircle();
};

#endif // DISPLAYROBOT

