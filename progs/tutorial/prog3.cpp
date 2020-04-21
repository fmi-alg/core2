/* ************************************************
   File: prog3

   Purpose:  Interaction of different I/O parameters, namely
             defRelprec, defAbsprec, defInputDigits and stream
	     output precision.
             
	     
	     
   Usage: 

        % prog3

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog3.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {

 setScientificFormat();
 setDefaultInputDigits(2); // defInputDigits = 2
 Expr X = "1234.567890";
 cout << setprecision(6) << X << endl;  // prints .123457e+4
 cout << setprecision(10) << X << endl;  // prints .1234567871e+4
 cout << setprecision(100) << X << endl;  // prints .123456787109375000e+4

}


