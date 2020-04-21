/*************************************************************
 * YATWM, Yet Another Teapot With Menus                      *
 *                                                           *
 * GLUT example code v0.1, 27 june 2003                      *
 *                                                           *
 * by Elie De Brauwer, look at  http://mindfuck.de-brauwer.be*
 * for more recent versions, you can contact me by email     *
 * elie@de-brauwer.be for questions, remarks or bugs,        *
 * this file is available under the terms of the GNU         *
 * general public license, available at                      *
 * http://www.gnu.org/licenses/gpl.html                      *
 * You may redistribute this without changing this header    *
 *************************************************************/


#include <iostream>
#include <GL/glut.h>
#include <cstdlib>
using namespace std;

void createMenu(void);
void menu(int value);
void disp(void);

static int win;
static int menid;
static int submenid;
static int primitive = 0;

int main(int argc, char **argv){

  // normal initialisation
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);

  win = glutCreateWindow("GLUT MENU");
  
  // put all the menu functions in one nice procedure
  createMenu();

  // set the clearcolor and the callback
  glClearColor(0.0,0.0,0.0,0.0);

  glutDisplayFunc(disp);

  // enter the main loop
  glutMainLoop();

}


void createMenu(void){
  //////////
  // MENU //
  //////////

  // Create a submenu, this has to be done first.
  submenid = glutCreateMenu(menu);

  // Add sub menu entry
  glutAddMenuEntry("Teapot", 2);
  glutAddMenuEntry("Cube", 3);
  glutAddMenuEntry("Torus", 4);

  // Create the menu, this menu becomes the current menu
  menid = glutCreateMenu(menu);

  // Create an entry
  glutAddMenuEntry("Clear", 1);

  glutAddSubMenu("Draw", submenid);
  // Create an entry
  glutAddMenuEntry("Quit", 0);

  // Let the menu respond on the right mouse button
  glutAttachMenu(GLUT_RIGHT_BUTTON);


}

void disp(void){
  // Just clean the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // draw what the user asked
  if(primitive == 1){
    glutPostRedisplay();
  }else if(primitive == 2){
    glutWireTeapot(0.5);
  }else if(primitive == 3){
    glutWireCube(0.5);
  }else if(primitive == 4){
    glutWireTorus(0.3,0.6,100,100);
  }
  glFlush();
}


void menu(int value){
  if(value == 0){
    glutDestroyWindow(win);
    exit(0);
  }else{
    primitive=value;
  }
  
  // you would want to redraw now
  glutPostRedisplay();
}
