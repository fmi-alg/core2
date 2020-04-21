// Note: this can be VERY slow because of the slow convergence
//	of the series we use!
// This program is to test out library using pi routines
// $Id: pi0.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

int main( int argc, char *argv[] ) {

  // eps is -log_2(accuracy desired)
  
  int eps;
  if (argc > 1) eps = atoi(argv[1]);
  else eps = 10;

  // outputPrec is the number of digits that are known to be accurate
  int outputPrec;
  outputPrec = (int) eps * log(2.0)/log(10.0);

  // Use the following series for pi:
  //	pi/4  = 1 - (1/3) + (1/5) - (1/7) + (1/9) - ...
  //	      = \sum_{i=0} ^ k  ((1/(4i+1)) - (1/(4i+3)))
  //
  // k is the number of terms to compute in the pi series
  int k   =  (int)pow(2,eps);

  std::cout << " Number of bits of accuracy = " << eps << std::endl;
  std::cout << " k = " << k << std::endl;

  double pi = 0;

  for (int i = 0; i< k; i++) {

	pi += 1/double((4*i + 1)*(4*i + 3));
  }
  

  pi *= 8;
  pi.approx(extLong::CORE_posInftyLong, eps);
  std::cout << " Pi = " << std::setprecision(outputPrec) << pi << std::endl;
  return 0;
}
