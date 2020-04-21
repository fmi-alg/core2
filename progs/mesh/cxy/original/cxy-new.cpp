/* **************************************
   File: cxy.cpp

   Description: 
	Implementation of the CXY algorithm
	  for approximation curves represented by a bi-variate polynomial.
	The input polynomial is given as a string in variables x and y,
		with very flexible format (see below).
		Only integer coefficients supported.
	The curve is displayed using OpenGL/glut library.
   Usage:

        > ./CXY "curveDef" [Xmin] [Ymin] [Xmax] [Ymax] [max-size] [min-size] [offset] [drawbox]

		Note that [...] indicates optional arguments.
		The default values are:
              		The Default Curve    is   [x^2 - x^4 - y^2 +0.01]
	                The Default box      is   [-1.5,-1.5,1.5,1.5]
			The Default max-size is   100000
			The Default min-size is   0.0001
			The Default offset   is   0
			The Default drawbox  is   true
		You can omit any suffice of these seven arguments.

		EXPLANATION:
		-- max-size = m  means that ANY candidate box
				of size > m will be split
			(Thus, m is the resolution of the curve)
		-- min-size = m  means that NO candidate box
				of size < m will be split
			(This prevents an infinite loop at singularies)

	E.g.,
	> ./CXY				-- plot default curve
	> ./CXY "y - x^2" 	        -- plots a parabola 

	> ./CXY "y^2 = x^3 - 3 xy" 	-- plots a cubic 
				(Note we can use equality sign in the equation)

	> ./CXY "x^3y + x(y-2x) = 1" -1 -1 3 3
				-- plots the curve in the box [-1,-1,3,3]

	> ./CXY "y - x^2" -10 -10 10 10 0.1 0.07
				-- The last two arguments tells us that
				the plot resolution is 0.1, and
				the offset is 0.02.  This means we are
				plotting the curve "y - x^2 = 0.07"

       Only the first argument is mandatory:
                    <curveDef> is a string defining a bivariate
		    polynomial in the variables 'x' and 'y'  (case sensitive)


   Author:  Shuxing Lu (based on Java Code of Long Lin)
	    Jihun and Chee (improvements)
   Date:    Apr 10, 2009

   To Do, Known Bugs, etc:
   ======================
	--Chee (Jan'2010):
		WARNING: this is currently slow because it assumes Level 3
		Bring back "reduce to dyadic/rational" to speed up Level 3
		Should implement the Level 2 version
	--BigFloat2 should be used for intervals
	--Should allow interactive display of curve, with zooming	
	--Should allow simultaneous display of several curves (e.g., f, f_x, f_y).

   Since Core Library  Version 2.0
   $Id: cxy-new.cpp,v 1.1 2010/05/18 14:36:01 exact Exp $
 ************************************** */
#ifndef CORE_LEVEL
//#  define CORE_LEVEL 2
#  define CORE_LEVEL 3
#endif
#include <GL/glut.h>
#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h
#include "CXY.h"

#include <math.h>

using namespace std;

//typedef BigInt	NT;
typedef BigFloat	NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;
Curve<NT> Bipolyno("x^2");
Curve<NT> Differenticate_X;
Curve<NT> Differenticate_Y;
vector<box*> Pic;
vector<box*> Pic_Red_Box;
vector<double>Line_Red;
machine_double Size_of_Graphic = 0.5;
machine_double Movement_X = 0;
machine_double Movement_Y = 0;
double  Max_Size = 100000;
double  Min_Size = 0.0001;
bool draw_Box = true;
// =========================================================
// Class Point Implementation
// =========================================================
Point::Point(){
		x=0;
		y=0;
	}
	
	Point::Point(double a, double b){
		x=a;
		y=b;
	}
	
	 void Point::copy(Point p){
		x=p.x;
		y=p.y;
	}
	
	double* Point::getPoint(){
		double * pointValue = new double[2];
		pointValue[0] = x;
        pointValue[1] = y;
		return pointValue;
	}
	
	double Point::getX(){
		return x;
	}
	
	double Point::getY(){
		return y;
	}
	
	void Point::setX(double ValueX){
		x=ValueX;
	}
	
	void Point::setY(double ValueY){
		y=ValueY;
	}
	
	int Point::sign(){
		double value;
		ImplicitCurve tmp;
		value = tmp.value(x, y);
		value = Bipolyno.yExprPolynomial(x).eval(y);
		if (value<0)
			return -1;
		if (value>0)
			return 1;
		else
			return 1;
	}

	
	void Point::print(){
		cout<<x<<" "<<y<<endl;
	}
	
	//return true if this point contains in the edge (a,b).
	bool Point::contain(Point a, Point b){
		bool e=false;
		if ((x==a.getX())&&(x==b.getX())){
			if (((a.getY()<y)&&(y<b.getY()))||((a.getY()>y)&&(y>b.getY())))
				e=true;
		}
		if ((y==a.getY())&&(y==b.getY())){
			if (((a.getX()<x)&&(x<b.getX()))||((a.getX()>x)&&(x>b.getX())))
				e=true;
		}
		return e;
	}	

// =========================================================
// ImplicitCurve Class(Not Sure whether to keep it or not)
// =========================================================
double ImplicitCurve::value(double x, double y){
    	return (x*x-x*x*x*x-y*y+0.01);
	}

Interval ImplicitCurve::value(Interval &s, Interval &t){
	Interval *I=new Interval();
	Interval temp;
	Interval temp1;
	Interval temp2(100,100);
	Interval temp3(0.01,0.01);

	temp.multiply(s, s);//x*x

	temp1.multiply(temp, temp);

	I->minus(temp,temp1);
	
	temp.copy(*I);
	temp1.copy(I->multiply(t, t));
	I->minus(temp, temp1);
	
	temp.copy(*I);
	I->plus(temp, temp3);
	return *I;
}

double* ImplicitCurve::dValue(double x, double y){
	double* dv = new double[2];
	dv[0]=2*x-4*x*x*x;
	dv[1]=-2*y;
	return dv;

}

Interval* ImplicitCurve:: dValue(Interval &s, Interval &t){
	Interval* dv= new Interval[2];
	Interval  I;
	Interval  temp;
	Interval  temp1;
    Interval  temp2(4,4);
	Interval  temp3(2,2);
	Interval  temp4(-2,-2);

	temp.copy(I.multiply(temp2, s));
	temp1.copy(I.multiply(s, s));
	I.multiply(temp, temp1);
	temp1.copy(I);
	temp.copy(I.multiply(temp3, s));
	I.minus(temp, temp1);
	temp.copy(I);
		
	dv[0]=temp;
	dv[1]=I.multiply(temp4, t);
	return dv;
}

double min(double x, double y){
	  return   x<y?x:y;   
}
double max(double x, double y){
	 return   x>y?x:y;  
}
// =========================================================
// Interval Class 
// Todo: use BigFloat2 of Core Lib Instead
// =========================================================
Interval::Interval(){
	a=0;
	b=0;
}

Interval::Interval(double x, double y){
	a=x;
	b=y;
}

void Interval::copy(Interval I){
	a=I.a;
	b=I.b;
}

double* Interval::getInterval(){
	double * ArrayInterval  = new double[2];
	ArrayInterval[0] = a;
	ArrayInterval[1] = b;
	return ArrayInterval;
}

double Interval::getA(){
	return a;
}

double Interval::getB(){
	return b;
}

double Interval::getM(){
	return (a+b)/2;
}
Interval Interval::getL(){
	return Interval(a,getM());
}
Interval Interval::getR(){
	return Interval(getM(),b);
}
	
void Interval::setA(double x){
	a=x;
}
void Interval::setB(double y){
	b=y;
}
void Interval::setMinus(){
	a = -a;
	b = -b;
}
bool Interval::zero(){
	return (a<=0)&&(b>=0);
}

Interval  Interval::multiply(Interval &ss, Interval &tt){
	a=min(min(ss.a*tt.a, ss.a*tt.b),min(ss.b*tt.a, ss.b*tt.b));
	b=max(max(ss.a*tt.a, ss.a*tt.b),max(ss.b*tt.a, ss.b*tt.b));
	return *this;
}

Interval  Interval::plus(Interval &s, Interval &t){
	a=s.a+t.a;
	b=s.b+t.b;
	return *this;
}

Interval  Interval::minus(Interval &s, Interval &t){
	a=s.a-t.b;
	b=s.b-t.a;
	return *this;
}

Interval  Interval::divide(Interval &s, Interval &t){
	if (t.zero())
		cout<<"Error: Enominator includes zero."<<endl;
	else{
		a=min(min(s.a/t.a, s.a/t.b),min(s.b/t.a, s.b/t.b));
		b=max(max(s.a/t.a, s.a/t.b),max(s.b/t.a, s.b/t.b));
	}
	return *this;
}

void Interval::print(){
	cout<<a<<" "<<b<<endl;
}

bool Interval::overlap(Interval s, Interval t){
	return (!((s.b<=t.a)||(s.a>=t.b)));
}

double Interval::length(){
	return b-a;
}
// =========================================================
// Overload eval function from curve.h 
// 
// =========================================================
Interval eval(BiPoly<NT> Bi,Interval x, Interval y );
Interval evalpoly(Polynomial<NT> po, Interval x);

Interval eval(BiPoly<NT> Bi,Interval x, Interval y )  {
    //Evaluate the polynomial at (x,y);	
    int deg_y = Bi.ydeg;
    if ( deg_y == -1 ) return Interval(0,0);
	Interval res  = evalpoly(Bi.coeffX[deg_y],x);
	

    for(int i = deg_y - 1; i >= 0 ; --i){
	  Interval tmp;
	  Interval tmp1;
	  tmp1 = evalpoly(Bi.coeffX[i],x);
	  tmp.multiply (res ,y);
	  res.plus(tmp,tmp1);
    }
    return res;
  }
// =========================================================
// Rewrite the eval function in poly.h
// =========================================================
Interval evalpoly(Polynomial<NT> po, Interval x){
	int deg = po.degree();
  if (deg == -1)
    return Interval(0,0);
  if (deg == 0)
    return Interval(po.coeff()[0],po.coeff()[0]);

 Interval val(po.coeff()[deg],po.coeff()[deg]);
  for (int i = deg-1; i>=0; i--) {
	  Interval tmp;
		Interval tmp1(po.coeff()[i],po.coeff()[i]);
		Interval tmp2(-po.coeff()[i],-po.coeff()[i]);
		tmp.multiply( val,x);
		val.plus(tmp, tmp1);
  }
  return val;
}
// =========================================================
// box Class 
// Todo: Make it as the public include file for further use
// =========================================================
box::box(){	
//	Interval x;
//	Interval y;
	vector<Point> p;
	size=0;		
	parent=NULL;		
	vector<box> children;		
	vector<box> neighbor;
	vector<Point> vertices ;
	v1=0;
	v2=0;
	v3=0;
	v4=0;
	mark=false;
}
	
box::box(Point a, Point b){
//		vector<Point> p;
		v1=a.getX();
		v2=b.getX();
		v3=a.getY();
		v4=b.getY();
		Point tmp1 (a.getX(), b.getY());
		p.push_back(tmp1);
		Point tmp2 (a.getX(), a.getY());
		p.push_back(tmp2);
		Point tmp3 (b.getX(), a.getY());
		p.push_back(tmp3);
		Point tmp4 (b.getX(), b.getY());
		p.push_back(tmp4);		
//		Interval x(a.getX(), b.getX());
//		Interval y(a.getY(), b.getY());	
		x.setA(a.getX());
		x.setB(b.getX());
		y.setA(a.getY());
		y.setB(b.getY());
//		size=abs(b.getX()-a.getX());		
		parent=NULL;
		mark=false;
//		vector<box> children;
//		vector<box> neighbor;
//		vector<Point> vertices;
		
		//ambiguous=false;
		//suspicious=false;
	}
bool box::operator ==(box & ref){
	if(this->v1==ref.v1&&this->v2==ref.v2&&this->v3==ref.v3&&this->v4==ref.v4)
		return true;
	else
		return false;
	
}

bool box::getMark(){
		return mark;
}
	
vector<box *> box::split(){
		vector<box*> n = this->neighbor;
		box *c1 = new box(this->m(p.at(0), p.at(1)), this->m(p.at(0), p.at(3)));
		c1->parent=this;
		box *c2 = new box(p.at(1), this->m(p.at(1), p.at(3)));
		c2->parent=this;
		box *c3 = new box(this->m(p.at(1), p.at(2)), this->m(p.at(2), p.at(3)));
		c3->parent=this;
		box *c4 = new box(this->m(p.at(1), p.at(3)), p.at(3));
		c4->parent=this;
/*		
		c1.neighbor.insert(c1.neighbor.begin(),c2);
		c1.neighbor.insert(c1.neighbor.begin(),c4);
		c2.neighbor.insert(c2.neighbor.begin(),c1);
		c2.neighbor.insert(c2.neighbor.begin(),c3);
		c3.neighbor.insert(c3.neighbor.begin(),c2);
		c3.neighbor.insert(c3.neighbor.begin(),c4);
		c4.neighbor.insert(c4.neighbor.begin(),c3);
		c4.neighbor.insert(c4.neighbor.begin(),c1);
*/
		c1->neighbor.push_back(c2);
		c1->neighbor.push_back(c4);
		c2->neighbor.push_back(c1);
		c2->neighbor.push_back(c3);
		c3->neighbor.push_back(c2);
		c3->neighbor.push_back(c4);
		c4->neighbor.push_back(c3);
		c4->neighbor.push_back(c1);
		for (unsigned int i=0; i<n.size(); i++){
//			box *nt=n.at(i);
			box *nt=this->neighbor.at(i);
			RemoveFirst(nt->neighbor, this);
//			nt.neighbor.erase(this);
			vector<Point> pt=this->neighbor.at(i)->getPoint();
						
			//if (nt.getPoint().get(0).getX()==-0.05&&nt.getPoint().get(0).getY()==0.1){
			//	this.print();
				//System.out.println(b.getVertices().size());
			//}
			//neighbors on the up side
		if (pt.at(1).getY()==this->getPoint().at(0).getY()){
				if (nt->getX().overlap(nt->getX(), c1->getX())){
					c1->neighbor.push_back(nt);
					nt->neighbor.push_back(c1);
				}
				if (nt->getX().overlap(nt->getX(), c4->getX())){
					c4->neighbor.push_back(nt);
					nt->neighbor.push_back(c4);
				}
			}

			//neighbors on the left side
			if (pt.at(2).getX()==this->getPoint().at(1).getX()){
				if (nt->getY().overlap(nt->getY(), c1->getY())){
					c1->neighbor.push_back(nt);
					nt->neighbor.push_back(c1);
				}
				if (nt->getY().overlap(nt->getY(), c2->getY())){
					c2->neighbor.push_back(nt);
					nt->neighbor.push_back(c2);
				}
			}
			//neighbors on the bottom side
			if (pt.at(3).getY()==this->getPoint().at(2).getY()){
				if (nt->getX().overlap(nt->getX(), c2->getX())){
					c2->neighbor.push_back(nt);
					nt->neighbor.push_back(c2);
				}
				if (nt->getX().overlap(nt->getX(), c3->getX())){
					c3->neighbor.push_back(nt);
					nt->neighbor.push_back(c3);
				}
			}
			//neighbors on the right side
			if (pt.at(0).getX()==this->getPoint().at(3).getX()){
				if (nt->getY().overlap(nt->getY(), c3->getY())){
					c3->neighbor.push_back(nt);
					nt->neighbor.push_back(c3);
				}
				if (nt->getY().overlap(nt->getY(), c4->getY())){
					c4->neighbor.push_back(nt);
					nt->neighbor.push_back(c4);
				}
			}
		}
		children.push_back(c1);
		children.push_back(c2);
		children.push_back(c3);
		children.push_back(c4);
//		this->neighbor=n;
		//children.add(new box(this.m(tl, bl), this.m(tl, tr)));
		//children.add(new box(bl, this.m(bl, tr)));
		//children.add(new box(this.m(bl, br), this.m(br, tr)));
		return children;
	}
	
	Point box::m(Point a, Point b){
//		Point *newPoint = new Point((a.getX()+b.getX())/2, (a.getY()+b.getY())/2);
//		return *newPoint;
		Point newPoint((a.getX()+b.getX())/2, (a.getY()+b.getY())/2);
		return newPoint;
	}

	void box::RemoveFirst(vector<box *>&vec, box *b){
		vector<box *>::iterator it;
		for ( it=vec.begin() ; it < vec.end(); it++ )
		{
			if (((*it)->v1== b->v1)&&((*it)->v2== b->v2)&&((*it)->v3==b->v3)&&((*it)->v4== b->v4))
			{
				vec.erase(it);
				break;
			}
		}
	}
	
	Interval box::getX(){
		return x;
	}
	
	Interval box::getY(){
		return y;
	}
	
	vector<Point> box::getPoint(){
		return p;
	}
	
	vector<box *> box::getChildren(){
		return children;
	}
	
	box * box::getParent(){
		return parent;
	}
	
	vector<box *> box::getNeighbor(){
		return neighbor;
	}
	
	vector<Point *> box::getVertices(){
		return vertices;
	}
	
	double box::getSize(){
		double tmp = v2-v1;
		if (tmp>= 0)
			return tmp;
		else
			return -tmp;
//		return size;
	}
	
	//box is suspicious if its four corners have the same sign.
	bool box::suspicious(){
		return (p.at(0).sign()==p.at(1).sign() && p.at(1).sign()==p.at(2).sign() && p.at(2).sign()==p.at(3).sign());
	}
	
	//box is ambiguous if its four corners have the same sign, plus it has 2 vertices.
	bool box::ambiguous(){
		if (!this->suspicious())
			return false;
		else{ 
			if (this->vertices.size()==2)
				return true;
			else
				return false;
		}
	}
	
	//c0 if for box with interval x & y, 0 not \in f(x,y).
	//c0 if for box with interval x & y, 0 not \in f(x,y).
	bool box::c0(){
		//new ImplicitCurve().value(x, y).print();
		ImplicitCurve tmp;
		return (!eval(Bipolyno,x,y).zero());
	}
	
	//c1 if for box with interval x & y, 0 not \in <f'(x,y), f'(x,y)>.
	bool box::c1(){
		//return (ImplicitCurve().dValue(x, y)[0]);
		ImplicitCurve ic;
		Interval I;
		Interval temp;
		Interval temp1;
		Interval temp2 = eval(Differenticate_X,x,y);
		Interval temp3 = eval(Differenticate_Y,x,y);
		temp.multiply(temp2,temp2);
		temp1.multiply(temp3,temp3);
		I.plus(temp, temp1);
		//I.print();
		
		return (!I.zero());
	}
	
	//cxy if for box with interval x & y, 0 not \in fx(x,y) or fy(x,y).
	bool box::cxy(){
		ImplicitCurve ic;
		Interval temp2 = eval(Differenticate_X,x,y);
		Interval temp3 = eval(Differenticate_Y,x,y);
		return ((!temp2.zero())||(!temp3.zero()));
//		return ((!ic.dValue(x, y)[0].zero())||(!ic.dValue(x, y)[1].zero()));
	}
	
	void box::print(){
		for (unsigned int i=0; i<p.size(); i++)
			p.at(i).print();
	}
	
	void box::addV(){
		for (unsigned int i=0; i<p.size(); i++){
			if (p.at(i).sign()!=p.at((i+1)%4).sign()){
				if (!existV(p.at(i), p.at((i+1)%4))){
					Point *v=new Point(m(p.at(i),p.at((i+1)%4)));
					vertices.push_back(v);	
					for (unsigned int j=0; j<this->neighbor.size(); j++){
						box  *b=this->neighbor.at(j);
						if (b->containP(*v)&&(!b->existV(*v))){
							b->vertices.push_back(v);
						}
						//if (b.getPoint().get(1).getY()==0.5&&b.getPoint().get(1).getX()==-1){
						//	this.print();
						//	System.out.println(b.getNeighbor().size());
						//}
					}
				}
			}
		}
		
	}
	
	//return true if the edge (a,b) already exists vertices.
	bool box::existV(Point a, Point b){
		bool e=false;
		for (unsigned int i=0; i<this->vertices.size(); i++){
			Point *v=this->vertices.at(i);
			if ((((v->getX()==a.getX())&&(v->getX()==b.getX())))||(((v->getY()==a.getY())&&(v->getY()==b.getY()))))
				e=true;
		}
		return e;
	}
	
	//return true if this box contains point a.
	bool box::containP(Point a){
		bool e=false;
		for (unsigned int i=0; i<p.size(); i++){
			if (a.contain(p.at(i), p.at((i+1)%4)))
				e=true;
		}
		return e;
	}
	
	bool box::existV(Point a){
		bool e=false;
		for (unsigned int i=0; i<this->getVertices().size(); i++){
			Point *v=this->getVertices().at(i);
			if (v->getX()==a.getX() && v->getY()==a.getY())
				e=true;
		}
		return e;
	}
// =========================================================
// mainFunc Class 
// The main function for the algorithm
// =========================================================

mainFunc::mainFunc(){
	cBox=1;	
	w=0, h=0;
	scaler=1.0;
	moveX=0;
	moveY=0;
	counter=0;
	ccc=0;
	N=4;
//	Point point_Temp1(-2,-2);
//	Point point_Temp2(2,2);
	Point point_Temp1(-2,-2);
	Point point_Temp2(2,2);
	B0 = new box(point_Temp1, point_Temp2);
}
mainFunc::mainFunc(double Point_1_x,double Point_1_y,double Point_2_x,double Point_2_y){
	cBox=1;	
	w=0, h=0;
	scaler=1.0;
	moveX=0;
	moveY=0;
	counter=0;
	ccc=0;
	N=4;
//	Point point_Temp1(-2,-2);
//	Point point_Temp2(2,2);
	Point point_Temp1(Point_1_x,Point_1_y);
	Point point_Temp2(Point_2_x,Point_2_y);
	B0 = new box(point_Temp1, point_Temp2);
}

double mainFunc::distance(Point *a, Point *b){
	return sqrt((b->getX()-a->getX())*(b->getX()-a->getX())+(b->getY()-a->getY())*(b->getY()-a->getY()));
}

void mainFunc::erase(vector<box*> &vec, box *b)
{
	vector<box*>::iterator it;
	for ( it=vec.begin() ; it < vec.end(); it++ )
	{
		if (((*it)->v1== b->v1)&&((*it)->v2== b->v2)&&((*it)->v3== b->v3)&&((*it)->v4== b->v4)){
			vec.erase(it);
			break;
		}
	}
}

void mainFunc::priorityQueue(vector<box *> &vec, box *b){
	if (vec.size() == 0){
		vec.push_back(b);
	}
	else{
		vector<box *>::iterator it;
		for ( it=vec.begin() ; it <vec.end(); it++ )
		{
			if ( b->getSize()>(*it)->getSize()){
				vec.insert(it,b);
				break;
			}
		}
		if (b->getSize()<=(*(vec.end()-1))->getSize()){
			vec.push_back(b);
		}
	}
}

vector<box *> mainFunc::balance (vector<box *> &Q){
		vector<box*> QR;
		while (!Q.empty()){
			box *b=Q.back();
//			Q.pop_back();
			bool done=true;
			vector<box*> nt=b->getNeighbor();
			for (unsigned int i=0; i<nt.size(); i++){
				if (nt.at(i)->getSize()>2*b->getSize()){
					done=false;
					erase(Q1,nt.at(i));
					vector<box*> s=nt.at(i)->split();
					counter++;
					cBox+=3;
					for (unsigned int j=0; j<s.size(); j++){
						if (s.at(j)->c0())
							C0.insert(C0.begin(),s.at(j));
						else
							priorityQueue(Q,s.at(j));
					}
				}
			}
			if (done){
				priorityQueue(QR,b);
				erase(Q,b);
//				Q.remove(b);
			}
		}
		return QR;
	}
int mainFunc::run(){
	//subdivision step
	Q0.insert(Q0.begin(),B0);
	while (!Q0.empty()){
		box *b=Q0.back();
		Q0.pop_back();
		if (b->c0()){
			C0.insert(C0.begin(),b);
		}
		else if ((b->cxy())&&(b->getSize()<Max_Size)&&(b->getSize()>Min_Size)){
			priorityQueue(Q1,b);
			if (N>b->getSize())
					N=b->getSize();
		}
		else{
			vector<box *> s;
			s=b->split();
			counter++;
			cBox+=3;
			for (unsigned int i=0; i<s.size(); i++){
				Q0.insert(Q0.begin(),s.at(i));
			}
		}
//		cout<<"Q.size()"<<Q0.size()<<endl;
	}
	Q2=balance(Q1);
//	cout<<Q0.size()<<" "<<C0.size()<<" "<<Q1.size()<<" "<<counter<<endl;
	vector<box*> markedQ;
	unsigned int n=0;
	//balancing step
		while(!Q2.empty()){
			box *b=Q2.back();
			if(b->getSize()<=2*N){
				erase(Q2,b);
				if (b->ambiguous()){
					vector<box*> children=b->split();
					counter++;
					cBox+=3;
					for (unsigned int i=0; i<children.size(); i++){
						if (children.at(i)->c0())
							C0.push_back(children.at(i));
						else
							Q2.push_back(children.at(i));
					}
					vector<box*> temp;
					temp=balance(Q2);
					Q2=temp;
				}
				else if(b->suspicious()){
					if (b->getPoint().at(0).getX()<-1.174&&b->getPoint().at(0).getX()>-1.176)
						b->print();
					b->mark=true;
					markedQ.push_back(b);
				}
				else{
					b->addV();
					//if (!Q3.contains(b))
					Q3.push_back(b);
				}
				b=Q2.back();
			}  //end b.size()<=2*N

			if (markedQ.size()!=n){
				n=markedQ.size();
				while (!markedQ.empty()){
					box *mb=markedQ.at(0);					
					Q2.push_back(mb);
					markedQ.erase(markedQ.begin());
				}
			}
			else{
				while (!markedQ.empty()){
					box *mb=markedQ.at(0);				
					Q3.push_back(mb);
					markedQ.erase(markedQ.begin());
				}
				N=N*2;
			}
		}
//	cout<<Q1.size()<<" "<<C0.size()<<" "<<Q3.size()<<" "<<counter<<endl;
//	cout<<counter<<endl;
	return 1;
}
int mainFunc::run_Next_Step(){
	Rect.push_back(B0);
	for (unsigned int i=0; i<C0.size(); i++){
			box *b=C0.at(i);
//			g.drawRect(cord(b.getPoint().get(1).getX()), cord(b.getPoint().get(1).getY()), cordL(b.getX().length()), cordL(b.getY().length()));
			Rect.push_back(b);
	} 
	int count = 0;
	while(!Q3.empty()){
		    
			box *b=Q3.back();
			Rect_RED.push_back(b);
			Q3.pop_back();
//			b->addV();
			
			vector<Point *> p=b->getVertices();
			int size=p.size();
			if (size==2){
				count ++;
				double x1=p.at(0)->getX();
				double y1=p.at(0)->getY();
				double x2=p.at((1)%size)->getX();
				double y2=p.at((1)%size)->getY();
				Line.push_back(x1);
				Line.push_back(y1);
				Line.push_back(x2);
				Line.push_back(y2);
			}
			if (size==4){
				int s1=5;
				int s2=5;
				int d1=5;
				int d2=5;
				vector<Point> pt=b->getPoint();
				for (int i=0; i<3; i++)
					for (int j=i+1; j<4; j++){
						if ((p.at(i)->getX()==p.at(j)->getX())&&((p.at(i)->getX()==pt.at(0).getX())||(p.at(i)->getX()==pt.at(2).getX()))){
							s1=i;
							s2=j;
						}
						if ((p.at(i)->getY()==p.at(j)->getY())&&((p.at(i)->getY()==pt.at(0).getY())||(p.at(i)->getY()==pt.at(2).getY()))){
							s1=i;
							s2=j;
						}
					}
				for (int m=0; m<4; m++){
					if ((m!=s1)&&(m!=s2)){
						d1=m;
					}
				}
				for (int n=0; n<4; n++){
					if ((n!=s1)&&(n!=s2)&&(n!=d1)){
						d2=n;
					}
				}
				if ((distance(p.at(s1), p.at(d1))+distance(p.at(s2), p.at(d2)))<(distance(p.at(s1), p.at(d2))+distance(p.at(s2), p.at(d1)))){
					Line.push_back(p.at(s1)->getX());
					Line.push_back(p.at(s1)->getY());
					Line.push_back(p.at(d1)->getX());
					Line.push_back(p.at(d1)->getY());
					Line.push_back(p.at(s2)->getX());
					Line.push_back(p.at(s2)->getY());
					Line.push_back(p.at(d2)->getX());
					Line.push_back(p.at(d2)->getY());
				}
				else{
					Line.push_back(p.at(s1)->getX());
					Line.push_back(p.at(s1)->getY());
					Line.push_back(p.at(d2)->getX());
					Line.push_back(p.at(d2)->getY());
					Line.push_back(p.at(s2)->getX());
					Line.push_back(p.at(s2)->getY());
					Line.push_back(p.at(d1)->getX());
					Line.push_back(p.at(d1)->getY());
				}
			}

		}
		cout<<"The number of subdivision is "<<counter<<endl;
		cout<<"The number of boxes is "<<cBox<<endl;
		return 1;
}
void background(void)
{
	glClearColor(255.0/256.0,192.0/256.0,203.0/256.0,0.0);
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(draw_Box) {
	for(unsigned int rect_num=0; rect_num<Pic.size(); rect_num++){
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex2f(Pic.at(rect_num)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,0.0f,1.0f);
	    glVertex2f(Pic.at(rect_num)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,0.0f,1.0f);
	    glVertex2f(Pic.at(rect_num)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,0.0f,1.0f);
	    glVertex2f(Pic.at(rect_num)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glEnd();
	}
	for(unsigned int rect_num_2=0; rect_num_2<Pic_Red_Box.size(); rect_num_2++){
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(Pic_Red_Box.at(rect_num_2)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic_Red_Box.at(rect_num_2)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(1.0,0.0,0.0);
	    glVertex2f(Pic_Red_Box.at(rect_num_2)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic_Red_Box.at(rect_num_2)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(1.0,0.0,0.0);
	    glVertex2f(Pic_Red_Box.at(rect_num_2)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic_Red_Box.at(rect_num_2)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(1.0,0.0,0.0);
	    glVertex2f(Pic_Red_Box.at(rect_num_2)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic_Red_Box.at(rect_num_2)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glEnd();
	}
	} else {
	for(unsigned int rect_num=0; rect_num<1; rect_num++){
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex2f(Pic.at(rect_num)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,1.0f,0.0f);
	    glVertex2f(Pic.at(rect_num)->v1.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,1.0f,0.0f);
	    glVertex2f(Pic.at(rect_num)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v3.doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0f,1.0f,0.0f);
	    glVertex2f(Pic.at(rect_num)->v2.doubleValue()*Size_of_Graphic-Movement_X,Pic.at(rect_num)->v4.doubleValue()*Size_of_Graphic-Movement_Y);
		glEnd();
	}
	}


	for(unsigned int line_num=0; line_num<Line_Red.size();line_num=line_num+4){
		glBegin(GL_LINES);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(Line_Red.at(line_num).doubleValue()*Size_of_Graphic-Movement_X,Line_Red.at(line_num+1).doubleValue()*Size_of_Graphic-Movement_Y);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(Line_Red.at(line_num+2).doubleValue()*Size_of_Graphic-Movement_X,Line_Red.at(line_num+3).doubleValue()*Size_of_Graphic-Movement_Y);
		glEnd();
	}

	glFlush();
}
void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:             // ESCAPE key
	  exit (0);
	  break;
  }
}

void myReshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	if(w <= h)
		gluOrtho2D(-1,1,-1,1);
//	gluOrtho2D(-3,4.5,-4.5,4.5*(GLfloat)h/(GLfloat)w);
//	else	
//		gluOrtho2D(-500,500,-500,500);
//		gluOrtho2D(-3,4.5*(GLfloat)w/(GLfloat)h,-4.5,4.5);
	glMatrixMode(GL_MODELVIEW);
}
void Calculate_Position_Graphic(double p1, double p2, double p3, double p4){
	double size_x = (p1 < p2)?(p2-p1):(p1-p2);
	double size_y = (p3 < p4)?(p4-p3):(p3-p4);
	Size_of_Graphic = (size_x <= size_y)? (1.9/size_y).doubleValue(): (1.9/size_x).doubleValue();
	Movement_X = (p1+p2>=0)?(p1+p2).doubleValue()*Size_of_Graphic/2:(-p1-p2).doubleValue()*Size_of_Graphic/2;
	Movement_Y = (p3+p4>=0)?(p3+p4).doubleValue()*Size_of_Graphic/2:(-p3-p4).doubleValue()*Size_of_Graphic/2;
}
// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[])
{
	double box_v1, box_v2, box_v3, box_v4;

	string CurveDef = "x^2 - x^4 - y^2 + 1";  // default curve
	BigFloat  coeffnew;

	switch (argc) { // process inputs
	case 2:
	   CurveDef = argv[1];		// user-defined curve
	case 1:
	   Bipolyno = Bipolyno.getbipoly(CurveDef);
	   Bipolyno.dump("Your input Curve is ");
	   cout<<"-------------"<<endl;
	   box_v1 = -1.5;
	   box_v2 = -1.5;
	   box_v3 = 1.5;
	   box_v4 = 1.5;
	   break;
	case 10:
	   draw_Box = static_cast<bool>(std::atoi(argv[9]));
	case 9:
	   coeffnew =argv[8];
	case 8:
	   Min_Size = argv[7]; 
	case 7:
	   Max_Size = argv[6]; 
	case 6:
	   CurveDef = argv[1];
	   Bipolyno = Bipolyno.getbipoly(CurveDef);
	   Bipolyno.dump("Input Curve is");
	   if (coeffnew != 0) {
		Bipolyno.coeffX[0].setCoeff(0,coeffnew);
	        Bipolyno.dump("Modified Iput Curve is");
	   }
	   cout<<"-------------"<<endl;
	   box_v1 = argv[2];
	   box_v2 = argv[3];
	   box_v3 = argv[4];
	   box_v4 = argv[5];
	case 3:
	case 4:
	case 5:
	default:
	   cout << "Synopsis:" << endl;
	   cout << "./CXY \"curveDef\" [Xmin] [Ymin] [Xmax] [Ymax] [max-size] [min-size] [offset]  "<< endl
		<< "The Default Curve is \"x^2 - x^4 - y^2 +0.01\" " << endl
	    << "The Default box is \"-1.5,-1.5,1.5,1.5\" " << endl
		<< "E.g. parseCurve \"y - x^2\" " << endl
		<< "E.g. parseCurve \"y - x^2\" -1.4 -1.4 1.6 1.6" << endl;
	   exit(1);
	}//switch

	Curve<NT> tmppoly = Bipolyno;
	Differenticate_X = tmppoly.differentiateX();
	tmppoly = Bipolyno;
	Differenticate_Y = tmppoly.differentiateY();
	mainFunc a(box_v1,box_v2,box_v3,box_v4);
	Calculate_Position_Graphic(a.B0->v1,a.B0->v2,a.B0->v3,a.B0->v4);
	a.run();
	a.run_Next_Step();
	Pic = a.Rect;
	Pic_Red_Box = a.Rect_RED;
	Line_Red = a.Line;
	cout <<"-------------Graphic----------------"<<endl;
	cout <<"--------Press ESC to exit-----------"<<endl;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("CXY       (Press ESC to exit)");
	background();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc (Keyboard);
	glutMainLoop();
//	PostQuitMessage(1);
    return 0;
}//main 
