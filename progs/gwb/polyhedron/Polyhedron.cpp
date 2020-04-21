/*
*Kai Cao Jun 2012
*/
#include "Constants.h"
int Vertex::counter=1;

/*Vertex constructors*/
Vertex::Vertex(Id no,double x, double y, double z, Solid *s){
  vertexno=no;
  vcoord=new vector<double>(4);
  (*vcoord)[0]=x;
  (*vcoord)[1]=y;
  (*vcoord)[2]=z;
  (*vcoord)[3]=1.0;
  vertexno=no;
  counter++;
  this->s=s;
  s->sverts->push_back(this);
}

Vertex::Vertex(double x,double y,double z,Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=x;
  (*vcoord)[1]=y;
  (*vcoord)[2]=z;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->push_back(this);
}


Vertex::Vertex(Solid* s){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  this->s=s;
  this->s->sverts->push_back(this);
}
/*Vertex default constructor*/
Vertex::Vertex(){
  vcoord=new vector<double>(4);
  (*vcoord)[0]=0;
  (*vcoord)[1]=0;
  (*vcoord)[2]=0;
  (*vcoord)[3]=1.0;
  vertexno=counter++;
  //s->sverts->push_back(this);
}

/*Destructors*/
Vertex::~Vertex(){
  vector<Vertex *>::iterator it=s->sverts->begin();
  for (;it!=s->sverts->end();it++){
    if((*it)->vertexno==this->vertexno){
      s->sverts->erase(it);
      break;
    }
  }
}
/*Print coordinates*/

void Vertex::print(){
  cout<<vertexno<<"("<<(*vcoord)[0]<<","<<(*vcoord)[1]<<","<<(*vcoord)[2]<<")"<<endl;
}

/******************************************/
/*HalfEdge constructors*/
HalfEdge::HalfEdge(){
  start=new Vertex();
  prvhe=this;
  nxthe=this;
}
HalfEdge::HalfEdge(Vertex* start){
  this->start=start;
  prvhe=this;
  nxthe=this;
} 
HalfEdge::HalfEdge(Loop *l){
  wloop=l;
}
HalfEdge::HalfEdge(Edge *e,Vertex *v,Loop *l,HalfEdge *prv,HalfEdge *nxt){
  edg=e;
  start=v;
  wloop=l;
  prvhe=prv;
  nxthe=nxt;  
}

/*To find the mate of a half edge*/
HalfEdge *HalfEdge::mate(){
  if(!this)
    return NULL;

  if(edg==NULL)
    return NULL;

  if(this==edg->he1)
    return edg->he2;
  else
    return edg->he1;
}


void HalfEdge::print(){
  if(start==NULL){
    cout<<"Wrong!No start point!"<<endl;
    return;
  }

  if (nxthe==NULL){
    cout<<"No next HalfEdge!"<<endl;
    nxthe=this;
    return;
  }

  cout<<(start->vertexno)<<"("<<start->getX()<<","<<start->getY()<<","<<start->getZ()<<")"
  <<" -> "<<(nxthe->getStart()->getId())<<"("<<nxthe->getStart()->getX()<<","<<nxthe->getStart()->getY()<<","<<nxthe->getStart()->getZ()<<")"<<endl;

}

/*Calculators*/
Num *HalfEdge::compDir(){
    HalfEdge* m=mate();
    Vertex* v=m->start;

    d[0]=v->getX()-start->getX();
    d[1]=v->getY()-start->getY();
    d[2]=v->getZ()-start->getZ();
    
    return d;

}

/*Intersections*/
Num *HalfEdge::intersection(HalfEdge *){
    Num *point=new Num[DIM];
    return point;
}

/********************Edge***************************/
/*Constructors*/
Edge::Edge(){
  he1=new HalfEdge();
  he2=new HalfEdge();
}
Edge::Edge(Solid *solid){
  s=solid;
  he1=new HalfEdge();
  he2=new HalfEdge();
  s->sedges->push_back(this);

}
Edge::Edge(HalfEdge *half1,HalfEdge *half2,Solid *solid){
  he1=half1;
  he2=half2;
  s=solid;
  s->sedges->push_back(this);
}
/*Destructors*/
Edge::~Edge(){
  vector<Edge *>::iterator it=s->sedges->begin();
  for (;it!=s->sedges->end();it++){
    if((*it)==this){
      s->sedges->erase(it);
      break;
    }
  }
}

/*********************Loop*********************/

Loop::Loop(){ledg=new HalfEdge;}/*Default constructor*/
Loop::Loop(Face *f){ledg=new HalfEdge;lface=f;lface->addLoop(this);}/*Constructor with a face*/
Loop::Loop(HalfEdge *he,Face *f){ledg=he;lface=f;lface->addLoop(this);}/*Constructor wigh face and leading edge*/
void Loop::print(){
  HalfEdge *he=ledg;
  if (he==NULL)
    cout<<"No Half Edge!"<<endl;
  else do{
    Vertex *v=he->getStart();
    cout<<v->getId()<<"("<<v->getX()<<","<<v->getY()<<","<<v->getZ()<<") -> ";
    he=he->nxthe;
  }while(he!=ledg);
  cout<<endl;

}
/*Loop Size*/
int Loop::size(){
    HalfEdge *he=ledg->nxthe;
    int si=1;
    while (he!=ledg){
        si++;
        he=he->nxthe;
    }

    return si;
}

int Loop::length(){
    return size();
}

/*Calculators of Loops*/
Num *Loop::compUpDir(){
    if (size()<3)
        return NULL;
    else {
        HalfEdge *he1=ledg->nxthe;
        HalfEdge *he2=ledg;

        Num* posNorm=crossProduct(he1,he2);

        return posNorm;
    }

}// compUpDir

Num *Loop::compDownDir(){

    Num* posNorm=compUpDir();
    if (posNorm==NULL)
        return NULL;
    for (int i=0;i<DIM;i++)
        posNorm[i]=-posNorm[i];
    return posNorm;
}// compDownDir

Num *Loop::compEq(){
    Num* posNorm=compUpDir();
    if (posNorm==NULL)
        return NULL;

    Num* eq=new Num[DIM+1];
    for (int i=0;i<DIM;i++)
        eq[i]=posNorm[i];

    Num sum=ZERO;
    Vertex *v = ledg->start;
    sum+= posNorm[0]*v->getX()+posNorm[1]*v->getY()+posNorm[2]*v->getZ();

    eq[DIM]=-sum;

    return eq;
}

Pos Loop::position(Vertex *v2){
    Num* posNorm=compUpDir();
    Vertex *v1=ledg->start;
    Num v[DIM];
    v[0]=v2->getX()-v1->getX();
    v[1]=v2->getY()-v1->getY();
    v[2]=v2->getZ()-v1->getZ();

    Num dot=dotProduct(posNorm,v);

    if (dot>0)
        return UP;
    if (dot<0)
        return DOWN;


        return ON;



}//position

/****************Face*************************/
/*Constructors of Face*/
Face::Face(){
  floops=new Vec<Loop *>();
  flout=new Loop(this);
  //fsolid->sfaces->push_back(this);

}

Face::Face(Solid* s){
  floops=new Vec<Loop *>();
  flout=new Loop();
  fsolid=s;
  fsolid->sfaces->push_back(this);

}

Face::Face(Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d){
  this->floops=floops;
  this->flout=flout;
  fsolid=s;
  fsolid->sfaces->push_back(this);
  feq[0]=a;
  feq[1]=b;
  feq[2]=c;
  feq[3]=d;
}


Face::Face(Id faceno,Vec<Loop *> *floops, Loop *flout,Solid * s, double a,double b,double c,double d){
  
  this->faceno=faceno;
  this->floops=floops;
  this->flout=flout;
  fsolid=s;
  fsolid->sfaces->push_back(this);
  feq[0]=a;
  feq[1]=b;
  feq[2]=c;
  feq[3]=d;
}

Face::~Face(){
  cout << "Now we are in the ~Face"<<endl;
  vector<Face *>::iterator it=fsolid->sfaces->begin();
  cout << "Now we have the iterator "<<endl;
  cout << "size ="<< fsolid->sfaces->size()<<endl;
  for (;it!=fsolid->sfaces->end();it++){
    cout<<"In the loop"<<endl;
    cout<<*it<<endl;
    if((*it)->faceno==this->faceno){
      fsolid->sfaces->erase(it);
      break;
    }
  }
  cout<<"Finished Deleting."<<endl;
}

void Face::print(){
  cout<<"Outer Loop:"<<endl;
  flout->print();
  cout<<"All Loops:"<<endl;
  for (unsigned int i=0;i<floops->size();i++){
    (*floops)[i]->print();
  }
  cout<<endl;
}

HalfEdge *Face::intersection(Face *f){
    Num* eq1=f->compEq();
    Num* eq2=f->compEq();

    Num* norm=crossProduct(eq1,eq2);

    Num x=ZERO;
    Num y=ZERO;
    Num z=ZERO;

    Num rootX=eq1[1]*eq2[2]-eq1[2]*eq2[1];
    Num rootY=eq1[0]*eq2[2]-eq1[2]*eq2[0];
    //Num rootZ=eq1[0]*eq2[1]-eq1[1]*eq2[0];


    if(rootX!=ZERO){
        Num headY=-(eq1[3]*eq2[2]-eq1[2]*eq2[3]);
        Num headZ=-(eq1[1]*eq2[3]-eq1[3]*eq2[1]);

        y=headY/rootX;
        z=headZ/rootX;
    }else  if(rootY!=ZERO){
        Num headX=-(eq1[3]*eq2[2]-eq1[2]*eq2[3]);
        Num headZ=-(eq1[0]*eq2[3]-eq1[3]*eq2[0]);

        x=headX/rootY;
        z=headZ/rootY;
    }else
        return NULL;

    HalfEdge *he = new HalfEdge;
    Vertex *v=new Vertex;
    v->setX(x);
    v->setY(y);
    v->setZ(z);
    he->start=v;
    he->d=norm;

    return he;


    /*if(rootZ!=ZERO){
        Num headX=-(eq1[3]*eq2[2]-eq1[2]*eq2[3]);
        Num headY=-(eq1[1]*eq2[3]-eq1[3]*eq2[1]);

        x=headX/rootZ;
        y=headY/rootZ;
    }*/



}

//HalfEdge *HalfEdge::mate(int a){return NULL;};
//int *HalfEdge::x(){return &1;}

/*Solid Constructors*/
Solid::Solid(){
  sfaces=new Vec<Face *>();/*pointer to list of faces*/
  sedges=new Vec<Edge *>();/*pointer to list of edges*/
  sverts=new Vec<Vertex *>();/*pointer to list of vertices*/

}
Solid::Solid(Vec<Solid *> *s){
  sfaces=new Vec<Face *>();/*pointer to list of faces*/
  sedges=new Vec<Edge *>();/*pointer to list of edges*/
  sverts=new Vec<Vertex *>();/*pointer to list of vertices*/
  solids=s;
  solids->push_back(this);
}
Solid::Solid(vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids){
  this->sfaces=sfaces;
  this->sedges=sedges;
  this->sverts=sverts;
  this->solids=solids;
  solids->push_back(this);
}

Solid::Solid(Id solidno,vector<Face *> *sfaces, vector<Edge *> *sedges,vector<Vertex *> *sverts,Vec<Solid *> *solids){
  this->solidno=solidno;
  this->sfaces=sfaces;
  this->sedges=sedges;
  this->sverts=sverts;
  this->solids=solids;
  solids->push_back(this);
}

/*printers*/
void Solid::print(){
  for (unsigned int i=0;i<sfaces->size();i++){
    cout<<"Face Id: "<<(*sfaces)[i]->faceno<<endl;
    (*sfaces)[i]->print();
  }
}

/*Intersections*/
Loop *Solid::intersection(Face *f){
    return intersection(f->flout);
}

Loop *Solid::intersection(Loop *l){
    return new Loop;

}

/****************************Global Functions*********************/
/*Products*/
Num dotProduct(HalfEdge *he1, HalfEdge *he2){
    Num* dir1=he1->compDir();
    Num* dir2=he2->compDir();

    return dotProduct(dir1,dir2);

}

Num dotProduct(Num *v1, Num *v2){
    Num result=ZERO;
    for (int i=0;i<DIM;i++){
        result+=v1[i]*v2[i];
    }
    return result;
}

Num* crossProduct(HalfEdge *he1, HalfEdge *he2){
    Num* dir1=he1->compDir();
    Num* dir2=he2->compDir();

    return crossProduct(dir1,dir2);

}

Num *crossProduct(Num *dir1, Num *dir2){
    Num* result=new Num[DIM];
    result[0]=dir1[1]*dir2[2]-dir1[2]*dir2[1];
    result[1]=-(dir1[0]*dir2[2]-dir1[2]*dir2[0]);
    result[2]=dir1[0]*dir2[1]-dir1[1]*dir2[0];

    return result;
}

/*Positions*/
Pos position(Face *f, Vertex *v){
    return f->flout->position(v);
}
Pos position(Vertex *v, Face *f){
    return f->flout->position(v);
}


