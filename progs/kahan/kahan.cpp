/* ************************************************
   File: kahan.cc

   Purpose: Kahan posed a challenge for Core Library,
	to compute a certain function G(n) and to compare its
	output to 1 for various values of n.  It turns out
	that G(n) is identically equal to 1.  In CORE_LEVEL 1,
	errors in machine floating point ensures that 
	this test gives the wrong answer.  But Core Library
	has no problems with this function.

   Usage:
        % kahan

   Author: Chee Yap
   Core Library, $Id: kahan.cpp,v 1.2 2010/06/11 17:49:29 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include "CORE.h"

  double F(double z) {
	if (z == 0) return 1;
	double zz = z*z;
	double out = (1 + (z/2) + (zz/6) + ((z*zz)/24));
	return (out);
  }
  inline double abs(double x) {
	return (x > 0)? (x) : (- (x));
  }
  double Q(double y) {
	double yy = y*y;
	double x = (sqrt(yy +1) - y) - (1/(y+sqrt(yy +1)));
	if (x != 0) std::cout << "x = " << x << std::endl;
	return x;
  }
  double G(double x) {
	double qx =  Q(x);
	if (qx != 0) 
	   std::cout << "            Q(x) not 0! = " << qx << std::endl;
	return (F(qx * qx));
  }

int main(void) 
{ 
	double g;
     	//setFpFilterFlag(false);
	for (int n=5000; n<5010; ++n){
	  g = G(n);
	  std::cout << "  n = " << n << ";   G(n) = " <<  g  <<
	  	((g == 1)? " Correct! \n" : "  Incorrect! \n");
	}
	return 0;
}
