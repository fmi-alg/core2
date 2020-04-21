/* ***************************************************************
   File: apparentPi.cpp

   Purpose: To compute Pi using some cheap approximations

	We use this formula:

	3 * log(640320) 
	---------------
	   sqrt(163)

	which is less than Pi by less than 10^{-15}.

	On a 15-digit pocket calculator, it would
	be indistinguishable from Pi.

   Usage:
	  % apparentPi 

   Author: Chee Yap (Apr 2010)
   Since CORE Library Version 2.0
   	$Id: apparentPi.cpp,v 1.2 2010/05/07 14:53:22 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include <fstream>
#include <sstream>
#include "CORE.h"

using namespace std;

int main( int argc, char *argv[] ) {

  eps = 0.000000000000001;	//15 digits
  int outputPrec = (int) (eps * log(2.0)/log(10.0)); // desired precision in digits
	// NOTE: log(x) is machine approximation to natural logarithms
	// The expression version is called Log(x).
  cout << " Output precision is " << outputPrec << " digits \n";

  double pi = 3* Log(640320)/sqrt(Expr(163));
  pi.approx(CORE_posInfty, eps);

  // Output of Pi
  cout << " Pi = " << setprecision(outputPrec+1) << pi << endl;
  //   Note: setprecision in C++ is actually "width" (=number of characters
  //   in output.  So we use "outputPrec + 1" to account for the decimal point.
  
  return 0;
}
