/*
 * file: teapot.cpp
 *
 * from http://mindfuck.de-brauwer.be/articles/glut/.
 *
 * An introduction to GLUT
 * by Elie De Brauwer
 *
 * Goal of this file
 * This file offers a brief overview of what GLUT is and does and how you
 * can use GLUT to create and environment that is usefull for the output
 * of an OpenGL application and how to build these applications. This
 * also contains a list of the most common GLUT functions and procedures,
 * the document is ended with two examples including screenshots.
 * This file is targeted towards Linux users programming C++. 
 *
 * =============================
 * Putting it all together: YAT
 * 
 * The source code below, opens a window, clears it with a black color and
 * draws a teapot, it listens to keyboard presses and writes out the key
 * that was pressed and the coordinates. The program terminates when the
 * user presses "q". YAT is Yet Another Teapot. You can find some
 * screenshots below showing some of the other primitives.
 *
 * This program only uses 2 OpenGL functions, the first one is
 * glClearColor() that sets the Clearcolor (background) to black and
 * glClear() that actually clears the window. In this example no
 * idlefunction is defined and I use single buffering
 * (no double buffering) since no animation is needed. 
 * 
 * */
#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
using namespace std;

// function prototypes
void disp(void);
void keyb(unsigned char key, int x, int y);


// window identifier
static int win;

int main(int argc, char **argv){
 
  //////////
  // INIT //
  //////////

  // initialize glut
  glutInit(&argc, argv);

  // specify the display mode to be RGB and single buffering 
  // we use single buffering since this will be non animated
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

  // define the size
  glutInitWindowSize(500,500);

  // the position where the window will appear
  glutInitWindowPosition(100,100);
  

  // if we would want fullscreen:
  // glutFullScreen();

  // create the window, set the title and keep the 
  // window identifier.
  win = glutCreateWindow("Yet another teapot");

  //////////////
  // CALLBACK //
  //////////////

  glutDisplayFunc(disp);
  glutKeyboardFunc(keyb);
  
  ////////////
  // OPENGL //
  ////////////

  // define the color we use to clearscreen 
  glClearColor(0.0,0.0,0.0,0.0);



  // enter the main loop
  glutMainLoop();

  return 0;
}


void disp(void){

  // do  a clearscreen
  glClear(GL_COLOR_BUFFER_BIT);
  
  // draw something

  //glutWireTeapot(0.5);
  // glutSolidTeapot(0.5);
  // glutWireSphere(0.5,100,100);
  // glutSolidSphere(0.5,100,100);
  // glutWireTorus(0.3,0.5,100,100);
   glutSolidTorus(0.3,0.5,100,100);
  // glutWireIcosahedron();
  // glutSolidIcosahedron();
  // glutWireDodecahedron();
  // glutSolidDodecahedron();
  // glutWireCone(0.5,0.5,100,100);
  // glutSolidCone(0.5,0.5,100,100);
  // glutWireCube(0.5);
  // glutSolidCube(0.5);
}

void keyb(unsigned char key, int x, int y){
  cout << "Pressed key " << key << " on coordinates (" << x << "," << y << ")";
  cout << endl;
  if(key == 'q'){
    cout << "Got q,so quitting " << endl;
    glutDestroyWindow(win);
    exit(0);
  }
}

