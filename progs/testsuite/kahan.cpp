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
   Core Library, $Id: kahan.cpp,v 1.2 2010/05/28 00:04:59 exact Exp $
 ************************************************ */  

#include <CORE/CORE.h>
#include <iostream>

using namespace std;

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
	if (x != 0) cout << "x = " << x << endl;
	return x;
  }
  double G(double x) {
	double qx =  Q(x);
	if (qx != 0) 
		cout << "            Q(x) not 0! = " << qx << endl;
	return (F(qx * qx));
  }

int main(void) 
{ 
	double g;
	int correct=0;
	int wrong=0;

	for (int n=5000; n<6010; ++n){
	  g = G(n);
	  if (g == 1) correct++;
	  else wrong++;
	  //cout << "  n = " << n << ";   G(n) = " <<  g  <<
	  //	((g == 1)? " Correct! \n" : "  Incorrect! \n");
	}
	cout << "Testing Kahan's example" << endl;
	if (wrong>0) 
		cout <<"ERROR!!! ";
	else 
		cout <<"CORRECT!!! ";
	cout << wrong << " wrong answers and "
		<< correct << " right answers!" << endl;
	return 0;
}
