/* ************************************************
   File: prog12

   Purpose:  Showing the effects of shared subexpressions.
	     See Section 8 (Efficiency Issues) in Tutorial. 
	     We compare the expressions

	     	E1 = sqrt(x) + sqrt(y)
		E2 = sqrt(x + y + 2 sqrt(xy))
	
	     for various values of x and y.

	     If the program (prog12) is given any arguments at all,
	     it will do the comparison with shared subexpressions;
	     otherwise not.

   Usage: 

        % time prog12 1		-- shows timing for shared subexpressions
        % time prog12 		-- shows timing for non-shared subexpressions

   Results:

	% time prog12 1
	Shared Subexpressions: Number of errors = 0
	real    0m0.382s
	user    0m0.390s
	sys     0m0.000s

	% time prog12
	Non-shared Subexpressions: Number of errors = 0
	real    0m0.437s
	user    0m0.450s
	sys     0m0.010s

	This shows a >10% speedup.  The improvement can made
	more dramatic by sharing more subexpressions.

   Author: Chee Yap (March 2004)

   Since Core Library Version 1.7
   $Id: prog12.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;


int main(int argc , char ** argv) {

  double x, y, sqrtx, sqrty, E1, E2; 
  int err = 0;

  if (argc >1) {
    for (int i = 1000; i<1200; i++) {
	x = 2*i; y = i^2;
  	sqrtx = sqrt(x);  sqrty = sqrt(y);
  	E1 = sqrtx + sqrty;
  	E2 = sqrt(x + y + 2*sqrtx * sqrty); 
  	if (E1 != E2) {err++; cout << "ERROR!" << endl;}
    }
    cout << "Shared Subexpressions: Number of errors = " << err << endl;
  } else {
    for (int i = 1000; i<1200; i++) {
	x = 2*i; y = i^2;
  	E1 = sqrt(x) + sqrt(y);
  	E2 = sqrt(x + y + 2*sqrt(x * y)); 
  	if (E1 != E2) {err++; cout << "ERROR!" << endl;}
    }
    cout << "Non-shared Subexpressions: Number of errors = " << err << endl;
  }
  return 0;
}


