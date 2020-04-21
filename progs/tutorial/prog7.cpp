/* ************************************************
   File: prog6

   Purpose:  Interplay between ostream precision and the
             composite precision on BigFloat numbers.
	     
	     
   Usage: 

        % prog6

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog7.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;


int main(int argc , char ** argv) {

 double q = BigRat(1, 3); 
 setDefaultAbsPrecision(67); // about 20 digits 
 cout << "q = " << setprecision(10) << q << ", in 10 digits" << endl;
   // output: q = 0.33333333, in 10 digits
 cout.precision(30); // or use setDefaultOutputDigits(30, cout), 
                     // default to output 30 digits.
 cout << "q = " << q << ", after setting Default Output to 30 digits" << endl;
  // output: q = 0.33333333333333333333, in positional notation.

}


