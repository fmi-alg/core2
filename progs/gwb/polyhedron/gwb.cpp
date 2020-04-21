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
#include "Euler_Ops.h"
#include "GL/glut.h"
#include <math.h>
#include <map>
#include <utility>

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
//#define WIRE
//#define MULT_SURFACE
#define SINGLE_SURFACE

/**************************************************
* PARAMETERS:
**************************************************/
int interactive=0;                  // interactive or not:
                                    // <=0 means non-interactive, >0 means interactive.
string inputDir("inputs"); 	    // Path for input files 
string fileName("cube.gwb"); 	    // Input file name (this is not used currently)
string model("chain");	    // model can be "cylinder", "sphere", "trefoil", etc.
string mode("wire");		    // mode can be "wire" or "face"
GLsizei windowWidth = 512;	    // initial configuration box size
GLsizei windowHeight = 512;
GLsizei windowPosX = 200;           // initial Window position
GLsizei windowPosY = 200;	

/*The Rotation, Movement, Zoom in&out degree*/
double rotDeg=1;
double movDeg=10;
double zoomDeg=1;

double startX=0;                    	//start postion of mouse press 
double startY=0;
double startZ=0;

double base=0.0;                   	//Initialize the origin rotation arguments
double theta=0.0;
double rad=0.0;

double rotX=0.0;                    	//Initialize the rotation vector
double rotY=0.0;
double rotZ=0.0;

double lightRate=1.0;		    	//The inverse of Lightness

bool centered=false;
double centerX=0.0;                 	//Initialize the center of the polyhedron
double centerY=0.0;
double centerZ=0.0;

double colorGap=1.0;               	//This is for shading

double scalar=1.0;                  	//Initialize the scalar of the polyhedron
double thetaX=0;
double thetaY=0;

double initRotDeg=45;			//Initialize the rotation of the polyhedron
double initRotAxisX=1;
double initRotAxisY=1;
double initRotAxisZ=1;

double light[]={1,1,1,100,100,100}; //Add a light

/* Chee: new parameters */
double radius=100.0;		  
double radius2=10.0;		 
double height=50.0;		
double height2=20.0;	
int nsegments=12;
int nsections=20;

/*Kai the second Part ( Like the legs of the chair )*/
int nsegments2=5;
int nsections2=1;
int nparts=10;
int nparts2=20;
double position2=80;

/*The third part ( Like the back of the chair ) */
double position3=90;
double thickness3=10;
double height3=50;

/*Kai: Rotation Amplitude*/
int rotUnitX=5;
int rotUnitY=5;
int rotUnitZ=5;

int comLen=1000;
/*View Point*/
GLdouble  	eyeX=0;
GLdouble  	eyeY=0;
GLdouble  	eyeZ=0;
GLdouble  	cX=0;
GLdouble  	cY=0;
GLdouble  	cZ=0;
GLdouble  	upX=0;
GLdouble  	upY=0;
GLdouble  	upZ=0;
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
  //gluLookAt(eyeX,eyeY,eyeZ,cX,cY,cZ,upX,upY,upZ);
}//MyInit

void displayPoly(Solid *s){
  /*Get Edges */
  Vec<Edge *> *es=s->sedges;

  Vec<Face *> *fs=s->sfaces;
  colorGap=1.0/(fs->size());
if (!centered){
  /*center of the polyhedron*/
  Vec<double> *c=s->center();

  /*center coordinates of the polyhedron*/

  centerX=(*c)[0];
  centerY=(*c)[1];
  centerZ=(*c)[2];
  centered=true;
}//if we do not have a center

  /*Wire Solids*/
 
  if (mode.compare("wire")==0){
  glBegin(GL_LINES);
  for (unsigned int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
  glEnd();
  }//wire

  else if (mode.compare("mult_surface")==0){
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
      Num* vert=crossProduct(the,lhe);
      
      if (vert[2]>0){
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
      }//if face up

    }//for j


  }//for i

  }//mult_face
  
  else if (mode.compare("single_surface")==0||mode.compare("shading")==0||mode.compare("face")==0){
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
      //glColor3f((0.0+rand())/RAND_MAX,(0.0+rand())/RAND_MAX,(0.0+rand())/RAND_MAX);
      
      /*Start vertex*/
      Vertex *v=lhe->start;
      /*Let us add a light*/
      double rate=255.0/pow(pow(v->getX()-light[3],2)+pow(v->getY()-light[4],2)+pow(v->getZ()-light[5],2),lightRate);
      glColor3f(rate,0,0);
	glEnable(GL_DEPTH_TEST);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);	
      glBegin(GL_POLYGON);

      
      glVertex3f(v->getX(),v->getY(),v->getZ());

      while(the!=lhe){
            v=the->start;
            glVertex3f(v->getX(),v->getY(),v->getZ());
            the=the->nxthe;
      }//while
      
      
      glEnd();

    }//for j


  }//for i

  }//single_surface

}//displaypoly
/*Display the polyhedron*/
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  for (unsigned int i=0;i<eo->solids->size();i++){
  Solid *s=(*(eo->solids))[i];
  
  /*Modify Scalar*/
  scalar=windowWidth/(s->diameter()+0.0)/4.0;
  //scalar=1.0;

  glTranslatef(windowWidth/2.0,windowHeight/2.0,centerZ);
  glScalef(scalar, scalar, scalar);
  //gluLookAt(eyeX,eyeY,eyeZ,cX,cY,cZ,upX,upY,upZ);
  glRotatef(initRotDeg,initRotAxisX,initRotAxisY,initRotAxisZ);
  glTranslatef(-centerX,-centerY,-centerZ);
  displayPoly(s);
  }

   glFlush();

}//display

/*Display without automatically setting the scalar*/
void displayWithoutScalar(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  //scalar=1.0;

  glTranslatef(windowWidth/2.0,windowHeight/2.0,centerZ);
  glScalef(scalar, scalar, scalar);
  //gluLookAt(eyeX,eyeY,eyeZ,cX,cY,cZ,upX,upY,upZ);
  //glRotatef(45,1,1,1);
  glRotatef(initRotDeg,initRotAxisX,initRotAxisY,initRotAxisZ);	
	
  glTranslatef(-centerX,-centerY,-centerZ);
  
  /*Rebuild the rotation*/ 
  glRotatef(rotX,0,1,0);
  glRotatef(rotY,-1,0,0);	

  for (unsigned int i=0;i<eo->solids->size();i++){
  	Solid *s=(*(eo->solids))[i];
  displayPoly(s);
  }

  glFlush();
}//display without scalar

/*Mouse operations*/

void pressMouse(int button,int state,int x ,int y){

if (state==GLUT_DOWN){
  startX=x;
  startY=windowHeight-y;
  //glRotatef(thetaX,0,1,0);
  //glRotatef(thetaY,-1,0,0);
  }
 if(state==GLUT_UP){
  startX=x;
  startY=windowHeight-y;
  /*holdMouse(x,y);
  base=rad;*/
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
//for (int i=0;i<eo->solids->size();i++){
  // Solid *s=(*(eo->solids))[i]; 
   // Chee: es is unused:
   //       vector<Edge *> *es=s->sedges;
   //cout<<"CENTERx="<<centerX<<endl;
   //cout<<"CENTERy="<<centerY<<endl;
   y=windowHeight-y;
   /*double radius=sqrt((startX-centerX)*(startX-centerX)+(startY-centerY)*(startY-centerY)+(startZ-centerZ)*(startZ-centerZ));
   double distance=sqrt((x-startX)*(x-startX)+(y-startY)*(y-startY));
   theta=acos((2.0*radius*radius-distance*distance)/(2.0*radius*radius))/PI*180.0;
   double rotX=(startY-centerY)*(0-centerZ)-(startZ-centerZ)*(y-centerY);
   double rotY=(startZ-centerZ)*(x-centerX)-(startX-centerX)*(0-centerZ);
   double rotZ=(startX-centerX)*(y-centerY)-(startY-centerY)*(x-centerX);*/
   //int direction=(startX-centerX)*(y-centerY)-(startY-centerY)*(y-centerY)>0?1:-1;

   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //glTranslatef(x-startX,y-startY,0);
   //rad=base+theta*direction;
  /*Same operation but including rotation*/
  glTranslatef(windowWidth/2.0,windowHeight/2.0,centerZ);
  glScalef(scalar, scalar, scalar);
  //glRotatef(45,1,1,1);
  //glRotatef(theta,rotX,rotY,rotZ);
  
  rotX+=x-startX;
  rotY+=y-startY;
  glRotatef(rotX,0,1,0);
  glRotatef(rotY,-1,0,0);
  //glRotatef(x-startX,0,1,0);
  //glRotatef(y-startY,-1,0,0);
  thetaX+=x-startX;
  thetaY+=y-startY;

  glTranslatef(-centerX,-centerY,-centerZ);

      
  displayWithoutScalar(); 

  /*glBegin(GL_LINES);
  for (unsigned int i=0;i<es->size();i++){
    Edge *e=(*es)[i];
    Vertex *v1=e->he1->start;
    Vertex *v2=e->he2->start;
   glVertex3f(v1->getX(),v1->getY(),v1->getZ());
   glVertex3f(v2->getX(),v2->getY(),v2->getZ());

  }
   glEnd();*/
//}
   glBegin(GL_POINTS);
   glPointSize(50);
   glVertex3f(centerX,centerY,centerZ);
   glEnd();

   glFlush();

   startX=x;
   startY=y;
  
}//holdMouse

/*Key Pressed*/
void keyPressed(unsigned char key, int x, int y){
	if (key=='+'){
		scalar+=zoomDeg;
		displayWithoutScalar();
	}
	
	if (key=='-'){
		scalar-=zoomDeg;
		displayWithoutScalar();
	}
   
  	if (key=='a'||key=='A'){
		rotX+=rotDeg;
		displayWithoutScalar();
	}
	
 	if (key=='d'||key=='D'){
		rotX-=rotDeg;
		displayWithoutScalar();
	}
	
	if (key=='w'||key=='W'){
		rotY-=rotDeg;
		displayWithoutScalar();
	}

	if (key=='s'||key=='S'){
		rotY+=rotDeg;
		displayWithoutScalar();
	}
	
	if (key=='j'||key=='J'){
		centerX+=movDeg;
		displayWithoutScalar();
	}
	
	if (key=='l'||key=='L'){
		centerX-=movDeg;
		displayWithoutScalar();
	}
	
	if (key=='i'||key=='I'){
		centerY-=movDeg;
		displayWithoutScalar();
	}
	
	if (key=='k'||key=='K'){
		centerY+=movDeg;
		displayWithoutScalar();
	}
}

//**************************************************
// MAIN:
//*************************************************

int main(int argc,char **argv){
	
	/*The Euler_Ops Object*/
	eo=new Euler_Ops();

	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) inputDir  = argv[2];		      // path for input files
	if (argc > 3) fileName = argv[3]; 		      // Input file name
		if (interactive == 0) {
		   
	if (argc > 4) windowWidth = atof(argv[4]);	// windowWidth
	if (argc > 5) windowHeight= atof(argv[5]);	// windowHt
	if (argc > 6) windowPosX = atoi(argv[6]);	// window X pos
	if (argc > 7) windowPosY = atoi(argv[7]);	// window Y pos
	if (argc > 8) mode=argv[8];			// mode = face or wire
	if (argc > 9) model=argv[9];			// model = cube, trefoil, etc, etc
	/* model parameters */
	if (argc > 10) radius = atof(argv[10]);
	if (argc > 11) height = atof(argv[11]);
	if (argc > 12) nsegments = atoi(argv[12]);
	if (argc > 13) nsections = atoi(argv[13]);
	if (argc > 14) height2 = atof(argv[14]);
	if (argc > 15) radius2 = atof(argv[15]);
	/*Rotation Size*/
	if (argc > 16) rotUnitX=atoi(argv[16]);
	if (argc > 17) rotUnitY=atoi(argv[17]);
	if (argc > 18) rotUnitZ=atoi(argv[18]);
	/*model parameters*/
	if (argc > 19) nsegments2=atoi(argv[19]);
	if (argc > 20) nsections2=atoi(argv[20]);
	if (argc > 21) nparts=atoi(argv[21]);
 	if (argc > 22) nparts2=atoi(argv[22]);
	if (argc > 23) position2=atoi(argv[23]);
	if (argc > 24) position3=atoi(argv[24]);
 	if (argc > 25) thickness3=atoi(argv[25]);
	if (argc > 26) height3=atoi(argv[26]);
	if (argc > 27) lightRate=atof(argv[27]);
	if (argc > 28) initRotDeg=atof(argv[28]);
	if (argc > 29) initRotAxisX=atof(argv[29]);
	if (argc > 30) initRotAxisY=atof(argv[30]);
	if (argc > 31) initRotAxisZ=atof(argv[31]);

	/*Parameters for the model */


      
	//double par[argc-3];
	//double par[]={5,1,50,20,8,1,5,8,8,1,6};
	/************************** Chee commented this out:
	  for (int i=0;i<argc-3;i++)
		par[i]=atof(argv[i+3]);
	  for (int i=0;i<3;i++)
		cout<<par[i]<<endl;
	************************* */
	/*if (model.compare("table")==0)
	{
		par[0] = radius;	//table rad
		par[1] = height; 	//table ht
		par[2] = height2; 	//legPos
		par[3] = 6;		//legNo
		par[4] = radius2;		//legRad
		par[5] = nsections;	//legHor
		par[6] = height2;	//legHeight
	} else if (model.compare("rocket")==0)
	{
		par[0] = radius;
		par[1] = height; 
		par[2] = height2; 	// coneheight for rocket head
		par[3] = nsegments;
		par[4] = nsections;
	} else{
		par[0] = radius;
		par[1] = height; // or "radius2"
		par[2] = nsegments;
		par[3] = nsections;
	}*/

	eo->prim(model);
	//(*(eo->solids))[0]->print();

	
	}//interactive=0
      /*Read from file*/  
      string fileAdd=inputDir+"/"+fileName;
      
      ifstream ifile;
      ifile.open(fileAdd.c_str());
      
      /*Variable Maps*/
      map<string, double> variables;
      
      /*Read every line*/
      string line;
	/*If we Read from file*/
	if (interactive==2){
	/*The model name*/
	model=fileName;
	
	/*Display direction*/
	if (argc > 4) initRotDeg=atof(argv[4]);
	if (argc > 5) initRotAxisX=atof(argv[5]);
	if (argc > 6) initRotAxisY=atof(argv[6]);
	if (argc > 7) initRotAxisZ=atof(argv[7]);
	
      while (ifile.good()){
	    
	    /*Read a line*/
            getline(ifile,line);
	    
	    /*If it is an empty line,go ahead*/
            if(line.length()<=0)
                  continue;

            /*If the line begins with '#' it must be a comment*/
            if(line[0]=='#')
                  continue;
	    //cout<<line<<endl;
	    /*The = position*/
	    int eqPos=line.find('=');
	    //cout<<"line "<<line<<endl;
            /*If it contains =, it is not a command, it is a variable*/
	    if ((unsigned)eqPos<line.size()&&eqPos>=0){
		/*Read variables*/
            	string first=line.substr(0,eqPos);
		istringstream iss2(first);
		string var;
		iss2>>var;
		//cout<<var<<endl;

		string second=line.substr(eqPos+1);
		istringstream iss3(second);
		double num;
		iss3>>num;
		
		variables[var]=num;
		//second=second.erase(second.find_last_not_of(" \n\r\t")+1);
		//cout<<num<<endl;
		
		//cout<<line.size()<<line<<endl;
	    }//variables
		
	    else{
	    //cout<<line.find("=")<<endl;
            /*Analyze the line*/
            string command;
            Vec<double> *ids= new Vec<double>();
	    
	    //cout<<line<<endl;
            /*Read first word*/
            string first;
            istringstream iss(line);
            iss>>first;

            double fileScalar=variables["scalar"]==0?1:variables["scalar"];
            
            /*Then the first word must be command*/
            command=first;
            while(iss){
		 /*Read the variable*/
		 string var;
		 /*Push the number*/
                 double num;
                 iss>>var;
	         //cout<<"haha "<<var<<endl;
		 /*Parse the variable*/
		 int varPos=var.find('$');
		 if ((unsigned)varPos<var.size()&&varPos>=0){
			string tempVar;          // string which will contain the result

			ostringstream convert;   // stream used for the conversion

			convert <<variables[var.substr(varPos+1)];      // insert the constant value

			tempVar = convert.str(); // set 'Result' to the contents of the stream
			num=atof((var.substr(0,varPos)+tempVar).c_str());
		 }
		 else 
			num=atof(var.c_str());
		 //cout<<"num="<<num<<endl;
		 
		 //cout<<var<<"="<<num<<endl;
                 ids->push_back(num);
            }//while(iss)

            /*We need to erase the last one*/
            ids->erase(ids->end()-1);

            /*Different commands*/
            if (command.compare("mvfs")==0){
		  
	          (*ids)[ids->size()-1]*=fileScalar;
		  (*ids)[ids->size()-2]*=fileScalar;
		  (*ids)[ids->size()-3]*=fileScalar;
			
                  eo->mvfs(ids);
	    }
            else if(command.compare("kvfs")==0)
                  eo->kvfs(ids);
            else if(command.compare("mev")==0){
		  (*ids)[ids->size()-1]*=fileScalar;
		  (*ids)[ids->size()-2]*=fileScalar;
		  (*ids)[ids->size()-3]*=fileScalar;
                  eo->mev(ids);
	    }
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
	}//command
      }//while (ifile.good())
  }//if read file
  //Vec<Solid *> *ss=eo->solids;
  
/*Prepare for the window*/
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_RGB  | GLUT_SINGLE);
   glutInitWindowSize(windowWidth,windowHeight);
   glutInitWindowPosition(windowPosX,windowPosY);
   glutCreateWindow(argc>2?model.c_str():"GWB");

/*Initialze the window*/
   MyInit();

/*Display the polyhedron and manage rotation*/
   glutDisplayFunc(displayWithoutScalar);
   glutMouseFunc(pressMouse);
   glutMotionFunc(holdMouse);
   glutKeyboardFunc(keyPressed);
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
