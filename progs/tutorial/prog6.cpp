/* ************************************************
   File: prog5

   Purpose:  Effect of different ways of initializing
             on the value of the variable being initialized,
	     i.e, exact or approximate initialization.
	     
	     
   Usage: 

        % prog5

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog6.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;


int main(int argc , char ** argv) {

 double a = 1.0/3; // the value of a is an approximation to 1/3
 double b = 1.3;  // the value of b is also approximate
 // To input the exact value of 1/3, do this instead: 
 double c = BigRat(1, 3); // sure way to get exact value of 1/3
 double d = "1/3"; // sure way to get exact value of 1/3
 double e = "1.3"; // the global defInputDigits should be
		     // +\infty in order for e to be exact.

 if(3*a != 1)
   cout << "a =  " << a << ", 3 * a != 1: since a is a machine double it is an approximation to 1/3" <<  endl;

 if(3*c == 1)
   cout << "c =  " << c << ", 3 * c == 1: since c is a BigRat it is precisely 1/3" <<  endl;

 if(3*d == 1)
   cout << "d =  " << d << ", 3 * d == 1: since d is precisely 1/3" <<  endl;


}


