/*************************************************************
 * YATWT+, Yet Another Teapot With Timers that  work         *
 *                                                           *
 * GLUT example code v0.1, 20 july 2003                      *
 *                                                           *
 * by Elie De Brauwer, look at  http://mindfuck.de-brauwer.be*
 * for more recent versions, you can contact me by email     *
 * elie@de-brauwer.be for questions, remarks or bugs,        *
 * this file is available under the terms of the GNU         *
 * general public license, available at                      *
 * http://www.gnu.org/licenses/gpl.html                      *
 * You may redistribute this without changing this header    *
 *************************************************************/



#include <iostream>    // for output
#include <cstdlib>     // for exit and rand
#include <GL/glut.h>   // for gl* glut*
#include <ctime>       // for seeding rand()
using namespace std;

#define TEAPOT 0
#define TORUS 1
#define CUBE 2
#define SPHERE 3

/////////////////
// global vars //
/////////////////

static int object;
static int win;
static float r;
static float g;
static float b;

////////////////
// prototypes //
////////////////

void display(void);
void keyboard(unsigned char key, int x, int y);
void timerColor(int value);
void changeShape();

//////////
// main //
//////////

int main(int argc, char **argv){

  // init
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  
  win = glutCreateWindow("YATWT");
  
  // callback
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  // change color each second
  timerColor(0);

  // clearcolor & main loop
  glClearColor(1.0,1.0,1.0,0.0);

  // seed random
  // see `man srand` for the user of srand and random
  time_t timer;
  srand(time(&timer));

  glutMainLoop();
  return 0;
}

/////////////
// display //
/////////////

void display(void){

  // clean te window
  glClear(GL_COLOR_BUFFER_BIT);
  
  // define the color in which te object has to be drawn
  glColor3f(r,g,b);
  // determine which object to draw
  switch(object){
  case TEAPOT:
    glutSolidTeapot(0.5);
    break;
  case TORUS:
    glutSolidTorus(0.25,0.50,100,100);
    break;
  case CUBE:
    glutSolidCube(0.5);
    break;
  case SPHERE:
    glutSolidSphere(0.5,100,100);
    break;
  }
  glutSwapBuffers();
}

//////////////
// keyboard //
//////////////

void keyboard(unsigned char key, int x, int y){
  if(key=='q'){
    cout << "Quitting" << endl;
    glutDestroyWindow(win);
    exit(0);
  }
}

////////////////
// timercolor //
////////////////

void timerColor(int value){
  static int counter=0;
  if(counter == 4){
    counter =0;
    changeShape();
  }else{
    counter++;
  }

  // get new color or a value in [0,1]
  r = (1.0*(random()%256))/256.0;
  g = (1.0*(random()%256))/256.0;
  b = (1.0*(random()%256))/256.0;

  // say what we are doing
  cout << "New color is (" << r << "," << g <<  "," << b << ")" << endl;

  // draw it + reinitialise timer
  glutPostRedisplay();
  glutTimerFunc(1000,timerColor,0);
}

/////////////////
// changeShape //
/////////////////

void changeShape(){
  // generate new object 
  object = random()%4;

  // say what we are doing
  cout << "Next object will be a ";
  switch(object){
  case TEAPOT:
    cout << "teapot" << endl;
    break;
  case TORUS:
    cout << "torus" << endl;
    break;
  case CUBE:
    cout << "cube" << endl;
    break;
  case SPHERE:
    cout << "sphere" << endl;
    break;
  }
}
