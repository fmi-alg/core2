#ifndef CORE_LEVEL
# define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

int main() 
{

   double x1 = 2.0, x2 = 3.0;
   Point2d p1(2, 0), p2(4, 2), p3(2, 4), p4(0, 2);

   Circle2d c1(p1, p2, p3);

   std::cout << "\n Circle c1: " << c1 << std::endl;

   std::cout << "This program tests if given points are on the circle \n"
        << "   Points test[0] - test[3] and p4 should be on the circle \n"
        << "   while the others are not" << std::endl;

   if (c1.contains(p4))
      std::cout << "p4(0, 2) is on the circle (CORRECT!)" << std::endl;
   else
      std::cout << "p4(0, 2) is not on the circle (ERROR!)" << std::endl;

   std::cout << "sqrt(2.0) = " << sqrt(x1) << std::endl;

      //test[0]-test[3] should be on the circle
      //test[4]-test[7] should not be on the circle

   Point2d test0(2.0+sqrt(x2), 1), test1(2.0+sqrt(x1), 2.0+sqrt(x1)), 
            test2(2.0-sqrt(x1), 2.0+sqrt(x1)), test3(2.0-sqrt(x2), 1),
            test4(5, 6), test5(-2, 3), test6(-1, -2), test7(2, -3);
   
   Point2d test[8] = {test0, test1, test2, test3, test4, test5, test6, test7};
  
   for (int i = 0; i < 4; i++)
      if (c1.contains(test[i]))
         std::cout << "point["<<i<<"] "<< test[i] 
                << " is on the circle (CORRECT!)" << std::endl;
      else
         std::cout << "point["<<i<<"] "<< test[i]
                 << " is not on the circle (ERROR!)" << std::endl;

   for (int j = 4; j < 8; j++)
      if (c1.contains(test[j]))
         std::cout << "point["<<j<<"] "<< test[j]
                 << " is on the circle (ERROR!)" << std::endl;
      else
         std::cout << "point["<<j<<"] "<< test[j]
                 << " is not on the circle (CORRECT!)" << std::endl;
  return 0;
} 
