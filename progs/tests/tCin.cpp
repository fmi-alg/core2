/* tCin.cpp
 * --- testing cin with Core expressions
 *  Since Core 2.1
 *
 * **********************/

#include <iostream>
#include "CORE.h"

using namespace std;

int main(int argc, char** argv)
{
   double x, y, z;

   int option=0; 	// non-interactive
   if (argc>1)
	option=1;	// ignore the actual argument!

   //set_def_output_and_computing_digits(100);
   setDefaultInputDigits(CORE_INFTY);
   set_scientific_output();	// this seems better for controlling precision
   
   if (option == 1) {
      cout<<"=========================" << endl;
      cout << "REMARK: cin will not complain on non numeric inputs! " << endl;
      cout << "Enter two numeric values: " << endl;
      cin >> x ;
      cin >> y;
   } else {
	x = "1/3";	// we set DefaultInputDigits=CORE_INFTY to force BigRat input
	y = 0;
   }

   z = (x+y)/2.0;
   z.approx(100);
   cout << "The average of " << x << " and " << y << " is: " << z << endl;
   cout<<"=========================" << endl;

   z.approx(30);
   setDefaultOutputDigits(40);
   cout << "Approximated to 100 bits : " << z << endl;

   cout << "Testing BF2 Constructors" << endl;

   BigFloat2 bf2[5];
   bf2[0] = BigFloat2(-1,1);
   bf2[1] = BigFloat2(0.0, 10.0);
   bf2[2] = BigFloat2(9.0, 10.0);
   bf2[3] = BigFloat2(9.99999999, 10.0);

   for (int i=0; i<4; i++) {
	cout<<"=========================" << endl;
	cout << "diameter, radius, left, right of " << bf2[i] << " is "
		<< bf2[i].abs_diam() << ", "
		<< bf2[i].abs_radius() << ", "
		<< bf2[i].getLeft() << ", "
		<< bf2[i].getRight() 
		<< endl;
	}
   
   return 0;
} 

