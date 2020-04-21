/* file: pivot.h
 * Since Core Library Version 1.7
 * Author: Seung-Hyun Yun, Chee Yap
 */
 
#ifndef _PIVOT_H
#define _PIVOT_H

#include <iostream>
#include <fstream>
#include <iomanip>

#define CORE_LEVEL 4
#include "CORE/CORE.h"

using namespace std;

// Point class 
class Point
{ 
public:
   // constructors and destructor
   Point() { x = y = 0.0; };
   Point( double x, double y ) { this->x = x; this->y = y; }
   ~Point(){}

public:
   // operator overloading.
   Point& operator =( const Point& pt )
   	{ x = pt.x; y = pt.y; return (*this); }
   double operator *( const Point& pt )
   	{ return ( x*pt.x + y*pt.y ); }
   double operator *( const double& c )
   	{ x *= c;  y *= c; return c; }
   bool operator ==( const Point& pt ) 
   	{ return ( x==pt.x && y==pt.y) ; }
   bool operator !=( const Point& pt ) 
   	{ return ( x!=pt.x || y!=pt.y) ; }
   Point operator -( const Point& pt ) 
   	{ return Point( x-pt.x, y-pt.y ); }

   inline double get_norm()
   	{ return sqrt( (x*x + y*y) ); }
   inline double get_norm_square()
   	{ return (x*x + y*y); }
   inline void normalize()
   	{ double len = get_norm(); x /= len; y /= len; }
public:
   // class attribute.
   double x;
   double y;
};

// testing a single pivoting strategy
void pivotest( Point *Pts, int strategy );
// testing and comparing two pivoting strategies
void pivotest2( Point *Pts, int strategy1, int strategy2 );
// approximate area computation using a particular strategy
double area_approx( Point p, Point q, Point r, int pivot_strategy );
// exact area computation (but rounded to double)
double area_exact( Point p, Point q, Point r );
// chooses a pivot based on strategy
inline int pivotChoice( Point p, Point q, Point r, int strategy );
// computes the angle at origin made by two vectors
double get_angle( Point v0, Point v1 );
// sign of number
inline int get_sign(double x)
	{return (x>0) ? 1 : ((x<0) ? -1 : 0);}
// isPositive?
inline bool isPositive(double x)
	{return (x>0) ? true : false;}
// isNegative?
inline bool isNegative(double x)
	{return (x<0) ? true : false;}
// index of max(x,y,z)
inline int max_index( double x, double y, double z) {
    if ( x > y ) {
        if ( x > z ) return 1; // x > y,z
        else return 3; // z >= x > y
    } else {
        if ( y > z ) return 2; // y >= x,z
        else return 3; // z >= y >= x;
    }
}
// index of median(x,y,z)
inline int median_index( double x, double y, double z) {
   if ( x > y ) {
      if ( y > z ) return 2;    // x > y > z
      else if (x > z) return 3; // x > z >= y
      else return 1;            // z >= x > y
   } else {
      if ( x > z ) return 1;    // y >= x > z
      else if (y>z) return 3;   // y > z >= x
      else return 2;            // z >= y >= x
   }
}

/* Legacy from Yun's code
 * *******************************************************************
void signed_area( Point *Pts, int size, int strategy, ofstream& out );
int get_pivot_idx_I( Point p, Point q, Point r );
int get_pivot_idx_II( Point p, Point q, Point r );
int get_pivot_idx_III( Point p, Point q, Point r );

double rho( double a, double b );
double rho( Point p0, Point p1 );
double rho( Point p, Point q, Point r, int pivot_idx );
int ExprSign( Expr e );
Expr ExprAbs( Expr e );
 * *******************************************************************
 */

#else
#endif // _PIVOT_H

