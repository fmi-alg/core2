/* ************************************************
   File: schmit.cpp

   Purpose:  

   Usage:
        % bench [number of iterations]

   Author: Sylvain Pion, August 2002.

   Susanne Schmitt wrote:

   >Hello,
   >
   >there seems to be a bug in the implementation.
   >For the polynomial
   >  Q = 544*x^4 - 224*x^3 + 80*x^2 - 62
   >I can not compute the real roots:
   >
   >  BigInt cQ[] = {-62, 0, 80,-224, 544}; 
   >  Polynomial<BigInt> Q(4,cQ);
   >  Expr rQ(Q,2);
   >
   >gives the error message:
   >
   >  test1: /usr/local/gcc-3.3/core_v1.7/inc/CORE/BigFloat.h:226: int 
   >CORE::BigFloat::sign() const: Assertion `!(isZeroIn())' failed.
   >  Aborted
   >
   >With P(x) = Q(x+1) the polynomial gets rid of the
   >zero coefficient:
   >
   >  P = 544*x^4 + 1952*x^3 + 2672*x^2 + 1664*x + 338
   >
   >and the test program works.
   >
   >
   >Regards,
   >Susanne Schmitt
   >MPI fuer Informatik,
   >Saarbruecken, Germany
   >
   
   Since Core Library 1.5
   $Id: schmitt.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */        

#include <iostream>
#include <cmath>

#define CORE_LEVEL 4
#include "CORE.h"
using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main(int argc, char **argv)
{

  PolyNT Q = "544*x^4 - 224*x^3 + 80*x^2 - 62";

  Expr rQ(Q,2);

  cout.precision(300);
  cout << "Second root of Q is " << rQ.approx(1000) << endl;

  PolyNT P = "544*x^4 + 1952*x^3 + 2672*x^2 + 1664*x + 338";

  Expr rP(P,2);

  cout << "Second root of P is " << rP << endl;

  if (rQ - rP == 1)
	  cout << "CORRECT!" << endl;
  else
	  cout << "ERROR!!!!" << endl;

  return 0;
}
