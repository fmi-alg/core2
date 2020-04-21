/***********************************************
 * file: testLine.cc
 *
 * author: Yaping Yuan
 * Since CORE Library Version 1.2
 * $Id: testLine.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 **********************************************/

#ifndef CORE_LEVEL
# define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

int main()
{

   /* triangle((0,0), (10, sqrt(7)), (5, 5 * sqrt(3)).
      The bisectors of each side of a triangle
      should intersect at the same point at level 3,
      but not necessarily at level 1

        NOTE: the sqrt here will be a CORE routine at
        level 3, but a math.h routine at level 1.
    */

   double x = sqrt( double(7) );        // we need this to avoid the
                                        // invocation of sqrt in math.h
                                        // in level 3.
   double f = 5;
   double y = f * sqrt(double(3)); 
   double eps = 123456789;              // make small perturbation
   eps = 1/eps;

   Point2d a(0.0, 0.0), b(10, x), c(f, y);      // triangle abc
   Point2d C(f, y + eps);               // C is a perturbation of c
 
   Point2d p1, p2, p3, P3;

   std::cout << "Point a = " << a << std::endl;
   std::cout << "Point b = " << b << std::endl;
   std::cout << "Point c = " << c << std::endl;
   std::cout << "Point C = " << C << std::endl;   // perturbation of c

   Line2d g1 = p_bisector(a, b);
   Line2d g2 = p_bisector(a, c);
   Line2d g3 = p_bisector(b, c);
   Line2d G3 = p_bisector(b, C);        // perturbation of g3

   std::cout << "In triangle(a, b, c), \n"
        << "   g1 is the bisector of line a-b :" << g1 << std::endl
        << "   g2 is the bisector of line a-c :" << g2 << std::endl
        << "   g3 is the bisector of line b-c :" << g3 << std::endl; 
   std::cout << "but G3 is a perturbation of g3    :" << G3 << std::endl;

   p1 = *(Point2d *)g1.intersection(g2);
   std::cout << "  g1 and g2 intersect at p1 = " << p1 << std::endl;
   p2 = *(Point2d *)g2.intersection(g3);
   std::cout << "  g2 and g3 intersect at p2 = " << p2 << std::endl;
   p3 = *(Point2d *)g3.intersection(g1);   
   std::cout << "  g3 and g1 intersect at p3 = " << p3 << std::endl;
   P3 = *(Point2d *)G3.intersection(g1);   
   std::cout << "  G3 and g1 intersects at P3 = " << P3 << std::endl;

   if (p1 == p2) 
      std::cout << "p1 = p2 (CORRECT!) \n";
   else
      std::cout << "p1 != p2 (ERROR!) \n";
   if (p1 == p3)
      std::cout << "p1 = p3 (CORRECT!) \n";
   else
      std::cout << "p1 != p3 (ERROR!) \n";
   if (p2 == p3)
      std::cout << "p2 = p3 (CORRECT!) \n";
   else
      std::cout << "p2 != p3 (ERROR!) \n";
   if (p2 == P3)
      std::cout << "p2 = P3 (ERROR!) \n";
   else
      std::cout << "p2 != P3 (CORRECT!) \n";

  return 0;
}

