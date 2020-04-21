/*
 * file: Euler_Ops.cpp
 *
 *  This is an implementation of Geometric Work Bench from Mantyla's book.
 *
 * Author: Kai Cao
 * June 2012
 * Since Core 2.1.
 *
 ***************************************************/

#define OUTPUT_EULER_OPS 0 //JML: turn on-off the Euler operator screen print

#include <iostream>
#include "Euler_Ops.h"
using namespace std;

extern int interactive;                  // interactive or not:
                                    // <=0 means non-interactive, >0 means interactive.
extern string inputDir; 	    // Path for input files 
extern string fileName; 	    // Input file name (this is not used currently)
extern string model;	    // model can be "cylinder", "sphere", "trefoil", etc.
extern string mode;		    // mode can be "wire" or "face"
extern GLsizei windowWidth;	    // initial configuration box size
extern GLsizei windowHeight;
extern GLsizei windowPosX;           // initial Window position
extern GLsizei windowPosY;	

extern double startX;                    //start postion of mouse press 
extern double startY;
extern double startZ;

extern double base;                   //Initialize the origin rotation arguments
extern double theta;
extern double rad;

extern double rotX;                    //Initialize the rotation vector
extern double rotY;
extern double rotZ;

extern double centerX;                 //Initialize the center of the polyhedron
extern double centerY;
extern double centerZ;

extern double colorGap;               //This is for shading

extern double scalar;                  //Initialize the scalar of the polyhedron
extern double thetaX;
extern double thetaY;
extern double light[];

/* Chee: new parameters */
extern double radius;		  
extern double radius2;		 
extern double height;		
extern double height2;	
extern int nsegments;
extern int nsections;
extern int nsegments2;
extern int nsections2;
extern int nparts;
extern int nparts2;
extern double position2;
extern double position3;
extern double thickness3;
extern double height3;

/*Constructors*/
Euler_Ops::Euler_Ops(Vec<Solid *> *solids){
  this->solids=solids;
}

Euler_Ops::Euler_Ops(){
 solids=new Vec<Solid *>();
}


/*Add HalfEdge to a HalfEdge*/
HalfEdge *Euler_Ops::addhe(Edge *e, Vertex *v,HalfEdge *followhe,int sign)
{
 HalfEdge *he; 
 if(followhe->edg==NULL)
  he=followhe;
 else{
  he=new HalfEdge();
  /*Set previous he*/
  followhe->prvhe->nxthe=he;
  he->prvhe=followhe->prvhe;
  /*set next he*/
  he->nxthe=followhe;
  followhe->prvhe=he;
 }

 he->edg=e;
 he->start=v;
 he->wloop=followhe->wloop;
 if(sign==PLUS)
  e->he1=he;
 else
  e->he2=he;
 
 return he;
}//addhe

HalfEdge *Euler_Ops::delhe(HalfEdge *he)
{
  if (he->edg==NULL){
    delete he;
    return NULL;
  }
  else if (he->nxthe==he){
    he->edg=NULL;
    return(he);
  }
  else{
  /*Delete one direction*/
    he->prvhe->nxthe=he->nxthe;
    he->nxthe->prvhe=he->prvhe;

  /*Return the previous HalfEdge*/
    HalfEdge *rhe=he->prvhe;
    delete he;
    return (rhe);
  }
}

/*Higher level make vertex face solid*/
Solid *Euler_Ops::mvfs(Id s,Id f,Id v,double x,double y,double z)
{

#if OUTPUT_EULER_OPS
  cout<<"mvfs "<<s<<" "<<f<<" "<<v<<" "<<x<<" "<<y<<" "<<z<<endl;
#endif  

/*Initialize*/
  Solid *newsolid;
  Face *newface;
  Vertex *newvertex;
  HalfEdge *newhe;
  Loop *newloop;
  /*Initialize all objects*/
  newsolid=new Solid(solids);
  newface=new Face(newsolid);
  newloop=new Loop(newface);
  newhe=new HalfEdge(newloop);
  newvertex=new Vertex(newsolid);

  /*Assign all values*/
  newsolid->solidno=s;
  newface->faceno=f;
  newvertex->vertexno=v;
  newvertex->setX(x);
  newvertex->setY(y);
  newvertex->setZ(z);

  /*Assign all pointers*/
  /*Connection between vertices and halfedges*/
  newhe->start=newvertex;
  newvertex->vedge=newhe;
  /*Connection between halfedges and Loops*/
  newhe->wloop=newloop;
  newloop->ledg=newhe;
  newhe->nxthe=newhe->prvhe=newhe;
  /*Connection between loops and and faces*/
  newface->flout=newloop;

  /*We have a half edge but no edge*/  
  newhe->edg=NULL;

  /*newvertex->print();
  newhe->print();
  newloop->print();
  newface->print();
  newsolid->print();*/

  return(newsolid);
}//mvfs

void Euler_Ops::kvfs(Id s)
{
#if OUTPUT_EULER_OPS
  cout<<"kvfs "<<s<<endl;
#endif  
}//kvfs

void Euler_Ops::lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z)
{

  HalfEdge *he;
  Vertex *newvertex=new Vertex(v,x,y,z,he1->wloop->lface->fsolid);
  Edge *newedge=new Edge(he1->wloop->lface->fsolid);
 
  /*Seperate all points before you reach he2*/
  he=he1;
  while(he!=he2){
    cout<<"In the HalfEdge Loop"<<endl;
    he->start=newvertex;
    he=he->mate()->nxthe;
  }

  addhe(newedge,he2->start,he1,MINUS);
  addhe(newedge,newvertex,he2,PLUS);

  newvertex->vedge=he2->prvhe;
  he2->start->vedge=he2; 
  /*cout<<newedge->he1<<endl;
  cout<<newedge->he2<<endl;
  cout<<he1<<endl;
  cout<<he2<<endl;*/
}

Face *Euler_Ops::lmef(HalfEdge *he1, HalfEdge *he2,Id f){

  Face *newface=new Face(he1->wloop->lface->fsolid);
  Loop *newloop=new Loop(newface);
  Edge *newedge=new Edge(he1->wloop->lface->fsolid);
  newface->faceno=f;
  newface->flout=newloop;

  /*The HalfEdges in [he1,he2) is in the newloop*/
  HalfEdge *he=he1;

  while(he!=he2){
    he->wloop=newloop;
    he=he->nxthe;
  }

  HalfEdge *nhe1=addhe(newedge,he2->start,he1,MINUS);
  HalfEdge *nhe2=addhe(newedge,he1->start,he2,PLUS);

  nhe2->prvhe->nxthe=nhe1;
  nhe1->prvhe->nxthe=nhe2;

  HalfEdge *temp=nhe1->prvhe;
  nhe1->prvhe=nhe2->prvhe;
  nhe2->prvhe=temp;

  newloop->ledg=nhe1;
  he2->wloop->ledg=nhe2;

  /*cout<<"In the MEF"<<endl;
  newedge->print();
  cout<<endl;*/


  return(newface);
  
}

/*Kill Edge Face*/
void Euler_Ops::lkef(HalfEdge *he1, HalfEdge *he2){

  /*check whether share an edge*/
  if (he1->edg!=he2->edg){
    cout<<"lkef:Two HalfEdges does not share an edge."<<endl;
    return;
  }
  /*Check whether belongs to different faces*/
  if (he1->wloop->lface==he2->wloop->lface){
    cout<<"lkefTwo HalfEdges in the same Face!"<<endl;
    return;
  }

  /*Connect the half edges*/
  he1->nxthe->prvhe=he2->prvhe;
  he1->prvhe->nxthe=he2->nxthe;
  
  he2->nxthe->prvhe=he1->prvhe;
  he2->prvhe->nxthe=he1->nxthe;

  /*Repoint the lead pointers of loops*/
  he1->wloop->ledg=he1->nxthe;

  cout<<"In KEF"<<endl;

  cout<<"DELETE EDGE"<<endl;
  delete he1->edg;
  cout<<"DELETE the face"<<endl;
  cout<<he2<<endl;
  cout<<he2->wloop<<endl;
  cout<<he2->wloop->lface<<endl;
  delete he2->wloop->lface;
  cout<<"DELETE HE1"<<endl;
  delete he1;
  cout<<"DELETE HE2"<<endl;
  delete he2;
}//lkef

int Euler_Ops::kef(Id s,Id f,Id v1,Id v2)
{

#if OUTPUT_EULER_OPS
  cout<<"kef "<<s<<" "<<f<<" "<<v1<<" "<<v2<<endl;
#endif  

  
  Solid *oldsolid;
  Face *oldface;
  HalfEdge *he1,*he2;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"kef: solid "<<s<<"not found\n";
    return(ERROR);
  }

  /*Get face*/
  oldface=fface(oldsolid,f);
  if(oldface==NULL){
    cout<<"kef: face "<<f<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  /*Get He v1->v2*/
  he2=fhe(oldface,v1,v2);
  if(he2==NULL){
    cout<<"kef: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f<<endl;
    return(ERROR);
  }

  /**Get He v2->v1*/
  he1=he2->mate();
  if(he1==NULL){
    cout<<"kef: HalfEdge"<<v2<<"->"<<v1<<"not found"<<endl;
    return (ERROR);
  }

  lkef(he1,he2);
  return SUCCESS;
}
/*Get solid*/
Solid *Euler_Ops::getsolid(Id sn){
  /*cout<<"begin get solid"<<endl;
  cout<<"solids=NLLL?"<<(solids==NULL)<<endl;
  cout<<"solids size="<<(solids->size())<<endl;
  cout<<"solids before begin"<<solids<<endl;
  cout<<"it points to begin"<<endl;*/
  for (unsigned int i=0;i<solids->size();i++){
    Solid *s=(*solids)[i];
    if(s->solidno==sn)
      return s;
  }//for
  return NULL;
}

/*Get face*/
Face *Euler_Ops::fface(Solid *s,Id fn){
  Vec<Face *>::iterator it;
  for (it=s->sfaces->begin();it!=s->sfaces->end();it++){
    if((*it)->faceno==fn)
      return *it;
  }//for
  return NULL;
}

/*Get HalfEdge*/
HalfEdge *Euler_Ops::fhe(Face *f,Id vn1,Id vn2){  

  Vec<Loop *> *ls=f->floops;

  /*Look through all loops*/
  for (unsigned int i=0;i<ls->size();i++){
    HalfEdge *he=(*ls)[i]->ledg;

    /*Look through all half edges*/
    do{
      if(he->start->vertexno==vn1&&he->nxthe->start->vertexno==vn2)
        return he;
       he=he->nxthe;
    }while(he!=(*ls)[i]->ledg);
   }//for

   return NULL;
 }//fhe

 /*Get HalfEdge (simple)*/
HalfEdge *Euler_Ops::fhe(Face *f,Id vn){  

  Vec<Loop *> *ls=f->floops;

  /*Look through all loops*/
  for (unsigned int i=0;i<ls->size();i++){
    HalfEdge *he=(*ls)[i]->ledg;

    /*Look through all half edges*/
    do{
      if(he->start->vertexno==vn)
        return he;
       he=he->nxthe;
    }while(he!=(*ls)[i]->ledg);
   }//for

   return NULL;
 }//fhe
 

/*Higher level make edge vertex*/
/*From he1(v1,v2) in f1 to he2(v1,v3) in f2*/
int Euler_Ops::mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z)
{
#if OUTPUT_EULER_OPS
  cout<<"mev "<<s<<" "<<f1<<" "<<f2<<" "<<v1<<" "<<v2<<" "<<v3<<" "<<v4<<" "<<x<<" "<<y<<" "<<z<<endl;
#endif  


  Solid *oldsolid;
  Face *oldface1,*oldface2;
  HalfEdge *he1,*he2;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mev: solid "<<s<<"not found\n";
    return(ERROR);
  }

  /*Get face*/
  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mev: face "<<f1<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  oldface2=fface(oldsolid,f2);
  if(oldface2==NULL){
    cout<<"mev: face "<<f2<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  he1=fhe(oldface1,v1,v2);
  if(he1==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f1<<endl;
    return(ERROR);
  }

  he2=fhe(oldface2,v1,v3);
  if(he2==NULL){
    cout<<"mev: HalfEdge"<<v1<<"->"<<v3<<"not found in face"<<f2<<endl;
    return(ERROR);
  }

  /*cout<<"Solid "<<oldsolid->solidno<<endl;
  oldsolid->print();
  cout<<"oldface1 "<<oldface1->faceno<<endl;
  oldface1->print();
  cout<<"oldface2 "<<oldface2->faceno<<endl;
  oldface2->print();
  cout<<"he1 "<<endl;
  he1->print();
  cout<<"he2 "<<endl;
  he2->print();
  cout<<endl<<endl;*/

  lmev(he1,he2,v4,x,y,z);
  return SUCCESS;
}//mev

/*Simple mev*/
int Euler_Ops::mev(Id s,Id f1,Id v1,Id v4,double x,double y,double z)
{
#if OUTPUT_EULER_OPS
  cout<<"mev "<<s<<" "<<f1<<" "<<v1<<" "<<v4<<" "<<x<<" "<<y<<" "<<z<<endl;
#endif  


  Solid *oldsolid;
  Face *oldface;
  HalfEdge *he;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mev: solid "<<s<<"not found\n";
    return(ERROR);
  }

  /*Get face*/
  oldface=fface(oldsolid,f1);
  if(oldface==NULL){
    cout<<"mev: face "<<f1<<"not found in solid "<<s<<endl;
    return(ERROR);
  }


  he=fhe(oldface,v1);
  if(he==NULL){
    cout<<"mev: HalfEdge"<<v1<<"not found in face"<<f1<<endl;
    return(ERROR);
  }


  /*cout<<"Solid "<<oldsolid->solidno<<endl;
  oldsolid->print();
  cout<<"oldface1 "<<oldface1->faceno<<endl;
  oldface1->print();
  cout<<"oldface2 "<<oldface2->faceno<<endl;
  oldface2->print();
  cout<<"he1 "<<endl;
  he1->print();
  cout<<"he2 "<<endl;
  he2->print();
  cout<<endl<<endl;*/

  lmev(he,he,v4,x,y,z);
  return SUCCESS;
}//mev

int Euler_Ops::kev(Id s,Id f,Id v1,Id v2){
#if OUTPUT_EULER_OPS
  cout<<"kev "<<s<<" "<<f<<" "<<v1<<" "<<v2<<endl;
#endif  

  return SUCCESS;
}//kev

/*Higher level make edge face*/
/*from he1(v1,v2) in f1 to he2(v3,v4) in f2 ,he1 in the new face*/
int Euler_Ops::mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2){
#if OUTPUT_EULER_OPS
  cout<<"mef "<<s<<" "<<f1<<" "<<v1<<" "<<v2<<" "<<v3<<" "<<v4<<" "<<f2<<endl;
#endif  

                    
  Solid *oldsolid;
  Face *oldface1;
  HalfEdge *he1,*he2;

  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mef: solid "<<s<<"not found\n";
    return(ERROR);
  }

  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mef: face "<<f1<<"not found in solid "<<s;
    return(ERROR);
  }

  he1=fhe(oldface1,v1,v2);
  if(he1==NULL){
    cout<<"mef: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f1;
    return(ERROR);
  }

  he2=fhe(oldface1,v3,v4);
  if(he2==NULL){
    cout<<"mef: HalfEdge"<<v3<<"->"<<v4<<"not found in face"<<f1;
    return(ERROR);
  }

  lmef(he1,he2,f2);
  return SUCCESS;
}//mef

/*Simple Higher level make edge face*/
int Euler_Ops::mef(Id s,Id f1,Id v1,Id v3,Id f2)
{
#if OUTPUT_EULER_OPS
  cout<<"mef "<<s<<" "<<f1<<" "<<v1<<" "<<v3<<" "<<f2<<endl;
#endif  

                    
  Solid *oldsolid;
  Face *oldface1;
  HalfEdge *he1,*he2;

  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mef: solid "<<s<<"not found\n";
    return(ERROR);
  }

  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"mef: face "<<f1<<"not found in solid "<<s;
    return(ERROR);
  }

  he1=fhe(oldface1,v1);
  if(he1==NULL){
    cout<<"mef: HalfEdge"<<v1<<"->"<<"not found in face"<<f1;
    return(ERROR);
  }

  he2=fhe(oldface1,v3);
  if(he2==NULL){
    cout<<"mef: HalfEdge"<<v3<<"->"<<"not found in face"<<f1;
    return(ERROR);
  }

  lmef(he1,he2,f2);
  return SUCCESS;
}//mef

void Euler_Ops::lkemr(HalfEdge *h1,HalfEdge *h2)
{

  register HalfEdge *h3,*h4;
  Loop *nl;
  Loop *ol;
  Edge *killedge;
  /*Get out loop,which is the old loop*/
  ol=h1->wloop;

  /*New Loop, which is the inner loop*/
  nl=new Loop(ol->lface);

  /*h1 and h2 share the same edge*/
  killedge=h1->edg;

  h3=h1->nxthe;
  h1->nxthe=h2->nxthe;
  h2->nxthe->prvhe=h1;
  h2->nxthe=h3;
  h3->prvhe=h2;

  h4=h2;
  
  do{
    h4->wloop=nl;
    h4=h4->nxthe;
  }while(h4!=h2);

  ol->ledg=h3=delhe(h1);
  nl->ledg=h4=delhe(h2);
  
  /*If there is no edge, it should be a self loop*/
  h3->start->vedge=(h3->edg)? h3: NULL;
  h4->start->vedge=(h4->edg)? h4: NULL;
  
  /*We have to use destruction here*/
  delete killedge;
}//lkemr

int Euler_Ops::kemr(Id s,Id f,Id v1,Id v2)
{

#if OUTPUT_EULER_OPS
  cout<<"kemr "<<s<<" "<<f<<" "<<v1<<" "<<v2<<endl;
#endif  

  Solid *oldsolid;
  Face *oldface;
  HalfEdge *he1;
  HalfEdge *he2;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"kemr: solid "<<s<<"not found\n"<<endl;
    return(ERROR);
  }

  /*Get face*/
  oldface=fface(oldsolid,f);
  if(oldface==NULL){
    cout<<"kemr: face "<<f<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  /*Get HalfEdge he1 & HalfEdge he2*/
  he1=fhe(oldface,v1,v2);
  if(he1==NULL){
    cout<<"kemr: HalfEdge"<<v1<<"->"<<v2<<"not found in face"<<f<<endl;
    return(ERROR);
  }

  he2=fhe(oldface,v2,v1);
  if(he2==NULL){
    cout<<"kemr: HalfEdge"<<v2<<"->"<<v1<<"not found in face"<<f<<endl;
    return(ERROR);
  }

  lkemr(he1,he2);
  return (SUCCESS);

}//kemr

/*Opposite Operation : Make Edge kill Ring*/
/*Lower Level*/
void Euler_Ops::lmekr(HalfEdge *fromhe, HalfEdge *tohe)
{
  if(fromhe==NULL){
    cout<<"In lmekr: fromhe is NULL"<<endl;
    return;
  }

  if(tohe==NULL){
    cout<<"In lmekr: tohe is NULL"<<endl;
    return;
  }

  
  /*Get the necessary pointers*/
  HalfEdge *beforefrom=fromhe->prvhe;
  HalfEdge *beforeto=tohe->prvhe;
  Edge *newedge=new Edge(fromhe->wloop->lface->fsolid);
  /*The result half edges*/
  HalfEdge *fromto=new HalfEdge;
  HalfEdge *tofrom=new HalfEdge; 
 
  /*Add to from Edge*/
  beforeto->nxthe=tofrom;
  tofrom->prvhe=beforeto;

  fromhe->prvhe=tofrom;
  tofrom->nxthe=fromhe;

  /*Add from to edge*/
  beforefrom->nxthe=fromto;
  fromto->prvhe=beforefrom;
  
  tohe->prvhe=fromto;
  fromto->nxthe=tohe;

 
  /*Set edge*/
 fromto->edg=newedge;
 tofrom->edg=newedge;

 newedge->he1=fromto;
 newedge->he2=tofrom;

  /*Set vertex*/
 fromto->start=fromhe->start;
 tofrom->start=tohe->start;

  /*Set Loop*/
 fromto->wloop=fromhe->wloop;
 tofrom->wloop=fromhe->wloop;  
 
  /*Seperate all points before you reach he2*/

  fromhe->wloop->lface->floops->pop_back();
}//lmekr

int Euler_Ops::mekr(Id s,Id f,Id v1,Id v2,Id v3,Id v4)
{
#if OUTPUT_EULER_OPS
  cout<<"mekr "<<s<<" "<<f<<" "<<v1<<" "<<v2<<" "<<v3<<" "<<v4<<endl;
#endif 

	/*Get solid*/
  Solid* oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"mekr: solid "<<s<<" not found\n";
    return(ERROR);
  }

  /*Get face*/
  Face* oldface=fface(oldsolid,f);
  if(oldface==NULL){
    cout<<"mekr: face "<<f<<" not found in solid "<<s<<endl;
    return(ERROR);
  }

  /*Get He v1->v2*/
  HalfEdge* he1=fhe(oldface,v1,v2);
  if(he1==NULL){
    cout<<"mekr: HalfEdge"<<v1<<"->"<<v2<<" not found in face"<<f<<endl;
    return(ERROR);
  }

  /**Get He v2->v1*/
  HalfEdge* he2=fhe(oldface,v3,v4);
  if(he2==NULL){
    cout<<"mekr: HalfEdge"<<v3<<"->"<<v4<<" not found"<<endl;
    return (ERROR);
  }
	
	lmekr(he1,he2);

      return SUCCESS;
}


int Euler_Ops::mekr(Id s,Id f,Id v1,Id v3)
{
#if OUTPUT_EULER_OPS
  cout<<"mekr "<<s<<" "<<f<<" "<<v1<<" "<<v3<<endl;
#endif   
      return SUCCESS;
}

/*Kill Face Make Ring Hole
*Suppose Face2 is simple
*remove Face2 and add outer 
*Loop of Face2 into Face1
*as an inner Loop
*/
void Euler_Ops::lkfmrh(Face *fac1,Face *fac2)
{
  if (fac1==NULL){
    cout<<"lkfmrh: fac1 is NULL"<<endl;
    return;
  }

  if (fac2==NULL){
    cout<<"lkfmrh: fac2 is NULL"<<endl;
    return;
  }

  if(fac2->flout==NULL){
    cout<<"lkfmrh: fac2's outer loop is NULL"<<endl;
    return;
  }
  
  fac1->addLoop(fac2->flout);

  delete fac2;
}//lkfmrh

int Euler_Ops::kfmrh(Id s,Id f1,Id f2)
{
#if OUTPUT_EULER_OPS
  cout<<"kfmrh "<<s<<" "<<f1<<" "<<f2<<endl;
#endif 


  Solid *oldsolid;
  Face *oldface1;
  Face *oldface2;
  
  /*Get solid*/
  oldsolid=getsolid(s);
  if(oldsolid==NULL){
    cout<<"kfmrh: solid "<<s<<"not found\n";
    return(ERROR);
  }

  /*Get face1*/
  oldface1=fface(oldsolid,f1);
  if(oldface1==NULL){
    cout<<"kfmrh: face "<<f1<<"not found in solid "<<s<<endl;
    return(ERROR);
  }
  /*Get face2*/
  oldface2=fface(oldsolid,f2);
  if(oldface2==NULL){
    cout<<"kfmrh: face "<<f2<<"not found in solid "<<s<<endl;
    return(ERROR);
  }

  lkfmrh(oldface1,oldface2);
  return (SUCCESS);

}//kfmrh

/*Make face , kill ring & hole*/
void Euler_Ops::lmfkrh(Loop *l,Id f)
{
}

int Euler_Ops::mfkrh(Id s,Id f1,Id v1,Id v2,Id f2){
#if OUTPUT_EULER_OPS
  cout<<"mfkrh "<<s<<" "<<f1<<" "<<v1<<" "<<v2<<" "<<f2<<endl;
#endif 

  return SUCCESS;
}

/*Move a ring*/
void Euler_Ops::lringmv(Loop *l,Face *tofac,int inout)
{
}

int Euler_Ops::ringmv(Id s,Id f1,Id f2,Id v1,Id v2,Id inout)
{
#if OUTPUT_EULER_OPS
  cout<<"ringmv "<<s<<" "<<f1<<" "<<f2<<" "<<v1<<" "<<v2<<" "<<inout<<endl;
#endif 

  return SUCCESS;
}

