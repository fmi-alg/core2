/***********************************************
 * file: testPoint.ccp
 *
 * purpose:  To exercise all the functions in Point2d.cc of
 *           the geometry2d COREX
 *
 * author: Yaping Yuan
 * Since CORE Library Version 1.2
 * $Id: testPoint.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 **********************************************/

#ifndef CORE_LEVEL
# define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

using namespace std;

int main()
{
   double xx = 2;       
   double yy = 3;      
   Point2d a(0.0, 0.0);
   Point2d b(5.0, 0.0);
   Point2d c(0.0, 12.0);
   Point2d d(3.0, 7.0);
   Point2d p(xx, yy);

   Point2d p1(sqrt(xx), sqrt(yy));
   double scale = 3;
   Point2d p2(sqrt(xx)*scale, sqrt(yy)*scale);
                // Before version 1.3, if you replace "3" by "/3", you saw
                // a drastic slow down.  This is no longer true.

   cout << "/* *************** DEFINITION OF POINTS ************* */\n";
   /*a.dump("a = ", "\n");
   b.dump("b = ", "\n");
   c.dump("c = ", "\n");
   d.dump("d = ", "\n");
   p.dump("p = ", " = (xx, yy) \n");
   p1.dump("p1 = ", " = (sqrt(xx), sqrt(yy)) \n");
   p2.dump("p2 = ", " = (sqrt(xx)*3, sqrt(yy)*3) \n");
   */
   cout << "/* *************** IDENTITY TESTS ******************* */\n";
   if (p1 == b) 
      cout << "p1 = b" << " (ERROR!) \n";
   else
      cout << "p1 != b" << " (CORRECT!) \n";

   cout << "/* *************** orientation2d TESTS***************** */\n";
   int orient = orientation2d(a,b,c);
   cout << "orientation2d of (a,b,c)  = " 
        << orient << " (1=ccw, 0=collinear, -1=cw) \n";
   orient = orientation2d(a,c,b);
   cout << "orientation2d of (a,c,b)  = " 
        << orient << " (1=ccw, 0=collinear, -1=cw) \n";

   orient = orientation2d(a, p1, p);
   cout << "orientation2d of (a,p1,p) = "
        << orient << " (1=ccw, 0=collinear, -1=cw)\n";

   cout << "/* *************** DISTANCE COMPUTATION ************** */\n";
   double dist = a.distance(b);
   cout << "Distance (a, b) = " << dist << endl;

   dist = p1.distance(p2);
   cout << "Distance (p1, p2) = " << dist << endl;

   double dist1 = p2.distance(p1);
   cout << "Distance (p2, p1) = " << dist1 << endl;

   cout << "Check if the last 2 distances are equal\n";
   cout.flush();
   if (dist == dist1)
        cout << "They are equal (CORRECT!)\n";
   else
        cout << "They are not equal (ERROR!)\n";

   cout << "/* *************** AREA COMPUTATION ****************** */\n";
   double area1 = area(a, b, c);
   cout << "Twice Area of triangle (a,b,c) = " << area1 << endl;

   area1 = area(a, p, p1);
   cout << "Twice Area of triangle (a,p,p1) = " << area1 << endl;

   area1 = area(a, p1, p);
   cout << "Twice Area of triangle (a,p1,p) = " << area1 << endl;

   cout << "/* *************** COLLINEARITY TESTS **************** */\n";
   bool isCollinear = collinear(a, b, c);
   cout << "Are a, b, c collinear? " << isCollinear << endl;

   // NOTE: The next two used to be slow as they involve sqrts!  But
   // these are no longer an issue.

   cout << "Are a, p1, p2 collinear? ";
   cout.flush();
   isCollinear = collinear(a, p1, p2);
   cout << isCollinear << " (1=collinear, 0=noncollinear) \n"
        << "Are a, p1, p collinear?  ";
   isCollinear = collinear(a,p1,p);
   cout << isCollinear << " (1=collinear, 0=noncollinear) \n";

   cout << "/* *************** BETWEEN TESTS **************** */\n";
   a = Point2d(123.4, -5678.9);
   c = Point2d(234.5, 67.8);
   b = midPoint(a, c);
   if (between(a, b, c))
	cout << "CORRECT! " << b << " is between " << a << " and " << c <<endl;
   else
	cout << "ERROR! " << b << " is not between " << a << " and " <<c <<endl;
   if (between(a, a, c))
	cout << "ERROR! " << a << " is not between " << a << " and " <<c <<endl;
   if (between(a, c, c))
	cout << "ERROR! " << c << " is not between " << a << " and " <<c <<endl;

   machine_double eps = -2.0;
   for (int i=0; i<10; i++){
      b = aCenter(a, c, eps+0.11);
      if (between(a, b, c))
	cout << "ERROR! " << c << " is between " << a << " and " <<c <<endl;
   }
   eps = 0.0;
   for (int i=0; i<10; i++){
      b = aCenter(a, c, eps+0.07);
      if (between(a, b, c))
	cout << "ERROR! " << c << " is not between " << a << " and " <<c <<endl;
   }
   eps = 1.0;
   for (int i=0; i<10; i++){
      b = aCenter(a, c, eps+0.8);
      if (between(a, b, c))
	cout << "ERROR! " << c << " is between " << a << " and " <<c <<endl;
   }

  /* ************* UNSUITABLE FOR AUTOMATIC TESTING: */
  /*
   Point2d p0; bool more=true; char ans;
   cout << "THIS demos the flexible form for our point inputs. \n"
	<< "The basic form is (x,y) but you can omit parenthesis or commas" 
	<< endl;
   while (more) {
   	cout << "Pls enter a point (control-C to quit): " << endl;
   	cin >> p0;
   	cout << "You entered: " << p0 << endl;
   }
   */
   /* ****************************** */

   return 0;
} 

