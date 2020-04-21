/* ************************************************
   File: invalid.cpp

   Purpose:  To test invalid input behavior

   	This program was originally from Stefan Funke and Pion.
   	In Core 1.5, when we construct an Expr (see Expr e below)
	from an invalid double (either infinite or NaN), the
	system will get into an infinite loop.

	We extended the program to test other invalid values:

	1)	constructing a leaf value from an invalid machine double
	2)	constructing a leaf value from an invalid machine float
	3)	dividing by zero  -- the CORE code was done earlier by Pion
	4)	sqrt of a negative number,
	5)	constructing a leaf value from an invalid Algebraic Number

	Although the default behavior of Core is to abort when an invalid
	expression is constructed, you can turn off this automatic
	abortion by setting a flag 

   Usage:
        % invalid 

   Author: 
   	    Chee and Zilin.

   Date: May 2003.

   Since Core Library Version 1.6
   $Id: invalid.cpp,v 1.2 2010/01/15 17:28:00 exact Exp $
 ************************************************ */        

#define CORE_LEVEL 3
#include "CORE/CORE.h"

using namespace std;

int main(int argc, char **argv)
{

  AbortFlag = false;	// turn off automatic abortion on invalid expression

  double zero = 0.0;
  double infty = 1.0/zero;
  double NaN = 0.0/zero;

  float fzero = 0.0;
  float finfty = 1.0/fzero;
  float fNaN = 0.0/fzero;

  Expr e(infty);
  cout << "Invalid flag for double Infinity: " << InvalidFlag << endl;

  Expr f(NaN);
  cout << "Invalid flag for double NaN: " << InvalidFlag << endl;

  Expr g(finfty);
  cout << "Invalid flag for float Infinity: "<< InvalidFlag << endl;

  Expr h(fNaN);
  cout << "Invalid flag for float NaN: " << InvalidFlag << endl;

  Expr i = Expr(1.1); i /= Expr(0);
  cout << "Invalid flag for Dividing by Zero: " << InvalidFlag << endl;

  Expr j = Expr(1.1)/(Expr(4) - Expr(4));
  cout << "Invalid flag for Dividing by Zero: " << InvalidFlag << endl;

  Expr k = sqrt(Expr(-1));
  cout << "Invalid flag for Squareroot of a negative number: " << InvalidFlag << endl;

  return 0;
}
