/* ************************************************
   File: prog9.cpp

   NOTE: this program is no longer supported in Core2.

   Purpose:  Output of Different Level 2 numbers. Note
             the approximation of BigRat to BigFloat when
	     'B' is initialized with R.
	     
   Usage: 

        % prog9

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog9.cpp,v 1.2 2010/05/18 10:34:13 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;


int main(int argc , char ** argv) {

	setDefaultRelPrecision(10);
	setDefaultAbsPrecision(CORE_INFTY);

 BigRat R(1, 3);
 BigFloat B(R); 
 BigInt   I = 1234567890;
 cout.precision(20); // set output precision to 8
 Real Q = R; 
 Real X = B; 
 Real Z = I; 
 cout << R << endl; 
 // prints:  1/3 
 cout << Q << endl; 
 if (Q*3 == 1) cout << "Q = 1/3" << endl;
 else cout << "Q != 1/3" << endl;
 // prints:  0.3333333
 cout << X << endl; 
 // prints:  0.3333333
 cout << Z << endl; 
 // prints:  1.2345679e+9


 defInputDigits = CORE_INFTY;
 Real QQ = R;
 cout << QQ << endl;
 Real QQQ(R);
 cout << QQQ << endl;
 // prints?
 //
 
 Expr e = "1/3";
 cout << "Expr e = 1/3: prints " << e << endl;
 if (3*e == 1) cout << "e = 1/3" << endl;
 else cout << "e != 1/3" << endl;


}


