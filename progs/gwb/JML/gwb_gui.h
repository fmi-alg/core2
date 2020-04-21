#pragma once

#ifdef __CYGWIN32__
#include "GL/glui.h"
#endif
#ifdef __linux__
#include <GL/glut.h>
#include "GL/glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <glui.h>
#endif


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <math.h>
#include <map>
#include <utility>


/***************************************************
 * namespace:
 ***************************************************/
using namespace std;


extern double rotDeg;
extern double movDeg;
extern double zoomDeg;

extern double startX;                    	//start postion of mouse press 
extern double startY;
extern double startZ;

extern double base;                   	//Initialize the origin rotation arguments
extern double theta;
extern double rad;

extern double rotX;                    	//Initialize the rotation vector
extern double rotY;
extern double rotZ;

extern double lightRate;		    	//The inverse of Lightness

extern bool centered;
extern double centerX;                 	//Initialize the center of the polyhedron
extern double centerY;
extern double centerZ;

extern double colorGap;               	//This is for shading

extern double scalar;                  	//Initialize the scalar of the polyhedron
extern double thetaX;
extern double thetaY;

extern double initRotDeg;			//Initialize the rotation of the polyhedron
extern double initRotAxisX;
extern double initRotAxisY;
extern double initRotAxisZ;

extern double light[]; //Add a light

/* Chee: new parameters */
extern double radius;		  
extern double radius2;		 
extern double height;		
extern double height2;	
extern int nsegments;
extern int nsections;

/*Kai the second Part ( Like the legs of the chair )*/
extern int nsegments2;
extern int nsections2;
extern int nparts;
extern int nparts2;
extern double position2;

/*The third part ( Like the back of the chair ) */
extern double position3;
extern double thickness3;
extern double height3;

/*Kai: Rotation Amplitude*/
extern int rotUnitX;
extern int rotUnitY;
extern int rotUnitZ;

extern int comLen;
/*View Point*/
extern GLdouble  	eyeX;
extern GLdouble  	eyeY;
extern GLdouble  	eyeZ;
extern GLdouble  	cX;
extern GLdouble  	cY;
extern GLdouble  	cZ;
extern GLdouble  	upX;
extern GLdouble  	upY;
extern GLdouble  	upZ;

extern GLsizei windowWidth;	    // initial configuration box size
extern GLsizei windowHeight;
extern GLsizei windowPosX;           // initial Window position
extern GLsizei windowPosY;	

extern string inputDir; 	    // Path for input files 
extern string fileName; 	    // Input file name (this is not used currently)
extern string model;	    // model can be "cylinder", "sphere", "trefoil", etc.
extern string mode;		    // mode can be "wire" or "face"

class Solid;

void keyPressed(unsigned char key, int x, int y);
void holdMouse(int x, int y);
void pressMouse( int button, int state, int x , int y);
/*Display without automatically setting the scalar*/
void displayWithoutScalar(void);
/*Display the polyhedron*/
void display(void);
void displayPoly(Solid *s);
void MyInit(void);
void createGUI(int argc,char **argv);