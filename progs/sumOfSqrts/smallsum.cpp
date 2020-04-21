/* **************************************
   File: smallsum.cc

   Purpose: 
	To demonstrate an interesting technique
	to generate arbitrarily small sum of
	squareroots in a systematic way.

	The idea is this: let delta(0, x) = sqrt(x).
	Now recursively, define

		delta(i+1,x)  = delta(i, x+1) - delta(i, x).

	CLAIM: for any x >= 1,
		| delta(i+1, x)|  <  | delta(i, x)|.

	E.g.,
	   delta(20, 17) = -0.0000000000015458648150...
	   delta(21, 17) =  0.0000000000008113604197...

   Usage:

	% smallsum [i=20] [x=17] [relprec=100]

   BUG:
   	For i greater than 29, the old Core1 library has
	an overflow in the rootbounds.  This is fixed in Core2.

   Author: Zilin Du and Chee Yap (Nov 25, 2002)

   Since CORE Library Version 1.6
   $Id: smallsum.cpp,v 1.3 2010/01/06 17:09:36 exact Exp $
 ************************************** */


#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE.h"
using namespace std;

int main(int argc, char** argv) {

  int d;	// depth
  int x;	// base number
  int relprec;  // the relative precision for evaluation

  if (argc > 1) d = atoi(argv[1]);
  else d=20;
  if (argc > 2) x = atoi(argv[2]);
  else x = 17;
  if (argc > 3) relprec = atoi(argv[3]);
  else relprec = 100;
 
  setScientificFormat();		// easier to see the progression 
  setDefaultRelPrecision(relprec);	// relative precision in bits
  cout.precision(10+(relprec)/3);  	// print precision in digits

  for (int k=1; k<=d; k++) {

     double delta = sqrt(double(x+k));
     BigInt binomial = 1;

     for (int i=1 ; i<=k; i++) {
	binomial = - (binomial * (k-i+1))/i;
	delta += binomial * sqrt(double(x+k-i)); 
     }

     cout << "delta(" << k << ", " << x << ") = " << delta << endl;
  }
  
  return 0;
}

