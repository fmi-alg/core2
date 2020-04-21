/* ***************************************************************
   File: demoPi.cpp

   Purpose: Simple illustration of how we can compute Pi using
   			arccos(x) + arccos(-x)
	for any x such that |x|<1.

   Known Bugs:
   	The output is 0 when |x|=1.

   Usage:
	  % demoPi [x = 0.6] [outPrec = 30]

	where 

	-- [x] is the argument to arccos(x), default value is 0.6

	-- [outPrec] is the absolute precision in number of digits
		desired in the output, default value is 100

   Author: Chee Yap and Dongguk Kim (Dec 2003)
   Since CORE Library Version 1.6
   	$$
 *************************************************************** */

#include <fstream> 
#include "MultiPrecision.h"      
#define CORE_LEVEL 4
#include "CORE/CORE.h"
using namespace std;

int main(int argc , char ** argv) {
  int outputPrec;
  
  int eps;
  Expr val, funVal;
  
  // PROCESS INPUTS =================================================
  val = Expr("0.6", 10, CORE_posInfty);
  if (argc < 2) 
     cout << "Usage:  demoPi [val = 0.6] [outputPrec = 30]" << endl;
  else
     val = Expr(argv[1], 10, CORE_posInfty);
  outputPrec = 30;
  if (argc > 2) outputPrec = atoi(argv[2]); 

  cerr << "Computing Pi as ArcCos(" << val <<
	  ") + ArcCos(" << (-val) << ")"<< endl;

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  // cerr << "eps = " << eps << endl;

  // COMPUTE Pi ======================================================
  setDefaultAbsPrecision(eps);
  funVal = acos(val) + acos(-val);
  funVal.approx(CORE_posInfty, eps);

  if (fabs(funVal) < 0.1)
        cout.setf(ios::scientific, ios::floatfield);
  else
        cout.setf(ios::fixed, ios::floatfield);
    
  cout << "Approximate value of Pi is to " << outputPrec << " digits is :\n"
	<< setprecision(outputPrec) << funVal << endl;
}
