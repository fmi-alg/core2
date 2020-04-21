
/**
 *Author:Kai Cao
*/


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
#include <vector>
#include <math.h>
#include <X11/Xlib.h>
//	#include <GL/glut.h>
//	#include <GL/glui.h>


using namespace std;


typedef float matrix[4][4];
typedef short Id;
typedef double Num;
typedef Num coords[4];
typedef Num Coords[4];
typedef Num Dir[3];
enum Pos {LEFT,ON,RIGHT,UP,DOWN,OUT,IN,UNKNOWN};
#define Vec vector 
#define DIM 3
#define ZERO 0.0
/*Declaration*/
class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;

/*Built for OpenGL*/
//GLsizei ww=500;
//GLsizei wh=500;
/*void init(void){
  glClearColor(1,1,1,0);
  glColor3f(1,0,0);
  glPointSize(10);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0,(GLdouble)ww,0.0,(GLdouble)wh);
  glOrtho(0.0,(GLdouble)ww,0.0,(GLdouble)wh,-1000,1000);
}*/
/*********************Vertex with coordinates*********************/
class Vertex
{
public:
  static  int counter;
  Id vertexno;/*vertex identifier*/
  HalfEdge *vedge;/*pointer to halfedge*/
  vector<double> *vcoord;/*vertext coordinates*/
  Solid *s;/*pointer to the solid*/

  /*Getters and Setters*/
  Id getId(){
    return vertexno;
  }
  /*Getter and Setter for the half edge*/
  HalfEdge *getHe(){
    return vedge;
  }

  void setHe(HalfEdge *he){
    vedge=he;
  }


  /*Getter and Setter for the coordinates*/
  vector<double> *getCoord(){
    return vcoord;
  }

  void setCoord(vector<double> *cod){
    vcoord=cod;
  }
  /*Get coordinates seperately*/
  double getX(){
    return (*vcoord)[0];
  }

  void setX(double x){
    (*vcoord)[0]=x;
  }

  double getY(){
    return (*vcoord)[1];
  }

  void setY(double y){
    (*vcoord)[1]=y;
  }

  double getZ(){
    return (*vcoord)[2];
  }

  void setZ(double z){
    (*vcoord)[2]=z;
  }

  /*Solid Getters and Setters*/
  Solid *getSolid(){
    return s;
  }

  void setSolid(Solid *s){
    this->s=s;
  }
  /*Constructors*/
  Vertex(Id no,double x,double y,double z,Solid *s);
  Vertex(double x, double y,double z,Solid *s);
  /*If we just have a pointer to s, the vcoords should be initialize all 0*/
  Vertex(Solid *s);
  Vertex();
  void print(); /*Print the coordinates of the vertex*/


  /*Destructors*/
  ~Vertex();
  

};

/*************HalfEdge with a start point and a mate funciton*******/
class HalfEdge
{
public:

  /*Parent Links*/
  Edge *edg;  /*pointer to parent edge*/
  Vertex *start;  /*pointer to starting vertex */
  Loop *wloop;  /*back pointer to loop*/

  /*This is important for the loop, and actually it is a sequence*/
  HalfEdge *nxthe;  /*pointer to next halfedge*/
  HalfEdge *prvhe;/*pointer to previous halfedge*/

  /*Direction*/
  Num* d;

  /*Getters and Setters*/
  /*edg*/
  Edge *getEdge(){
    return edg;
  }
  void vectorEdge(Edge *e){
    edg=e;
  }

  /*start*/
  Vertex *getStart(){
    return start;
  }
  void vectorStart(Vertex *v){
    start=v;
  }

  /*wloop*/
  Loop *getLoop(){
    return wloop;
  }
  void getLoop(Loop *l){
    wloop=l;
  }
 
  /*previous and next HalfEdge*/
  HalfEdge *getNxthe(){
    return nxthe;
  }
  void setNxthe(HalfEdge *he){
    nxthe=he;
  }
  HalfEdge *getPrvhe(){
    return prvhe;
  }
  void setPrvhe(HalfEdge *he){
    prvhe=he;
  }


  HalfEdge();/*Default constructor*/
  HalfEdge(Vertex *start);/*HalfEdge from start vertex*/
  HalfEdge(Loop *l);
  HalfEdge(Edge *e,Vertex *v,Loop *wloop,HalfEdge *prv,HalfEdge *nxt);/*Initiate all members*/

  /*The other edge*/
  HalfEdge *mate();/*Find its mate*/

  /*Printers*/
  void print();/*Print start -> end*/

  /*Calculators*/
  Num* compDir();

  /*Intersections*/
  Num *intersection(HalfEdge *);

};


/*****************Edge with two half edges***********************/
class Edge
{
public:
  HalfEdge *he1;/*pointer to right halfedge*/
  HalfEdge *he2;/*pointer to left halfedge*/
  Solid *s;
  Dir   *d;

  HalfEdge *getHe1(){return he1;}
  void setHe1(HalfEdge *he){he1=he;}
  HalfEdge *getHe2(){return he2;}
  void setHe2(HalfEdge *he){he2=he;}
  Solid *getSolid(){return s;}
  void setSolid(Solid *solid){s=solid;}

  /*Constructors*/
  Edge();
  Edge(Solid *s);
  Edge(HalfEdge *he1,HalfEdge *he2,Solid *s);

  /*Destructors*/
  ~Edge();

  /*Printers*/

  void print(){ if(he1) he1->print(); else if(he2) he2->print(); else cout<<"empty edge without halfedges"<<endl;}; /*print the start end end point*/

  /*Calculators*/
  /*Compute the coordinates*/

  /*Intersections*/
  Num *intersection(Edge *e);
};


/******************Loop with leading Edge*************************/
class Loop
{
public:
  HalfEdge *ledg;/*Pointer to ring of half edges*/
  Face *lface;/*back to pointer to face*/

  Loop();/*Default constructor*/
  Loop(Face *f);/*Constructor with a face*/
  Loop(HalfEdge *he,Face *f);/*Constructor wigh face and leading edge*/

  /*Printers*/
  void print();/*print the loop points in a circle*/

  /*Length & size*/
  int size();
  int length();

  /*Calculators*/
  Num* compUpDir();
  Num* compDownDir();

  Num* compEq();

  /*Positions*/
  Pos position(Vertex* v);

  /*Intersections*/
  Loop intersection(Loop* l);
};

/*********************Loop and a inner loop vector********************/
class Face
{
public:
  static int counter;
  Id faceno;/*face identifier*/
  Solid *fsolid;/*back pointer to solid*/
  Loop *flout;/*pointer to outer loop*/
  Vec<Loop *> *floops;/*pointer to list of loops*/
  coords feq;/*face equation*/

  /*Constructors*/
  Face();
  Face(Solid* s);
  Face(Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d);
  Face(Id faceno,Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d);

  /*Destructors*/
  ~Face();

  /*Is simple*/
  bool isSimple(){
    return floops->size()<=1;
  }

  /*Add an inner Loop*/
  void addLoop(Loop *l){
    if (flout==NULL)
      flout=l;

    for (unsigned int i=0;i<floops->size();i++)
      if((*floops)[i]==l)
      return;

    floops->push_back(l);
  }

  /*Getters & Setters*/
  /*flout*/
  Loop* getFlout(){
    return flout;
  }

  void setFlout(Loop *flout){
    this->flout=flout;
  }

  /*Printers*/
  void print();

  /*Calculators*/
  Num* compUpDir(){
      return flout->compUpDir();
  }
  Num* compDownDir(){
      return flout->compDownDir();
  }
  Num* compEq(){
      return flout->compEq();
  }

  /*Interesections*/
  HalfEdge *intersection(Face *f);

};

/*******************Solid with vertices,edges and faces**********************/
class Solid
{
public:
  Id solidno;/*solid identifer*/
  Vec<Face *> *sfaces;/*pointer to list of faces*/
  Vec<Edge *> *sedges;/*pointer to list of edges*/
  Vec<Vertex *> *sverts;/*pointer to list of vertices*/
  Vec<Solid *> *solids;

  Solid();
  Solid(Vec<Solid *> *solids);
  Solid(vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids);
  Solid(Id solidno,vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids);

  /*Intersections*/
  Loop* intersection(Face *f);
  Loop* intersection(Loop *l);
  Solid* intersection(Solid *s);
  




  /*void showGL(int argc,char **argv){
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize(ww,wh);
   glutInitWindowPosition(150,150);
   glutCreateWindow("Line");

   init();

   glutDisplayFunc(Display);
   //glutMouseFunc(pressMouse);
   //glutMotionFunc(holdMouse);
   glutMainLoop();
    

  }*/

  /*The center of the solid*/
  Vec <double> *center(){
    double cx=0;
    double cy=0;
    double cz=0;
    for (unsigned int i=0;i<sverts->size();i++){
      Vertex *v=(*sverts)[i];
      cx+=v->getX();
      cy+=v->getY();
      cz+=v->getZ();
    }
    cx/=sverts->size();
    cy/=sverts->size();
    cz/=sverts->size();
    
    Vec<double> *vs=new Vec<double>(3,0);
    (*vs)[0]=cx;
    (*vs)[1]=cy;
    (*vs)[2]=cz;
    return vs;
  }//center

  /*print every loops*/
  void print();
  void showInfo(){
    cout<<"Solid Id="<<solidno<<endl;
    cout<<"sfaces: size="<<sfaces->size()<<endl;
    for (unsigned int i=0;i<sfaces->size();i++)
      cout<<"faceno="<<(*sfaces)[i]->faceno<<endl;
    cout<<endl;

    cout<<"sedges: size="<<sedges->size()<<endl;
    for (unsigned int i=0;i<sedges->size();i++){
      Edge *e=(*sedges)[i];
      e->he1->print();
      e->he2->print();
      cout<<endl;
    }
    cout<<endl;

    cout<<"sverts: size="<<sverts->size()<<endl;
    for (unsigned int i=0;i<sverts->size();i++){
      Vertex *v=(*sverts)[i];
      v->print();
    }

    cout<<endl;
  }

  /*Radius and Diameter*/
  double radius(){
    Vertex *v= (*sedges)[0]->he1->start;
    double x = v->getX();
    double y = v->getY();
    double z = v->getZ();

    Vec<double> *c=center();
    return sqrt((x-(*c)[0])*(x-(*c)[0])+(y-(*c)[1])*(y-(*c)[1])+(z-(*c)[2])*(z-(*c)[2]));
  }
  double diameter(){
      return radius()*2;
  }

};

//Pos position(Vertex* v, Edge* e);
//Pos position(Edge* e,Vertex* v);
Pos position(Vertex* v, Face* f);
Pos position(Face *f, Vertex* v);
Pos position(Vertex *v, Solid *s);
Pos position(Solid *s, Vertex* v);

/*Products*/
Num dotProduct(HalfEdge *he1,HalfEdge *he2);
Num dotProduct(Num* v1,Num* v2);
Num *crossProduct(HalfEdge *he1,HalfEdge *he2);
Num *crossProduct(Num *v1, Num *v2);


