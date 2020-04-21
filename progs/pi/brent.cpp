/* ***************************************************************
   File: brent.ccp

   Purpose: To compute Pi using Brent's algorithm

   Usage:
	  % pi [optPrec] [DOsqrt] [DOvalidate]

	where the arguments (all optional) are

	-- [optPrec] is the absolute precision in number of bits.
		Default is [optPrec] = 54 (as in machine double)

	-- [DOvalidate] is a flag (default to 1) 
		[DOvalidate]=0 means no validation of Pi.  
		[DOvalidate]=1 means validate Pi up to 250 digits.
		[DOvalidate]=2 means validate Pi up to 2000 Digits.  

	EXAMPLE:  To compute Pi to 2000 digits, set [optPrec] to
	6644 (= 2000 * log_2(10)).  If you also want to check the 2000
	computed digits, but to skip the sqrt(Pi) computation, you may type:

	  % pi 6644 0 2

   Author: Chee Yap and Chen Li (June 2000)
   Since CORE Library Version 1.2
   	$Id: brent.cpp,v 1.3 2007/10/19 16:00:58 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#  define CORE_LEVEL 4

#include <fstream> 
#include "CORE.h"

using namespace std;

// implements Brent's algorithm for Pi
//	to precision 2^{-prec}
//
/* Original version using Expr:
Expr brent(int prec) {

  Expr A = 1;
  Expr B = Expr(1)/ sqrt(Expr(2));
  Expr T = BigRat(1,4);
  BigInt X = 1;
  Expr Y;
  Expr eps = BigFloat::exp2(- prec +1);	
  Expr sq;

  while (A - B > eps) {
	Y = A; A = (A+B)/2; B = sqrt(B*Y);
	sq = (A-Y)*(A-Y);
	T -= X* sq;
	X <<= 1;
  }
  Expr ans = A*A/T;
  ans.approx(CORE_posInfty, prec);
  return ans;
} //brent
*/

// New version using BigFloat
Expr brent(int prec) {
  defBFradicalRelPrec = prec;
  BigFloat A = 1;
  BigFloat B = 1;
  B /= sqrt(BigFloat(2));
  BigFloat T = 0.25;
  BigInt X = 1;
  BigFloat Y;
  BigFloat eps = BigFloat::exp2(- prec +1);	
  BigFloat sq;

  while (A - B > eps) {
	Y = A; A = div2(A+B); B = sqrt(B*Y);
	sq = (A-Y)*(A-Y);
	T -= X * sq;
	X <<= 1;
	//A.makeExact();
	//B.makeExact();
	//T.makeExact();
  }
  Expr ans = Expr(A)*Expr(A)/Expr(T);
  ans.approx(CORE_posInfty, prec);
  return ans;
} //brent

int main( int argc, char *argv[] ) {

  /* ***************************************************************************
  COMMAND LINE ARGUMENTS
  *************************************************************************** */
  int eps = 54; 	// Number of bits of absolute precision desired
			// default to 54 bits (= machine double precision)
  if (argc > 1) eps = atoi(argv[1]);	

  int DOvalidate = 1;   // 1st level of checking

  /* ***************************************************************************
  COMPUTING Pi
  *************************************************************************** */
  // compute pi to eps-bits of precision:
  Expr pi = brent(eps);
  // Translates eps (in bits) to outputPrec (in digits)

  int outputPrec;	// desired precision in digits
  outputPrec = (int) (eps * log(2.0)/log(10.0));
  cout << " Output precision is " << outputPrec << " digits \n";

  // Output of Pi
  cout << "outputPrec = " << outputPrec << endl;
  cout << " Pi = " << setprecision(outputPrec+1) << pi << endl;

  /* ***************************************************************************
  AUTOMATIC CHECK that
	(1) our internal value of Pi
  	(2) our output value of Pi
  are both correct to 250 (or 2000) digits
  *************************************************************************** */

  // Reading in digits of Pi from Files
  int prec;				// bit precision for the comparison
  ifstream from;			// input stream from file
  if (DOvalidate == 0) {		// no validation 
    prec = 1;
  }
  if (DOvalidate == 1) {
    prec = min(eps, 830);	// 830 bits = 250 digits.
    from.open("inputs/PI250", ios::in);	// read 250 digits of Pi from file
  }
  if (DOvalidate == 2) {
    prec = min(eps, 6644);	// 6644 bits = 2000 digits
    from.open("inputs/PI2000", ios::in);	// read 2000 digits of Pi from file
  }

  if (DOvalidate > 0) {		// validation needed
		string piStr; 
    char c;
    while (from.get(c))
	if ((c >= '0' && c <= '9') || (c=='.')) 
		piStr += c;
    if (!from.eof()) cout << "!! Error in reading from PI250 \n";
    from.close();
    Expr bigPi(piStr.c_str());	// bigPi is the value of Pi from file

    // debug:
    //  cout << " bigPi = " << setprecision(outputPrec + 1) << bigPi << endl;

    // CHECKING OUTPUT VALUE OF Pi:
    ostringstream ost;
    ost << setprecision(outputPrec+1) << pi << endl;
    piStr = ost.str();
    // Need to take the min of outputPrec and the number of digits in bigPi
    int minPrec = 0;
    if (DOvalidate == 1)
	minPrec = min(outputPrec, 250);
    if (DOvalidate == 2) 
	minPrec = min(outputPrec, 2000);
//debug:
cout << "minPrec = " << minPrec << endl;
cout << "bigPi = " << setprecision(outputPrec+1) << bigPi << endl;
cout << "pi    = " << setprecision(outputPrec+1) << Expr(piStr.c_str()) << endl;


// ?? fail:
// Expr ee = pow(Expr(10), -minPrec+4); ee.approx(minPrec * 4);
// cout << "pow(10, -minPrec+4) = " << pow(Expr(10), -minPrec+4) << endl ;

    if ( fabs(Expr(piStr.c_str()) - bigPi) <= pow(Expr(10), -minPrec+4))
       cout << " >> Output value of Pi verified to "<< minPrec << " digits\n";
    else
       cout << " !! Output value of Pi INCORRECT to " << minPrec << " digits! \n";

/** ignore for now
    // CHECKING INTERNAL VALUE of Pi:
    if ( fabs(pi - bigPi) <= BigFloat::exp2(- prec +1))
       cout << " >> Internal value of Pi verified up to " << prec << " bits\n";
    else
       cout << " !! Internal value of Pi INCORRECT to " << prec << " bits! \n";

 **/

  };

  return 0;
}

