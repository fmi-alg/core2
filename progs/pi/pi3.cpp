/* ***************************************************************
   File: pi3.cc
	Adapted from pi.cc

   Author: Chee Yap and Chen Li (June 2000)
   Since CORE Library Version 1.2
   	$Id: pi3.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#  define CORE_LEVEL 3

#include <fstream>
#include "CORE.h"


double arctan(int x, int a) {
  // compute arctan(1/x) to absolute precision 2^{-a}
  // x is a natural number and x > 1.

  int N = (int) (a / ( 2 * log(float(x))/log(2.0))) + 1;
  // std::cout << " Number of terms needed in arctan, N = " << N << std::endl;

  double at = 1;
  double x2 = x * x;
  for (int i=1; i<=N; i++) {
    if (i % 2 == 1) 
      at = x2 * at - 1 / double(2 * i + 1);
    else
      at = x2 * at + 1 / double(2 * i + 1);
  }
  
  at /= pow (x2, N) * x;
  
  at.approx(CORE_posInfty, a);
  return at;
}

int main( int argc, char *argv[] ) {

  /* ***************************************************************************
  COMMAND LINE ARGUMENTS
  *************************************************************************** */
  int eps = 54; 	// Number of bits of absolute precision desired
			// default to 54 bits (= machine double precision)
  int DOsqrt = 1;		// a flag: defaults to 1
			// (i.e., compute sqrt)
  int DOvalidate = 1;	// a flag to validate Pi: defaults to 1
			// (i.e., validate to 250 digits)

  if (argc > 1) eps = atoi(argv[1]);	
  if (argc > 2) DOsqrt = atoi(argv[2]);
  if (argc > 3) {
     DOvalidate = atoi(argv[3]);	
     if ((DOvalidate < 0) || (DOvalidate >2)) {
	std::cout << " !! Third argument in error, defaults to 1\n";
	DOvalidate = 1;
     };
  };

std::cout << "DOsqrt = " << DOsqrt << ";  DOvalidate = " << DOvalidate << std::endl;

  /* ***************************************************************************
  COMPUTING Pi
  *************************************************************************** */
  // Translates eps (in bits) to outputPrec (in digits)

  int outputPrec;	// desired precision in digits
  outputPrec = (int) (eps * log(2.0)/log(10.0));
  std::cout << " Output precision is " << outputPrec << " digits \n";

  // Computing Auxilliary Values
  double t1 = arctan(5, eps + 6);
  // debugging output:
  // std::cout << std::setprecision(outputPrec+1) << "arctan(1/5)   = " << t1 << std::endl;
  double t2 = arctan(239, eps + 4);
  // debugging output:
  // std::cout << std::setprecision(outputPrec+1) << "arctan(1/239) = " << t2 << std::endl;

  double pi = (4 * t1 - t2) * 4;
  pi.approx(CORE_posInfty, eps);

  // Output of Pi
  std::cout << " Pi = " << std::setprecision(outputPrec+1) << pi << std::endl;
  //   Note: setprecision in C++ is actually "width" (=number of characters
  //   in output.  So we use "outputPrec + 1" to account for the decimal point.

  /* ***************************************************************************
  AUTOMATIC CHECK that
	(1) our internal value of Pi
  	(2) our output value of Pi
  are both correct to 250 (or 2000) digits
  *************************************************************************** */

  // Reading in digits of Pi from Files
  int prec = 0;				// bit precision for the comparison
  std::ifstream from;			// input stream from file
  if (DOvalidate == 0) {		// no validation 
    prec = 1;
  }
  if (DOvalidate == 1) {
    prec = core_min(eps, 830);	// 830 bits = 250 digits.
    from.open("inputs/PI250", std::ios::in);	// read 250 digits of Pi from file
  }
  if (DOvalidate == 2) {
    prec = core_min(eps, 6644);	// 6644 bits = 2000 digits
    from.open("inputs/PI2000", std::ios::in);	// read 2000 digits of Pi from file
  }

  if (DOvalidate > 0) {		// validation needed
		std::string piStr; 
    char c;
    while (from.get(c))
	if ((c >= '0' && c <= '9') || (c=='.')) 
		piStr += c;
    if (!from.eof()) std::cout << "!! Error in reading from PI250 \n";
    from.close();
    double bigPi(piStr.c_str());	// bigPi is the value of Pi from file

    // debug:
    //  std::cout << " bigPi = " << std::setprecision(outputPrec + 1) << bigPi << std::endl;

    // CHECKING OUTPUT VALUE OF Pi:
    std::ostringstream ost;
    ost << std::setprecision(outputPrec+1) << pi << std::endl;
    piStr = ost.str();

    // Need to take the min of outputPrec and the number of digits in bigPi
    int minPrec = 0;
    if (DOvalidate == 1)
	minPrec = core_min(outputPrec, 250);
    if (DOvalidate == 2) 
	minPrec = core_min(outputPrec, 2000);

double ee = pow(Expr(10), -minPrec+4);
ee.approx(eps);
std::cout << "pow(Expr(10), -minPrec+4)) = " << std::setprecision(outputPrec) << ee << std::endl;


    if ( fabs(Expr(piStr.c_str()) - bigPi) <= pow(Expr(10), -minPrec+4))
       std::cout << " >> Output value of Pi verified to "<< minPrec << " digits\n";
    else
       std::cout << " !! Output value of Pi INCORRECT to " << minPrec << " digits! \n";

    // CHECKING INTERNAL VALUE of Pi:
    if ( fabs(pi - bigPi) <= (Expr)BigFloat::exp2(- prec +1))
       std::cout << " >> Internal value of Pi verified up to "
	       << prec << " bits\n";
    else
       std::cout << " !! Internal value of Pi INCORRECT to "
	       << prec << " bits! \n";

  };

  /* ***************************************************************************
  COMPUTING THE SQUARE ROOT of Pi
	to (eps - 2) bits of absolute precision
  *************************************************************************** */
  if (DOsqrt == 0) return 0;	// do not compute sqrt(Pi)

  // Here is sqrt(Pi) to 100 digits from Knuth:
  Expr SQRTPI="1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665";

  double sPi = sqrt(pi);
  sPi.approx(CORE_posInfty, eps + 2);
  std::cout << " sqrt(Pi) = " << std::setprecision(outputPrec-1) << sPi << std::endl;

  /* ***************************************************************************
  AUTOMATIC CHECK that the internal value of computed sqrt(Pi) is correct
  up to the first 100 digits.
  *************************************************************************** */
  prec = core_min(eps-1, 332);	// 332 bits == 100 digits
  double d3 = fabs(sPi - SQRTPI);
  double tmp = pow(Expr(10), -(int) (prec * log(2.0)/log(10.0)) );
  if ( d3 <= tmp )
     std::cout << " >> Internal value of sqrt(Pi) verified to " << prec << " bits\n";
  else
     std::cout << " !! Internal value of sqrt(Pi) INCORRECT to " << prec << " bits! \n";
  
  return 0;
}

