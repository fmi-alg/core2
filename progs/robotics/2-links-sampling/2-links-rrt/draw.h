#ifndef DRAW_H__
#define DRAW_H__

#include <stdio.h>
#include <iostream>
#include <string>
#include "GL/gli.h"
#include "GL/gliDump.h"
#include "Vector.h"
#include "RRT.h"
using namespace mathtool;
using namespace std;


void drawLine(const Vector4d& start, const Vector4d& end, double width);

//copied from meshlab
void DisplayBackground(void);

void DrawLine(const Vector3d& p1, const Vector3d& p2);


void DrawTree(const RRT_TREE& tree);
void DrawPolygons(RRT_OBJS& rects);
void DrawPath(const PATH& path);
void DrawRobot(RRT_ROBOT& robot);
void DrawAll();

void Display();
//-----------------------------------------------------------------------------
// other regular openGL callback functions
bool InitGL();

void Reshape( int w, int h);

// show keys for controlling the rendering
void printGUIKeys();

void updateWindownTitle();

void Keyboard( unsigned char key, int x, int y );

void resetCamera();

void Draw3D(int argc, char ** argv);

#endif // DRAW_H__
