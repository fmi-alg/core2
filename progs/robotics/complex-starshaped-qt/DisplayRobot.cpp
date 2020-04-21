// Header
#include "DisplayRobot.h"

// Standard Library
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <ctime>
#include <Triangle.h>

// Global Variables in complex-starshaped-qt.cpp
extern vector<Triangle> compRoboTriangles;

extern double R0;
extern Color clr_totalFREE;
extern Color clr_partialFREE;
extern Color clr_MIXED;
extern Color clr_STUCK;
extern Color clr_EPS;
extern Color clr_UNKNOWN;
extern Color clr_start;
extern Color clr_goal;
extern Color clr_path;
extern Color clr_robot;


/*
 * CONSTRUCTOR
 *
 * Define data members
 */
DisplayRobot::DisplayRobot(QWidget* parent):
    QOpenGLWidget(parent)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
DisplayRobot::~DisplayRobot()
{}

void DisplayRobot::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(1.0f);
}


/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void DisplayRobot::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //renderScene();
    glFlush();
}


/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void DisplayRobot::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DisplayRobot::renderScene() {
    Pose mid = Pose(50, 50, 0);
    drawRobot(mid, clr_robot);
    drawCircle();
}

void DisplayRobot::drawRobot(Pose p, Color c){
    for (Triangle & tri:compRoboTriangles) {
        Corner boxcenter(p.x,p.y);

        Corner A = Corner(tri.a.x, tri.a.y);
        A.rotate(p.theta,0,0);
        A = A + boxcenter;
        Corner B = Corner(tri.b.x, tri.b.y);
        B.rotate(p.theta,0,0);
        B = B + boxcenter;
        Corner C = Corner(tri.c.x, tri.c.y);
        C.rotate(p.theta,0,0);
        C = C + boxcenter;

        drawTriangle(Pose(A.x,A.y),
                     Pose(B.x,B.y),
                     Pose(C.x,C.y),
                     c, true, true);
    }

}


void DisplayRobot::drawLine(Pose endPoint) {
    glColor4fv(Color(0,0,0,1).rgba);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);
        glVertex2f( 50, 50 );
        glVertex2f( endPoint.x, endPoint.y );
    glEnd();
    glLineWidth(1.0);
}

void DisplayRobot::drawTriangle(Pose a, Pose b, Pose c, Color clr, bool draw_solid, bool draw_outline) {
    if (draw_solid) {
        glColor4fv(clr.rgba);
        glBegin(GL_TRIANGLES);
            glVertex2f( a.x, a.y );
            glVertex2f( b.x, b.y );
            glVertex2f( c.x, c.y );
        glEnd();
    }
    if (draw_outline) {
        glColor4fv(Color(0,0,0,1).rgba);
        glLineWidth(1.0);
        glBegin(GL_LINE_STRIP);
            glVertex2f( a.x, a.y );
            glVertex2f( b.x, b.y );
            glVertex2f( c.x, c.y );
            glVertex2f( a.x, a.y );
        glEnd();
        glLineWidth(1.0);
    }
}

void DisplayRobot::drawCircle() {
    glColor4fv(Color(0,0,0,1).rgba);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<360; ++i) {
       float degInRad = (float)(i*PI/180.0f);
       glVertex2f(50+cos(degInRad)*R0,50+sin(degInRad)*R0);
    }
    glEnd();
    glLineWidth(1.0);
}
