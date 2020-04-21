/* ***************************************************************
   File: pi.cpp

   Purpose: To compute Pi and sqrt(Pi) to any desired precision using
        Machin's series.

   Usage:
	  % pi [optPrec] [DOsqrt] [DOvalidate]

	where the arguments (all optional) are

	-- [optPrec] is the absolute precision in number of bits.
		Default is [optPrec] = 54 (as in machine double)

	-- [DOsqrt] is a flag (default to 1).
		A non-zero value will cause the program to compute sqrt{Pi),
		validated to 100 digits.

	-- [DOvalidate] is a flag (default to 1) 
		[DOvalidate]=0 means no validation of Pi.  
		[DOvalidate]=1 means validate Pi up to 250 digits.
		[DOvalidate]=2 means validate Pi up to 2000 Digits.  

	EXAMPLE:  To compute Pi to 2000 digits, set [optPrec] to
	6644 (= 2000 * log_2(10)).  If you also want to check the 2000
	computed digits, but to skip the sqrt(Pi) computation, you may type:

	  % pi 6644 0 2

	WARNING: there are 2 sources for slowness in running this program.
	(1) [DOvalidate]=2 is slow just because of the need to read 2000
		digits from a file and converting them into internal format.
		We cannot do much about this.  The actual time to compute
		Pi to 2000 digits is no problem at all (you will see the
		output in a flash)
	(2) [DOsqrt]=1 and [optPrec] is large.

   Notes:
	The implementation is basically straightforward, and illustrates
	the power of the Core Library's features.  It is clear that most
	of the common functions of analysis can be similarly implemented
	to achieve the guaranteed accuracy feature (see below).

	GUARANTEED OUTPUT:  The specified number of bits is translated into
	an equivalent number of digits for output.  All but the last printed
	digits are guaranteed to be correct.  The last digit can be off 
	by +/-1.  Thus, if an output that terminates with ...1999, the
	actual digits might actually be ...2000 or ...1998.
	
	SQUARE ROOT of Pi: This constant is useful, e.g., in computing
	the error function (erf).  We compute sqrt(Pi) to ([optPrec] - 2) bits
	of absolute precision.  But this computation can be turned off
	if the optional second argument is 0.

	SELF-VALIDATION:
	This program checks that the INTERNAL VALUE of computed Pi
	and the OUTPUT of the computed Pi are each correct up to
	N digits.  Here, N = 0, 250, 2000 (respectively) when the flag
	[DOvalidate] = 0, 1, 2.

	It also checks that the internal value of computed sqrt(Pi),
	when computed, is correct up to 100 digits.

   Author: Chee Yap and Chen Li (June 2000)
   Since CORE Library Version 1.2
   	$Id: pi.cpp,v 1.2 2006/08/09 14:50:28 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include <fstream>
#include <sstream>
#include "CORE.h"

using namespace std;

double arctan(int x, int a) {
  // compute arctan(1/x) to absolute precision 2^{-a}
  // x is a natural number and x > 1.

  int N = (int) (a / ( 2 * log(float(x))/log(2.0))) + 1;
  // cout << " Number of terms needed in arctan, N = " << N << endl;

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

  setFpFilterFlag(false);

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
      cout << " !! Third argument in error, defaults to 1\n";
      DOvalidate = 1;
    };
  };

  // debug
  //eps = 9000;
  //DOsqrt = 0;
  //DOvalidate = 0;

  cout << "DOsqrt = " << DOsqrt << ";  DOvalidate = " 
	    << DOvalidate << endl;

  /* ***************************************************************************
  COMPUTING Pi
  *************************************************************************** */
  // Translates eps (in bits) to outputPrec (in digits)

  int outputPrec;	// desired precision in digits
  outputPrec = (int) (eps * log(2.0)/log(10.0));
  cout << " Output precision is " << outputPrec << " digits \n";

  // Computing Auxilliary Values
  double t1 = arctan(5, eps + 6);
  //debugging output:
  //cout << setprecision(outputPrec+1) << "arctan(1/5)   = " 
  //          << t1 << endl;
  double t2 = arctan(239, eps + 4);
  // debugging output:
  // cout << setprecision(outputPrec+1) << "arctan(1/239) = " 
  //           << t2 << endl;

  double pi = (4 * t1 - t2) * 4;
  pi.approx(CORE_posInfty, eps);

  // Output of Pi
  cout << " Pi = " << setprecision(outputPrec+1) << pi << endl;
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
  ifstream from;			// input stream from file
  if (DOvalidate == 0) {		// no validation 
    prec = 1;
  }
  if (DOvalidate == 1) {
    prec = std::min(eps, 830);	// 830 bits = 250 digits.

    from.open("inputs/PI250", ios::in); // read 250 digits of Pi from file
  }
  if (DOvalidate == 2) {
    prec = std::min(eps, 6644);	// 6644 bits = 2000 digits
    from.open("inputs/PI2000", ios::in);// read 2000 digits of Pi from file
  }

  if (DOvalidate > 0) {		// validation needed
    string piStr; 
    char c;
    while (from.get(c)) {
      if ((c >= '0' && c <= '9') || (c=='.')) 
	piStr += c;
    }
    if (!from.eof()) cout << "!! Error in reading from PI250 \n";
    from.close();
    double bigPi(piStr.c_str());	// bigPi is the value of Pi from file

    // debug:
    // cout << " bigPi = " << setprecision(outputPrec + 1) 
    //           << bigPi << endl;

    // CHECKING OUTPUT VALUE OF Pi:
    ostringstream ost;
    ost << setprecision(outputPrec+1) << pi << endl;
    piStr = ost.str();

    // Need to take the min of outputPrec and the number of digits in bigPi
    int minPrec = 0;
    if (DOvalidate == 1)
	minPrec = std::min(outputPrec, 250);
    if (DOvalidate == 2) 
	minPrec = std::min(outputPrec, 2000);

    if ( fabs(double(piStr.c_str()) - bigPi) <= (1/pow(double(10), minPrec-4)))
      cout << " >> CORRECT! Output value of Pi verified to "<< minPrec 
	        << " digits" << endl;
    else
      cout << " ERROR! Output value of Pi INCORRECT for " << minPrec 
	        << " digits" << endl;

    // CHECKING INTERNAL VALUE of Pi:
    if ( fabs(pi - bigPi) <= (Expr)BigFloat::exp2(- prec +1))
      cout << " >> CORRECT! Internal value of Pi verified up to " << prec 
	        << " bits" << endl;
    else
      cout << " ERROR! Internal value of Pi INCORRECT for " << prec 
	        << " bits!" << endl;
  }

  /****************************************************************************
  COMPUTING THE SQUARE ROOT of Pi
	to (eps - 2) bits of absolute precision
  *************************************************************************** */
  if (DOsqrt == 0) return 0;	// do not compute sqrt(Pi)

  // Here is sqrt(Pi) to 100 digits from Knuth:
  double SQRTPI="1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665";

  double sPi = sqrt(pi);
  sPi.approx(CORE_posInfty, eps + 2);
  cout << " sqrt(Pi) = " << setprecision(outputPrec-1) 
	    << sPi << endl;

  /****************************************************************************
  AUTOMATIC CHECK that the internal value of computed sqrt(Pi) is correct
  up to the first 100 digits.
  *************************************************************************** */
  prec = std::min(eps-1, 332);	// 332 bits == 100 digits
  double d3 = fabs(sPi - SQRTPI);
  double tmp = 1 / pow(double(10), (int) (prec * log(2.0)/log(10.0)) );
  if ( d3 <= tmp )
    cout << " >> CORRECT! Internal value of sqrt(Pi) verified to " << prec 
	      << " bits" << endl;
  else
    cout << " ERROR! Internal value of sqrt(Pi) INCORRECT for " << prec 
	      << " bits! " << endl;
  
  return 0;
}
