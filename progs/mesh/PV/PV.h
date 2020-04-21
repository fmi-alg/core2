#ifndef INTERVAL_H_
#define INTERVAL_H_
#include <vector>
#include <iostream>

using namespace  std;
class Point {
	public:	
		Point();	
		Point(double x, double y);	
		void copy(Point p);	
		double * getPoint();	
		double getX();	
		double getY();	
		void setX(double x);	
		void setY(double y);	
		int sign();	
		void print();	
		//return true if this point contains in the edge (a,b).
		bool contain(Point a, Point b);
	private:
		double x;
		double y;
};
class Interval {
	public:
		Interval();	
		Interval(double a, double b);
		
		void copy(Interval I);	
		double* getInterval();	
		double getA();	
		double getB();	
		void setA(double a);
		void setB(double b);
		void setMinus();
		bool zero();
		Interval multiply(Interval &s, Interval &t);
		Interval plus(Interval &s, Interval &t);	
		Interval minus(Interval &s, Interval &t);
		Interval divide(Interval &s, Interval &t);	
		void print();	
		bool overlap(Interval s, Interval t);
		double length();
	private:
		double a;
	    double b;
};
double min(double x, double y);
double max(double x, double y);
class ImplicitCurve {
	public:
		ImplicitCurve(){};
		double value(double x, double y);
		Interval value(Interval &s, Interval &t);
		double * dValue(double x, double y);
		Interval * dValue(Interval &s, Interval &t);
};

class box{
	public:
		box();
		box(Point a, Point b);
		vector<box *> split();
		Point  m(Point a, Point b);
		Interval getX();
		Interval getY();
		vector<Point> getPoint();
		vector<box *> getChildren();
		box * getParent();
		vector<box *> getNeighbor();
		vector<Point *> getVertices();
		double getSize();
	//box is suspicious if its four corners have the same sign.
        bool suspicious();	
	//box is ambiguous if its four corners have the same sign, plus it has 2 vertices.
        bool ambiguous();	
	//c0 if for box with interval x & y, 0 not \in f(x,y).
	    bool c0();	
	//c1 if for box with interval x & y, 0 not \in <f'(x,y), f'(x,y)>.
	    bool c1();	
	//cxy if for box with interval x & y, 0 not \in fx(x,y) or fy(x,y).
	    bool cxy();
        void print();
        void addV();	
	//return true if the edge (a,b) already exists vertices.
	    bool existV(Point a, Point b);	
	//return true if this box contains point a.
	    bool containP(Point a);	
	    bool existV(Point a);
		bool operator ==(box & ref);
		void RemoveFirst(vector<box *> &vec, box *b);
//	private:
		double v1;
		double v2;
		double v3;
		double v4;
		Interval x;
	    Interval y;
		vector<Point> p;
		double size;
		box * parent;
		vector<box *> children;
		vector<box *> neighbor;
		vector<Point *> vertices;
};

class mainFunc{
	public:	
		mainFunc();	
		mainFunc(double Point_1_x,double Point_1_y,double Point_2_x,double Point_2_y);	
		double distance(Point *a, Point *b);
		void erase(vector<box *> &vec, box *b);
		void priorityQueue(vector<box*> &vec, box *b);
		int run();
		int run_Next_Step(void);
	public:
		int cBox;	
	    int w;
	    int h;
		double scaler;
		int moveX;
		int moveY;
		int counter;
		int ccc;
		double N;
		box *B0;
		vector<box*> Q0;
		vector<box*> C0;
		vector<box*> C1;

		vector<box*> Q1;
		vector<box*> Q2;
		vector<box*> Q3;
		vector<box*> Rect;
		vector<box*> Rect_RED;
		vector<double> Line;
};

#endif /*INTERVAL_H_*/

