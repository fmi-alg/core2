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
#include "./mathtool/Vector.h"
#include "RRT.h"
#include "m_Triangulate.h"

// Qt
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include "glu.h"

// Standard Library
#include <stdio.h>
#include <iostream>
#include <string>

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
    void drawPath(Planner* planner, const PATH& path);
    void drawRobot(RRT_ROBOT& robot, const CFG& cfg);
    void drawLink(Vector2d a, Vector2d b, Vector3d clr);
    void drawTriangle(Vector2d a, Vector2d b, Vector2d c, Vector3d clr);
    void drawCircle(Vector2d a, Vector3d clr);
    void drawPolygons(RRT_OBJS& objs);
    void drawTree(RRT* planner, const RRT_TREE& tree);
    void drawGraph(PRM* planner, const WDG& graph);
    void drawFreeGraph(TOGGLEPRM* planner, const WDG& graph, vector<struct subset> graph_set);
    void drawObstGraph(TOGGLEPRM* planner, const WDG& graph);
    void drawMixedGraph(LAZYTOGGLEPRM* planner, const WDG& graph);
    void drawFreeGraph(LAZYTOGGLEPRM* planner, const WDG& graph);
    void drawObstGraph(LAZYTOGGLEPRM* planner, const WDG& graph);
};

#endif /* defined(__disc__Display__) */
