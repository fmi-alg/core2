/* ************************************************
   File: prog4

   Purpose:  Interaction of different I/O parameters, namely
             defRelprec, defAbsprec, defInputDigits and stream
	     output precision.
             
	     
	     
   Usage: 

        % prog4

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog4.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {

 setScientificFormat();
 setDefaultInputDigits(CORE_INFTY);
 Expr X = "1234.567890"; // exact input
 cout << setprecision(6) << X << endl; // prints 1234.57
 cout << setprecision(10) << X << endl; // prints 1234.567890 
 cout << setprecision(100) << X << endl; // prints 1234.5678899
 X.approx(CORE_INFTY, 333); // enough for 100 digits.
 cout << setprecision(100) << X << endl;
 // prints 100 digits: 1234.56789000000000000000000000000000000000
 //     0000000000000000000000000000000000000000000000000000000000

}


