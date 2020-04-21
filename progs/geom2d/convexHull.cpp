#ifndef CORE_LEVEL
# define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

int main()
{
   int index;
   bool isOnHull[10][10];  
       //points p[i], p[j] are on the hull if isOnHull[i][j] is true
   int temp, i, j, test, count;


   std::cout << "\nThe program will test which of the following points "
        << "are on the convex hull" << std::endl;

   Point2d a0(0, 0), a1(10, 1), a2(8, 2), a3(6, 5), a4(10, 10),
            a5(-5, -5), a6(0, 9), a7(-1, 7), a8(5, -6), a9(8, 0);

   Point2d p[10] = {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9};
 
   bool isOn[10]; //isOn[i] is true means p[i] is on the convex hull

   for (i = 0; i < 10; i++)
      isOn[i] = false; 

   for (i=0; i<10; i++)
      for (j=0; j<10; j++)
         isOnHull[i][j] = true;

   for (index = 0; index < 10; ++index) {
       if (index % 3 == 0)
          std::cout << std::endl;
       std::cout << "p[" << index << "] = " << p[index] << "\t";
   }

   std::cout << "\n\n Bruteforce Algorithm:" << std::endl;
   std::cout << "For each pair of points, if all the other points have the " 
        << "same orientation, then this pair of points are on the convex hull"
        << std::endl; 

   for (i = 0; i < 10; i++)
     for (j = i+1; j < 10; j++) {
        if (j == 9) 
           test = (i + 1) % 9;
        else 
           test = j+1;
        
        //temp will hold the standard orientation for comparison
        temp = orientation2d(p[i], p[j], p[test]);
        if (temp == 0) {     //three points are collinear
           for (index = test+1; index < 10; index++)
               if (orientation2d(p[i], p[j], p[index]) != 0) {
                   temp = orientation2d(p[i], p[j], p[index]);
                   break;
               }
        }
        for (int k = 0; k < 10; k++)
            if (k != i && k != j && k != test)
              if(temp != orientation2d(p[i],p[j],p[k])) {
                isOnHull[i][j] = false;
                break;
              }
     }

   std::cout << "\nThe following points are on the convex hull" << std::endl;

   for (i = 0; i < 10; i++) {
     for (j = i+1; j < 10; j++)
       if (isOnHull[i][j] == true) {
          isOn[i] = true;
          isOn[j] = true;
       }
   } 

   count = 0;

   for (i=0; i<10; i++) {
      if (isOn[i] == true) {
         std::cout << "p[" << i << "] = " << p[i] << "   ";
         count++;
         if (count % 3 == 0)
            std::cout << std::endl;
      }
   } 
   std::cout << std::endl;
   return 0;
}   
