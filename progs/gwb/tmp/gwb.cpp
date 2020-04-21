/* file: gwb.cpp
 *
 * Author: Kai Cao (Supervised by Professor Yap, July 2012)
 *
 * Since Core 2.1.
 * 
 ***************************************************/


/***************************************************
 * include files:
 ***************************************************/
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
#include "Euler_Ops.h"
#include <math.h>

// #include <GL/glut.h>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif


/***************************************************
 * namespace:
 ***************************************************/
using namespace std;


/***************************************************
 * CONSTANTS:
 ***************************************************/
#define SQRTWO sqrt(2)
#define SQRTFIVE sqrt(5)
#define PHI ((1+SQRTFIVE)/2)
#define WIRE

/**************************************************
* PARAMETERS:
**************************************************/
int interactive=0;                  // mode of interaction
                                    //    =0 means non-interactive, >0 means interactive.
string inputDir("inputs"); 		// Path for input files 
string fileName("cube.gwb"); 	      // Input file name

GLsizei windowWidth = 512;	      // initial configuration box size
GLsizei windowHeight = 512;

GLsizei windowPosX = 200;           // initial Window position
GLsizei windowPosY = 200;	

double startX=0;                    //start postion of mouse press 
double startY=0;
double startZ=0;

double  base=0.0;                   //Initialize the origin rotation arguments
double theta=0.0;
double rad=0.0;

double rotX=0.0;                    //Initialize the rotation vector
double rotY=0.0;
double rotZ=0.0;

double centerX=0.0;                 //Initialize the center of the polyhedron
double centerY=0.0;
double centerZ=0.0;

double colorGap=1.0;               //This is for shading

double scalar=1.0;                  //Initialize the scalar of the polyhedron
/*Declaration of mouse operations*/
void pressMouse(int button,int state,int x,int y);
void holdMouse(int x,int y);
Euler_Ops *eo;

/************************************************
 * GUI STUFF:
 ************************************************/

/*Initialize the window paint*/

void MyInit(void){
  glClearColor(1,1,1,0);
  glColor3f(1,0,0);
  glPointSize(10);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0,(GLdouble)ww,0.0,(GLdouble)wh);
  glOrtho(-100.0,(GLdouble)windowWidth,-100.0,(GLdouble)windowHeight,-1000,1000);
}//MyInit

void displayPoly(Solid *s){
  /*Get Edges */
  Vec<Edge *> *es=s->sedges;

  Vec<Face *> *fs=s->sfaces;
  colorGap=1.0/(fs->size());

  /*center of the polyhedron*/
  Vec<double> *c=s->center();

  /*center coordinates of the polyhedron*/
  centerX=(*c)[0];
  centerY=(*c)[1];
  centerZ=(*c)[2];
  

  /*Wire Solids*/
  #ifdef WIRE

  glBegin(GL_LINES);
  for (unsigned int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
  glEnd();

  #elif defined(SURFACE)
  /*Initialize the random seed*/
  srand((unsigned)time(0));
  for (unsigned int i=0;i<fs->size();i++){
    /*Now we get the face*/
    Face *f=(*fs)[i];

    /*Now we have the loops*/
    Vec< Loop *> *ls=f->floops;

    for (unsigned int j=0;j<ls->size();j++){
      
      /*Leading HalfEdge*/
      HalfEdge *lhe=(*ls)[j]->ledg;
      HalfEdge *the=lhe->nxthe;

      /*Reset the face color*/
      glColor3f((0.0+rand())/RAND_MAX,(0.0+rand())/RAND_MAX,(0.0+rand())/RAND_MAX);
      
      glBegin(GL_POLYGON);

      Vertex *v=lhe->start;
      glVertex3f(v->getX(),v->getY(),v->getZ());

      while(the!=lhe){
            v=the->start;
            glVertex3f(v->getX(),v->getY(),v->getZ());
            the=the->nxthe;
      }//while
      
      
      glEnd();

    }//for j


  }//for i

  #endif
}
/*Display the polyhedron*/
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  Solid *s=(*(eo->solids))[0]; 
  
  /*Modify Scalar*/
  scalar=windowWidth/(s->diameter()+0.0);

  glTranslatef(windowWidth/2.0,windowHeight/2.0,centerZ);
  glScalef(scalar, scalar, scalar);
  glRotatef(45,1,1,1);
  glTranslatef(-centerX,-centerY,-centerZ);
  displayPoly(s);

   glFlush();

}//display

/*Mouse operations*/

void pressMouse(int button,int state,int x ,int y){
if (state==GLUT_DOWN){
  startX=x;
  startY=windowHeight-y;
  }
 if(state==GLUT_UP){
  holdMouse(x,y);
  base=rad;
 }
}//pressMouse

void holdMouse(int x, int y){
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_POINTS);
    glPointSize(50);
    glVertex2i(x,wh-y);
    glEnd();
    glFlush();*/

   Solid *s=(*(eo->solids))[0]; 
   vector<Edge *> *es=s->sedges;
   //cout<<"CENTERx="<<centerX<<endl;
   //cout<<"CENTERy="<<centerY<<endl;
   y=windowHeight-y;
   double radius=sqrt((startX-centerX)*(startX-centerX)+(startY-centerY)*(startY-centerY)+(startZ-centerZ)*(startZ-centerZ));
   double distance=sqrt((x-startX)*(x-startX)+(y-startY)*(y-startY));
   theta=acos((2.0*radius*radius-distance*distance)/(2.0*radius*radius))/PI*180.0;
   double rotX=(startY-centerY)*(0-centerZ)-(startZ-centerZ)*(y-centerY);
   double rotY=(startZ-centerZ)*(x-centerX)-(startX-centerX)*(0-centerZ);
   double rotZ=(startX-centerX)*(y-centerY)-(startY-centerY)*(x-centerX);
   //int direction=(startX-centerX)*(y-centerY)-(startY-centerY)*(y-centerY)>0?1:-1;

   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //glTranslatef(x-startX,y-startY,0);
   //rad=base+theta*direction;
  /*Same operation but including rotation*/
  glTranslatef(windowWidth/2.0,windowHeight/2.0,centerZ);
  glScalef(scalar, scalar, scalar);
  glRotatef(45,1,1,1);
  glRotatef(theta,rotX,rotY,rotZ);
  glTranslatef(-centerX,-centerY,-centerZ);

      
  displayPoly(s); 

  /*glBegin(GL_LINES);
  for (unsigned int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
   glEnd();*/

   glBegin(GL_POINTS);
   glPointSize(50);
   glVertex3f(centerX,centerY,centerZ);
   glEnd();

   glFlush();
  
}//holdMouse

//**************************************************
// MAIN:
//*************************************************

int main(int argc,char **argv){

	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) inputDir  = argv[2];		      // path for input files
	if (argc > 3) fileName = argv[3]; 		      // Input file name
	if (argc > 4) windowWidth = atof(argv[4]);	// windowWidth
	if (argc > 5) windowHeight= atof(argv[5]);	// windowHt
	if (argc > 6) windowPosX = atoi(argv[6]);	      // window X pos
	if (argc > 7) windowPosY = atoi(argv[7]);	      // window Y pos
cout << "winX = " << windowPosX << endl;
cout << "winY = " << windowPosY << endl;

      eo=new Euler_Ops();
      /*Read from file*/  
      string fileAdd=inputDir+"/"+fileName;
      ifstream ifile;
      ifile.open(fileAdd.c_str());

      
      /*Read every line*/
      string line;
      while (ifile.good()){
            getline(ifile,line);
            cout<<line<<endl;



            /*Analyze the line*/
            string command;
            Vec<double> *ids= new Vec<double>();

            /*Read first word*/
            string first;
            istringstream iss(line);
            iss>>first;

            /*If it is an empty line,go ahead*/
            if(first.length()<=0)
                  continue;

            /*If the line begins with '#' it must be a comment*/
            if(first[0]=='#')
                  continue;
            
            /*Then the first word must be command*/
            command=first;
            while(iss){
                 double num;
                 iss>>num;
                 ids->push_back(num);
            }//while(iss)

            /*We need to erase the last one*/
            ids->erase(ids->end()-1);


            /*Different commands*/
            if (command.compare("mvfs")==0)
                  eo->mvfs(ids);
            else if(command.compare("kvfs")==0)
                  eo->kvfs(ids);
            else if(command.compare("mev")==0)
                  eo->mev(ids);
            else if(command.compare("kev")==0)
                  eo->kev(ids);
            else if(command.compare("mef")==0)
                  eo->mef(ids);
            else if(command.compare("kef")==0)
                  eo->kef(ids);
            else if(command.compare("kemr")==0)
                  eo->kemr(ids);
            else if(command.compare("mekr")==0)
                  eo->mekr(ids);
            else if(command.compare("kfmrh")==0)
                  eo->kfmrh(ids);
            else if(command.compare("mfkrh")==0)
                  eo->mfkrh(ids);
            else if(command.compare("ringmv")==0)
                  eo->ringmv(ids);
            else {
                  cout<<command<<" is not a valid command."<<endl;
            }





      }//while (ifile.good())
  //Vec<Solid *> *ss=eo->solids;



 
  
  

/*Prepare for the window*/
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize(windowWidth,windowHeight);
   glutInitWindowPosition(150,150);
   glutCreateWindow("GWB");

/*Initialze the window*/
   MyInit();

/*Display the polyhedron and manage rotation*/
   glutDisplayFunc(display);
   glutMouseFunc(pressMouse);
   glutMotionFunc(holdMouse);
   glutMainLoop();
  

}//main


//**************************************************
//* END
//**************************************************



  /*cout<<"Solid no"<<cube->solidno<<endl;

  cout<<"sfaces ="<<fs<<endl;
  cout<<"sfaces size="<<fcube->size()<<endl;
  cout<<"face no="<<(*fs)[0]->faceno<<endl;

  cout<<"sedges ="<<es<<endl;
  cout<<"sedges size="<<ecube->size()<<endl;

  cout<<"sverts ="<<vs<<endl;
  cout<<"sverts size="<<vcube->size()<<endl;
  cout<<"vertex no="<<(*vs)[0]->vertexno<<endl;

  cout<<"Loops size="<<lcube->size()<<endl;
  cout<<"Loop 0 = "<<(*ls)[0]<<endl;
  cout<<"Outer Loop="<<l<<endl;
  l->print();
  HalfEdge *he=l->ledg;
  he->print();
  
  he->nxthe->print();
  he->prvhe->print();

  cout<<"he= "<<he<<endl;
  cout<<"nxthe= "<<he->nxthe<<endl;
  cout<<"prvhe= "<<he->prvhe<<endl;
  cout<<"he start vertex"<<he->start<<endl;
  cout<<"nxthe start vertex"<<he->nxthe->start<<endl;*/
