/****************************************************************************
 * Core Library Version 1.7, August 2004
 * Copyright (c) 1995-2004 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of CORE (http://cs.nyu.edu/exact/core/); you may
 * redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with CORE.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * File: Bezier.h
 *
 * Description: 
 *
 * 	This defines the following classes:
 * 		Point 	-- 2d points based on BigFloat
 * 		CHull 	-- Convex hull of Point
 * 		Bezier	-- Bezier curve
 *
 * 	IMPORTANT: You should make sure that all the points
 * 		have exact BigFloat values (if unsure, call "makeExact()"
 * 		to clear error bits).
 *
 *  Author:  Chee Yap
 *  Date:    August 1, 2004
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $Source: /home/exact/cvsroot/exact/corelib2/progs/bezier/Bezier.h,v $
 * $Revision: 1.1 $ $Date: 2006/03/07 04:51:23 $
 ***************************************************************************/

#ifndef CORE_BEZIER_H
#define CORE_BEZIER_H

// #define CORE_LEVEL 4

#include <fstream>
#include <list>
// #include "CORE/geometry2d.h"
#include "CORE/poly/Poly.h"

using namespace std;

CORE_BEGIN_NAMESPACE

// ==================================================
// Curve Class
// ==================================================

//typedef BigInt NT;
//typedef Expr   NT;
//typedef Polynomial<NT>        PolyNT;
//typedef std::vector<Expr>	VecExpr;
//typedef std::vector<BigInt>	VecBigInt;
//typedef std::vector<NT>       VecNT;
//typedef std::vector<Polynomial<NT> >	VecPoly;

class Point;
typedef std::vector<Point>	VecPoint;


  ////////////////////////////////////////////////////////
  // Generic Dump Functions: 
  //     provide only static functions, so any class can call them
  //
  ////////////////////////////////////////////////////////
class Dump {

public:
  // dumpObject() -- should be implemented in each subclass
  static void dumpObject(std::ostream &os, std::string com) {
     os << "Generic dumpObject Function -- ";
     os << " replaced by your class specific version";
     os << std::endl;
  }

  // dump(ofs, msg, com, fcnPtr) -- dump to a file
  // 	The main dump routine: the 4th argument is a pointer
  //       to a dump function from subclass
  static void filedump(std::ostream & os, std::string msg, std::string com,
	void (* dumpFcnPtr)(std::ostream, std::string)   ){
    // message:
    if (msg != "") os << msg << std::endl;
    // comment character:
    os << com;
    // class specific action:
    (* dumpFncPtr)(os,com);  // apply class specific dump function
  }

  // dump(ofs, msg, com, fncPtr) -- dump to file
  static void dump(std::ofstream & ofs, std::string msg, std::string com,
	     void (* dumpFcnPtr)(std::ostream, std::string)   ){
    std::ostream * osp = & ofs;  // convert ofstream to ostream
    filedump((*osp), msg, com, dumpFcnPtr);
  }

  // dump(msg, com, fncPtr) -- dump to std output
  static void dump(std::string msg, std::string com
	     void (* dumpFcnPtr)(std::ostream, std::string)   ){
    filedump(std::cout, msg, com, dumpFcnPtr);
  }

  // dump(msg, fncPtr) -- dump to std output
  static void dump(char * msg,
	     void (* dumpFcnPtr)(std::ostream, std::string)   ){
    filedump(std::cout, msg, "", dumpFcnPtr);
  }
};//Dump class


/************************************************************
 * POINT CLASS
 ************************************************************/
class Point {

public:
	BigFloat x, y;
public:
  // CONSTRUCTORS:
	Point(): x(0.0), y(0,0) {}
	Point(const BigFloat a, const BigFloat b) : x(a), y(b) {
		x.makeExact(); y.makeExact(); }
	Point(double a, double b) : x(BigFloat(a)), y(BigFloat(b)) {}
	Point(int a, int b) : x(BigFloat(a)), y(BigFloat(b)) {}
	Point(const Point & p) : x(p.x), y(p.y) {}
  // METHODS:
  // DISTANCES 
  //  -- may want a precision argument...
  BigFloat distance(const Point & p) const {
	  return sqrt(((p.x - x)*(p.x - x) + (p.y - y)*(p.y - y)));
  }
  // LENGTH
  BigFloat length() const { return distance(Point(0, 0)); }

  //VECTOR OPERATIONS
  Point operator-(const Point & p) const {
	  return Point(x-p.x, y-p.y);
  }
  Point operator+(const Point & p) const {
	  return Point(x+p.x, y+p.y);
  }
  Point operator*(const BigFloat & c) const {
	  return Point(c*x, c*y);
  }

  //COMPARISONS
  bool operator==(const Point & p) const {
	  return ((x == p.x) && (y == p.y));
  }
  bool operator!=(const Point & p) const {
	  return !operator==(p);
  }
  //TOWARDS
  Point towards(Point target, BigFloat t) const {
	  if (t==0.5) 
	     return Point( (x + target.x).div2(), (y + target.y).div2());
	  return Point( (1-t)*x + t* target.x, (1-t)*y + t* target.y);
  }
  //HALFWAY
  Point halfway(Point target) const {
	  return Point( (x + target.x).div2(), (y + target.y).div2());
  }
  //LEXICAL COMPARISON
  //  compare_lex(p) is the most general version:
  //  	returns -1 if *this < p
  //  	        +1 if *this > p
  //  	         0 if *this = p
  int compare_lex(Point p) {
	  if (x< p.x) return -1;
	  if (x> p.x) return +1;
	  if (y< p.y) return -1;
	  if (y> p.y) return +1;
	  return 0;
  }
  //  less_lex(p)    -- returns true iff *this < p
  bool less_lex(Point p) {
	  return (compare_lex(p) < 0);
  }
  //  less_eq_lex(p) -- returns true iff *this <= p
  bool less_eq_lex(Point p) {
	  return (compare_lex(p) <= 0);
  }
  
  //INPUT-OUTPUT
  friend std::ostream& operator<< (std::ostream&, const Point&);
  // write point p to output stream
  // The format is, e.g.,  Point(1.0, 23)

};// Point Class


/* orientation: computes the orientation of points a, b, and c as the sign
                of the determinant
                | ax  ay  1 |
                | bx  by  1 |
                | cx  cy  1 |
             i.e., it returns +1 if point c lies left of the directed line
                  through a and b, 0 if a,b,and c are collinear, and
                  -1 otherwise.
 */
int orientation2d(const Point& a, const Point& b, const Point& c)
{
   BigFloat d1 = (a.x - b.x) * (a.y - c.y);
   BigFloat d2 = (a.y - b.y) * (a.x - c.x);
   if (d1 == d2)
      return 0;
   else
      return (d1 > d2) ? +1 : -1;
}

bool leftTurn(const Point& a, const Point& b, const Point& c) {
	return (orientation2d(a,b,c)>0);
}

bool rightTurn(const Point& a, const Point& b, const Point& c) {
	return (orientation2d(a,b,c)<0);
}
bool betweenVar(const Point& a, const Point& b, const Point& c) {
	return (((a.x-b.x)*(c.x-b.x) + (a.y-b.y)*(c.y-b.y))<0);
     // if (a,b,c) are collinear, this implies b is strictly between a and c
}

// Friend of Point class
std::ostream &operator<<(std::ostream &out, const Point &p) {
  out << "Point(" << p.x << "," << p.y << ")";
  return out;
}

/************************************************************
 * CONVEX HULL CLASS
 ************************************************************/
class CHull {

public:
	int n; //number of points
	VecPoint vp;
	bool isConvex; // isConvex=true iff vp is really convex
	BigFloat diam; // if diam=0, then bounding box unknown
	BigFloat xmin, xmax, ymin, ymax;
public:
   // CONSTRUCTORS:
  	CHull(): n(0), vp(0), isConvex(false), diam(0) {}
	CHull(std::vector<Point > & v):
		n(v.size()), vp(v), isConvex(false), diam(0) {;}
	CHull(const CHull& ch) :
		n(ch.n), vp(ch.vp), isConvex(ch.isConvex), diam(ch.diam) {;}
   // METHODS:
   //   convexify() runs the giftwrap algorithm
   //   	and replace vp by the true convex hull;
   //   	the isConvex flag is set to true.
	int convexify(){
	    Point* hull = new Point[n+1];  // one extra position for sentinel!
	    Point currPoint = vp[0];
	    // Initialize convex hull to the lex-smallest point
	    for (int i=1; i<n; i++) {      // hull[0], hull[n] unused!
		if (vp[i].less_lex(currPoint)){
			hull[i] = currPoint; currPoint = vp[i];
		} else hull[i] = vp[i];
	    }
	    // currPoint is our first hull point!
	    int next=n;
	    hull[n]=currPoint;
	    int size=0; // size of current hull.  Moreover, the current hull
			// is hull[0,1,...,size]).
	    // Try to get second hull point
	    // 		(on the vertical line through currPoint!)
	    for (int i=1; i<n; i++)
	        if (hull[i].x == currPoint.x && hull[i].y>hull[next].y)
			next=i;
	    if (next == n) { // did not find second hull point
	    } else {  // set up hull[0] and hull[n-1] to hold hold points
	       hull[0] = hull[next];  // hull[0] was not assigned earlier
	       hull[next] = hull[1];  // empty hull[1] for next hull point
	       size = 1;  // current hull has 2 points now!
	    }
	    Point sentinel = hull[n]; // sentinel variable makes code clearer
	    do { // Giftwrapping!
	      hull[size] = currPoint; // invariant: hull[size] is empty
	      size++;   // after this step, size IS the size of current hull!
	      next = size;
	      for (int i=size+1; i<=n; i++){ // loop to find next hull point
	        int o = orientation2d(currPoint, hull[next], hull[i]);
	        if ((o<0) ||  // rightturn, or
	           (o==0 && betweenVar(hull[next],currPoint,hull[i])))
	    	     next = i; // hull[next] strict.between currPoint & hull[i]
              }
	      currPoint = hull[next];  // Empty hull[size] for next round
	      hull[next] = hull[size]; // N.B. Even if next=n, this is OK! 
	    } while (sentinel != currPoint);
	    // The convex hull is counter clockwise in hull[0,1,...,size-1]
	    vp.clear(); // cleanup 
	    for (int i=0; i<size; i++)
		    vp.push_back(hull[i]);
	    isConvex = true;
	    n = size;
	    delete [] hull;
	    return n;
	}//convexify

	//diameter() computes an approximation to the
	//	true diameter (currently, it is the length
	//	of the smallest bounding box)
	BigFloat diameter() {
		if (diam == 0) {
cout << "does diameter work?  start! diam = " << diam << endl;
		   xmin=vp[0].x;
cout << "xmin=" << xmin << endl;
		   xmax=xmin;
cout << "xmax=" << xmax << endl;
		   ymin=vp[0].y; ymax=ymin;
		   for (int i=1; i<n; i++) {
cout << "inside loop, n = " << n << endl;
			if (vp[i].x <xmin) xmin=vp[i].x;
			if (vp[i].x >xmax) xmax=vp[i].x;
			if (vp[i].y <ymin) ymin=vp[i].y;
			if (vp[i].y >ymax) ymax=vp[i].y;
		   }
		   diam = core_max(xmax-xmin, ymax-ymin);
		}
cout << "does diameter work?  end!" << endl;
		return diam;
	}// diameter

  ////////////////////////////////////////////////////////
  // Dump Functions
  ////////////////////////////////////////////////////////
  // dumpObject() -- specific to each class
  void dumpObject(std::ostream &os, std::string com) const{
    if(n == 0) {
      os << com << " Empty convex hull " << std::endl;
      return;
    }
    os << com << "Convex Hull (n = " << n << std::endl;
    for (int i=0; i < n; i++)
	    os << com << vp[i].x << vp[i].y << std::endl;
    os << com << ")" << n << std::endl;
  }
  // mydump()
  void mydump(std::string m = ""){
	  Dump::dump(m, &dumpObject);
  }
  // dump(ofs, msg, com) -- dump to file
  void dump(std::ofstream & ofs, std::string msg, std::string com){
	  Dump::dump(ofs, msg, "# ", &dumpObject);
  }
  
  ////////////////////////////////////////////////////////
  // plot for CHull
  ////////////////////////////////////////////////////////
  void plothull(int fileNo=1){

  const char* filename[] = {"data/input", "data/plot", "data/plot2"};

cout<< "before diameter is called" << endl;
  if (diam == 0) diameter();
cout<< "after diameter is called" << endl;

  ofstream outFile;
  outFile.open(filename[fileNo]); // ought to check if open is successful!
  outFile << "########################################\n";
  outFile << "# Convex Hull: \n";
  mydump(outFile,"", "# ");
  outFile << std::endl;
  outFile << "#     n = " << n << std::endl;
  outFile << "#     isConvex = " << isConvex << std::endl;
  outFile << "#     diam = " << diam << std::endl;
  outFile << "#     (xmin,xmax) = " << xmin << xmax << std::endl;
  outFile << "#     (ymin,ymax) = " << ymin << ymax << std::endl;
cout<< "middle of plothull " << endl;
  outFile << "########################################\n";
  outFile << "# X-axis " << std::endl;
  outFile << "o 2" << std::endl;
  outFile << "0.99 \t 0.99 \t 0.99" << std::endl;
  outFile << (xmin * 1.05) << "\t" << 0 << std::endl;
  outFile << (xmax * 1.05) << "\t" << 0 << std::endl;
  outFile << "########################################\n";
  outFile << "# Y-axis " << std::endl;
  outFile << "o 2" << std::endl;
  outFile << "0.99 \t 0.99 \t 0.99" << std::endl;
  outFile << 0 << "\t" << (ymin * 1.05) << std::endl;
  outFile << 0 << "\t" << (ymax * 1.05) << std::endl;
  outFile << "########################################\n";
  outFile << "p " << n << std::endl;
  outFile << "0.50 \t 0.99 \t 0.79" << std::endl;
  for (int i=0; i<n; i++) 
	  outFile << vp[i].x << "\t" << vp[i].y << std::endl;
  outFile << "############ END OF FILE ###############\n";
cout<< "end diameter is called" << endl;
  }//plothull

  //INPUT-OUTPUT
  //  stream input
  friend std::ostream& operator<< (std::ostream&, const CHull& c);

};// CHull Class


// output stream for CHull
std::ostream& operator<<(std::ostream& o, const CHull& c) {
  o <<   "CHull( n = " << c.n << ", " << std::endl;
  for (int i=0; i<c.n; i++)
      o <<   "   " << c.vp[i].x << ", " << c.vp[i].y << std::endl;
  o << ")" << std::endl;
  return o;
}


/************************************************************
 * BEZIER CLASS
 ************************************************************/
class Bezier : public CHull {
public:
  int deg;         // one less than number of control points
  VecPoint cp;     // sequence of control points
  Bezier *root;    // pointer to the original (root) curve
  BigFloat tmin;   // tmin=0.0;  The curve is equal to root[tmin,tmax]
  BigFloat tmax;   // tmax=1.0;

  // assert (0<= tmin < tmax <=1)

  /////////////////////////////////////////////////////////////////////////
  //  PLOT DATA
  /////////////////////////////////////////////////////////////////////////
  // Plot Parameters
  BigFloat eps;   // eps=0.1;
  int fileNo;     // fileNo=1;

  // Colors for plotting curves
  const static int NumColors=7;
  static double red_comp(int i){
  	static double RED_COMP[] = {0.9, 0.8, 0.7, 0.6, 0.8, 0.8, 0.7};
	return RED_COMP[i % NumColors];
  }
  static double green_comp(int i){
  	static double GREEN_COMP[] = {0.5, 0.9, 0.3, 0.9, 0.7, 0.55, 0.95};
	return GREEN_COMP[i % NumColors];
  }
  static double blue_comp(int i){
  	static double BLUE_COMP[] = {0.8, 0.3, 0.8, 0.5, 0.4, 0.85, 0.35};
	return BLUE_COMP[i % NumColors];
  }

  /////////////////////////////////////////////////////////////////////////
  // CONSTRUCTORS
  /////////////////////////////////////////////////////////////////////////
  // Null curve
  Bezier() : deg(0) {} 
  // Construct from VecPoints:
  Bezier(VecPoint v) : deg(v.size()-1), cp(v),
  	tmin(BigFloat::getZero()), tmax(BigFloat::getOne()) {} 
  // Construct from an array of n points:
  Bezier(int n, Point * pa, Bezier * bc): deg(n-1) {
	for (int i=0; i<n; i++)
		cp.push_back(pa[i]);
	root= bc;
	tmin=BigFloat::getZero(); tmax=BigFloat::getOne();
  }
  //Copy Constructor
  Bezier(Bezier& bc) : deg(bc.deg-1), cp(bc.cp), root(bc.root),
	tmin(bc.tmin), tmax(bc.tmax) {}

  /////////////////////////////////////////////////////////////////////////
  // METHODS
  /////////////////////////////////////////////////////////////////////////
  // getPoint (t)
  //	computes the point at parameter value t
  //	NOTE: this is just the subdivision algorithm, but simplified!  
  //	
  Point getPoint(const BigFloat t) const {
  
    Point *  curr = new (Point)[deg];    // current array

    for (int i=0; i<deg; i++) { // get initial 
	    curr[i] = cp[i].towards(cp[i+1], t);
    }
    for (int i=0; i<deg-1; ++i) {
      // invariant: curr contains (deg-i) points
      for (int j=0; j<deg-1-i; ++j) {
        curr[j] = curr[j].towards(curr[j+1], t);
      }
    }
    Point p(curr[0]); 
    delete [] curr;
    return p;
  }//getPoint

  // Subdivision (t, L, R)
  //	will split at parameter t, and
  //	return the left and right subcurves in L and R
  //	Return value is the left subcurve
  //
  Bezier subdivision(const BigFloat t, Bezier & left, Bezier & right) {
  //    assert(tmin < t && t < tmax);
cout<<"subdivision begin: t=" << t << endl;
    Point *  lseq = new (Point)[deg+1];  // these will become the left and
    Point *  rseq = new (Point)[deg+1];  //   right control polygons
    Point *  curr = new (Point)[deg];    // current array

    lseq[0] = cp.front();  // initialize
    rseq[deg] = cp.back(); // initialize
    for (int i=0; i<deg; i++) {
	    curr[i] = cp[i].towards(cp[i+1], t);
    }
    for (int i=0; i<deg-1; ++i) {
      // invariant: curr contains (deg-i) points
      lseq[i+1] = curr[0];
      rseq[deg-i-1] = curr[deg-i-1];

      for (int j=0; j<deg-1-i; ++j) {
        curr[j] = curr[j].towards(curr[j+1], t);
      }
    }
    left  = Bezier(deg+1, lseq, root);
    right = Bezier(deg+1, rseq, root);
cout<<"subdivision end" << endl;
    delete [] lseq; delete [] rseq; delete [] curr;
    return left;
  }//subdivision

  /////////////////////////////////////////////////////////////////////////
  // plot(eps)
  //
  // 	All parameters have defaults
  //
  //    Gives the points on the curve at resolution "eps".  Currently,
  //    eps is viewed as delta-x step size (but it could change).
  //    The display is done in the rectangale 
  //    defined by [(x1, y1), (x2, y2)].
  //    The output is written into a file in the format specified
  //    by our drawcurve function (see COREPATH/ext/graphics).
  //
  //    Heuristic: the open polygonal lines end when number of roots
  //    changes...
  //
  int  plot( int n=100, int fileNo=1){

  const char* filename[] = {"data/input", "data/plot", "data/plot2"};

  ofstream outFile;
  outFile.open(filename[fileNo]); // ought to check if open is successful!
  outFile << "########################################\n";
  outFile << "# Convex Hull: \n";
  // dump(outFile,"", "# ");
  outFile << std::endl;
  outFile << "#     n = " << n << std::endl;
  outFile << "#     isConvex = " << isConvex << std::endl;
  if (diam == 0) diameter();
  outFile << "#     diam = " << diam << std::endl;
  outFile << "#     (xmin,xmax) = " << xmin << xmax << std::endl;
  outFile << "#     (ymin,ymax) = " << ymin << ymax << std::endl;
  outFile << "########################################\n";
  outFile << "# X-axis " << std::endl;
  outFile << "o 2" << std::endl;
  outFile << "0.99 \t 0.99 \t 0.99" << std::endl;
  outFile << (xmin * 1.05) << "\t" << 0 << std::endl;
  outFile << (xmax * 1.05) << "\t" << 0 << std::endl;
  outFile << "# Y-axis " << std::endl;
  outFile << "o 2" << std::endl;
  outFile << "0.99 \t 0.99 \t 0.99" << std::endl;
  outFile << 0 << "\t" << (ymin * 1.05) << std::endl;
  outFile << 0 << "\t" << (ymax * 1.05) << std::endl;
  outFile << "########################################\n";
  outFile << "p " << n << std::endl;
  outFile << "0.50 \t 0.99 \t 0.79" << std::endl;

  BigFloat eps = BigFloat(1)/n;
  outFile << "#### eps = " << eps << std::endl;
  BigFloat t = 0;
  outFile << "p " << n-1 << std::endl;
  Point p;
  for (int i=0; i<n-1; i++) {
	p= getPoint(t); t += eps;
        outFile << p.x << "\t" << p.y << std::endl;
  }
  outFile << "############ END OF PLOT ###############\n";
  return n;
  }//plot


  /////////////////////////////////////////////////////////////////////////
  // dumpObject(os)  -- for Dump class
  // How come it is not called by dump in the Dump class?
  void dumpObject(std::ostream &os, std::string com) const {
     if (deg==0) {
        os << com << "Empty Bezier Object" << std::endl;
        return;
     } else {
        os << com << "Bezier Object ( deg =" << deg 
		<< ", Control Points = " << std::endl;
	for (int i=0; i<=n; i++)
           os << com << "   (" << vp[i].x  
		<< ", " << vp[i].y << ")" << std::endl;
        os << com << ")" << std::endl;
     }
  } 
  // mydump(m)
  void mydump(std::string m = ""){
	  Dump::dump(m, &dumpObject);
  }

  /*
  // Helper Functions:
  // Set Parameter
  //      -- does not work?
  static int setPlotParameters(BigFloat e=0.1, BigFloat tn =0.0,
  	     BigFloat tx =1.0, int fn=1) {
     eps = e; tmin = tn; tmax = tx; fileNo = fn;
     return 1;
  }
  */

};// Bezier class

// Initializing Static Plot Parameters
// Bezier::setPlotParameters(0.1, 0.0, 1.0, 1); // use default values

  ////////////////////////////////////////////////////////
  // Bezier helper functions
  ////////////////////////////////////////////////////////

CORE_END_NAMESPACE
#endif
/*************************************************************************** */
// END BEZIER
/*************************************************************************** */

