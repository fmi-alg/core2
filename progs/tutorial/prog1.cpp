/* ************************************************
   File: prog1

   Purpose:  To show the setting of output format
             which can be either positional or scientific,
	     and the manipulation of output precision.
	     
   Usage: 

        % prog1

   Author: Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog1.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {

 Expr e1 = 12.34;          // constructor from C++ literals
 Expr e = "1234.567890";  // constructor from string
 // The precision for reading inputs is controlled by \definput
 // This value is initialized to be 16 (digits).
 cout << e << endl;
 // prints 1234.57 as the output precision defaults to 6.
 cout << setprecision(10) << e << endl; // prints 1234.567890
 cout << setprecision(11) << e << endl; // prints 1234.5678900
 setScientificFormat();
 cout << setprecision(6) << e << endl;  // prints 1.23457e+3 
}
