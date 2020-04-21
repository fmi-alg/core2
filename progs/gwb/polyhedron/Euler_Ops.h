#include <iostream>
#include "Constants.h"
using namespace std;
class Euler_Ops{
public:
  Vec<Solid *> *solids;
  /*Constructors*/
  Euler_Ops();
  Euler_Ops(Vec<Solid *> *solids);
  /**********Operations on HalfEdge************/
  HalfEdge *addhe(Edge *e, Vertex *v,HalfEdge *he,int sign);
  Edge *addEdge(Edge *e, HalfEdge *he1, HalfEdge *he2, int sign);
  HalfEdge *delhe(HalfEdge* he);

  /***************************************Vertex, Face Solid****************************************************/
  /**********Initialize a solid with a single vertex and a single face*****/
  Solid *mvfs(Id s,Id f,Id v,double x,double y,double z);
  /*mvfs for vector*/
  Solid *mvfs(Vec<double> *v){
      if (v->size()!=6){
            cout<<"wrong vector size  ( "<<v->size()<<" ) while calling mvfs!"<<endl;
            return NULL;
      }

      return mvfs((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(*v)[3],(*v)[4],(*v)[5]);
  }
  
  

  /********Kill a solid with a single face & a single vertex***************/
  void lkvfs(Solid *s);
  /********Kill a solid with a single face & a single vertex***************/
  void kvfs(Id s);  
  /*kvfs for vector*/
  void kvfs(Vec<double> *v){
      if (v->size()!=1){
            cout<<"wrong vector size  ( "<<v->size()<<" ) while calling kvfs!"<<endl;
            return;
      }

      kvfs((Id)((*v)[0]));
  }


  /*Lower level Operations*/
  /********************************Lower Level Edge & Vertex****************************************************/
  /*Lower level make edge vertex*/
  void lmev(HalfEdge *he1,HalfEdge *he2,Id v,double x,double y,double z);
  /*Lower level kill edge vertex*/
  void lkev(HalfEdge *he1,HalfEdge *he2);
  
  /********************************Lower Level Edge & Ring******************************************************/
  /*Make Edge Face*/
  Face *lmef(HalfEdge *he1, HalfEdge *he2, Id f);
  /*Kill Edge Face*/
  void lkef(HalfEdge *he1,HalfEdge *he2);

  /********************************Lower Level Edge & Ring******************************************************/
  /*Kill Edge make Ring*/
  void lkemr(HalfEdge *h1,HalfEdge *h2);
  /*Make Edge kill Ring*/
  void lmekr(HalfEdge *he1,HalfEdge *he2);

  /*********************************Lower level Face & Ring,Hole*************************************************/
  /*Kill Face make Ring Hole*/
  void lkfmrh(Face *fac1,Face *fac2);
  /*Make Face kill Ring Hole*/
  void lmfkrh(Loop *l,Id f);
  /*Move the loop from its parent face to another(*tofac)*/
  void lringmv(Loop *l,Face *tofac,int inout);

  /*************Higher Leve Opeartions**************/
  /*In higher level,we need some search for objects*/
  /*Get solid we want*/
  Solid *getsolid(Id sn);
  /*Get face*/
  Face *fface(Solid *s,Id fn);
  /*Get HalfEdge*/
  HalfEdge *fhe(Face *f,Id vn1,Id vn2);
  /*Simple get HalfEdge*/
  HalfEdge *fhe(Face *f,Id vn);

  /*****************************************************Actural higher level Euler Operators*********************************************************/
  /**************************Edge & Vertex***************************************************/
  /*Higher level make Edge Vertex*/
  int mev(Id s,Id f1,Id f2,Id v1,Id v2,Id v3,Id v4,double x,double y,double z);
  int mev(Id s,Id f1,Id v1,Id v4,double x,double y,double z);
  /*mev for vector*/
  int mev(Vec<double> *v){
      if (v->size()==10){
            return mev((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]),(Id)((*v)[5]),(Id)((*v)[6]),(*v)[7],(*v)[8],(*v)[9]);
      }
      if (v->size()==7){
            return mev((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(*v)[4],(*v)[5],(*v)[6]);
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling mev!"<<endl;
      return ERROR;
  }

  /*Higher level kill Edge Vertex*/
  int kev(Id s,Id f,Id v1,Id v2);
  /*kev for vector*/
  int kev(Vec<double> *v){
      if (v->size()!=4){
            cout<<"wrong vector size  ( "<<v->size()<<" ) while calling kev!"<<endl;
            return ERROR;
      }

      return kev((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]));
  }


  /**************************Edge & Face*****************************************************/
  /*Higher level make edge face*/
  int mef(Id s,Id f1,Id v1,Id v2,Id v3,Id v4,Id f2);
  int mef(Id s,Id f1,Id v1,Id v3,Id f2);
  /*mef for vector*/
  int mef(Vec<double> *v){
      if (v->size()==7){
            return mef((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]),(Id)((*v)[5]),(Id)((*v)[6]));
      }
      if (v->size()==5){
            return mef((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling mef!"<<endl;
      return ERROR;
  }


  /*Higher lever kill Edge Face*/
  int kef(Id s,Id f,Id v1,Id v2);
  /*kef for vector*/
  int kef(Vec<double> *v){
      if (v->size()==4){
            return kef((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling kef!"<<endl;
      return ERROR;
  }


  /***************************Edge & Ring*****************************************************/
  /*Higher level kill edge make ring*/
  int kemr(Id s, Id f, Id v1, Id v2);
  /*kemr for vector*/
  int kemr(Vec<double> *v){
      if (v->size()==4){
            return kemr((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling kemr!"<<endl;
      return ERROR;
  }


  /*Higher level make edge kill ring*/
  /*We build an edge from the start vertex of He(v1,v2) to He (v3,v4)*/
  int mekr(Id s,Id f,Id v1,Id v2,Id v3,Id v4);
  int mekr(Id s,Id f,Id v1,Id v3);
  /*mekr for vector*/
  int mekr(Vec<double> *v){
      if (v->size()==6){
            return mekr((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]),(Id)((*v)[5]));
      }
      if (v->size()==4){
            return mekr((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling mekr!"<<endl;
      return ERROR;
  }


  /**************************face & ring,hole****************************************/
  int kfmrh(Id s,Id f1,Id f2);

  /*kfmrh for vector*/
  int kfmrh(Vec<double> *v){
      if (v->size()==3){
            return kfmrh((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling kfmrh!"<<endl;
      return ERROR;
  }

  int mfkrh(Id s,Id f1,Id v1,Id v2,Id f2);

  /*mfkrh for vector*/
  int mfkrh(Vec<double> *v){
      if (v->size()==5){
            return mfkrh((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling mfkrh!"<<endl;
      return ERROR;
  }


  /**************************Move a ring from one face to another face****************/
  int ringmv(Id s,Id f1,Id f2,Id v1,Id v2,Id inout);

  /*ringmv for vector*/
  int ringmv(Vec<double> *v){
      if (v->size()==6){
            return ringmv((Id)((*v)[0]),(Id)((*v)[1]),(Id)((*v)[2]),(Id)((*v)[3]),(Id)((*v)[4]),(Id)((*v)[5]));
      }

      cout<<"wrong vector size ( "<<v->size()<<" ) while calling ringmv!"<<endl;
      return ERROR;
  }

/*primitives*/
Solid* prim(string name);
Solid* prim(string name,int solidNum);

/*x y and z*/
double modelX(double t){
	return modelX("trefoil",t);
}
double modelX(string type,double t){
	if (type.compare("trefoil")==0)	{
		return sin(t)+2*sin(2*t);
	}
	return 1.0;
}//modelX
double dirX(double t){
	return dirX("trefoil",t);
}
double dirX(string type,double t){
	if (type.compare("trefoil")==0){
		return cos(t)+4*cos(2*t);
	}
	return 1.0;
}//dirX
double modelY(double t){
	return modelY("trefoil",t);
}
double modelY(string type,double t){
	if (type.compare("trefoil")==0){
		double x=cos(t)-2*cos(2*t);
		//cout << x<<endl;
		return x;
	}
	return 1.0;
}//modelY
double dirY(double t){
	return dirY("trefoil",t);
}
double dirY(string type,double t){
	if (type.compare("trefoil")==0){
		return -sin(t)+4*sin(2*t);
	}
	return 1.0;
}//dirX
double modelZ(double t){
	return modelZ("trefoil",t);
}
double modelZ(string type,double t){
	if (type.compare("trefoil")==0){
		return -sin(3*t);
	}
	return 1.0;
}//modelZ
double dirZ(double t){
	return dirZ("trefoil", t);
}
double dirZ(string type,double t){
	if (type.compare("trefoil")==0){
		return -3*cos(3*t);
	}
	return 1.0;
}//modelZ

double* rotate(double* dir,double* v, double theta,double* result){
	//cout<<"good before norm"<<endl;
	double normDir[3];
	norm(dir,normDir);
	/*cout<<"good after norm"<<endl;
	cout<<dir[0]<<" "<<normDir[0]<<endl;
	cout<<dir[1]<<" "<<normDir[1]<<endl;
	cout<<dir[2]<<" "<<normDir[2]<<endl;*/
	
	double x=normDir[0];
	double y=normDir[1];
	double z=normDir[2];
	//cout<<"good after xyz"<<endl;
	
	
	double mat[]=	{x*x*(1-cos(theta))+cos(theta),x*y*(1-cos(theta))-z*sin(theta),x*z*(1-cos(theta))+y*sin(theta),
			 x*y*(1-cos(theta))+z*sin(theta),y*y*(1-cos(theta))+cos(theta),y*z*(1-cos(theta))-x*sin(theta),
			 x*z*(1-cos(theta))-y*sin(theta),y*z*(1-cos(theta))+x*sin(theta),z*z*(1-cos(theta))+cos(theta)};
	/*cout<<"theta="<<theta<<endl;
	for (int i=0;i<9;i++)
		cout<<mat[i]<<endl;
	cout<<endl;*/
	return matrixLeftMult(mat,v,result);
	
	
}
double* norm(double* v,double* a,double len2){
	double len=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	//cout<<"len="<<len<<endl;
	//double* a=new double[3];
	a[0]=v[0]/len*len2;
	a[1]=v[1]/len*len2;
	a[2]=v[2]/len*len2;
	return a;
}
double* norm(double* v,double* a){
	double len=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	//cout<<"len="<<len<<endl;
	//double* a=new double[3];
	a[0]=v[0]/len;
	a[1]=v[1]/len;
	a[2]=v[2]/len;
	return a;
}
double* matrixLeftMult(double* mat, double* v,double* result){
	return matrixLeftMult(mat,v,3,3,result);
}
double* matrixLeftMult(double* mat, double* v,int row,int col,double* result){
	//cout<<"good vefore new "<<endl;
	//double* result=new double[3];
	//cout<<"good after new"<<endl;
	for (int i=0;i<row;i++){
		double num=0;
		for (int j=0;j<col;j++)
			num+=mat[i*col+j]*v[j];
		//cout<<"("<<i<<")"<<"="<<num<<endl;
		result[i]=num;
	
	}//for rows
	return result;
}//

};
