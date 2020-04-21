/* ***************************************************************
   File: newPi.ccp

Purpose: To compute Pi using several algorithms:
		Brent's algorithma and Machin's algorithm
   	     
   Usage:
	  % multiPi [optPrec=54] [method=2]

	where the arguments (all optional) are

	-- [optPrec] is the absolute precision in number of bits.
		Default is [optPrec] = 54 (as in machine double)
	-- [method] specifies the algorithm:

		 method=1 for machin's algorithm
		 method=2 for brent's algorithm (dynamically checking if suff.precision)
		 method=3 for brent's algorithm (static determination of suff.precision)

	EXAMPLE:  To compute Pi to 2000 digits, set [optPrec] to
	6644 (= 2000 * log_2(10)).  

   Author: Zilin (Oct 2004)
   Since Core Library Version 1.7
   	$Id: t.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 4:
#  define CORE_LEVEL 4

#include <fstream> 
#include "CORE.h"

using namespace std;

BigInt R(int a, int b) {
	if (b - a == 1)
		return 2*a + 3;
	else {
		int m = (a+b)/2;
		return R(a, m) * R(m, b);
	}
}
BigInt Q(int a, int b, int x) {
	if (b - a == 1)
		return (2*a + 3)*x*x;
	else {
		int m = (a+b)/2;
		return Q(a, m, x) * Q(m, b, x);
	}
}
BigInt P(int a, int b, int x) {
	if (b - a == 1) {
		if (a % 2 == 1)
			return 1;
		else
			return -1;
	} else {
		int m = (a+b)/2;
		return Q(m, b, x)*P(a, m, x) + R(a, m)*P(m, b, x);
	}
}
Expr arctan(int x, int a) {
  // compute arctan(1/x) to absolute precision 2^{-a}
  // x is a natural number and x > 1.

  int N = (int) (a / ( 2 * log(float(x))/log(2.0))) + 1;
  // cout << " Number of terms needed in arctan, N = " << N << endl;

/*  double at = 1;
  double x2 = x * x;
  for (int i=1; i<=N; i++) {
    if (i % 2 == 1) 
      at = x2 * at - 1 / double(2 * i + 1);
    else
      at = x2 * at + 1 / double(2 * i + 1);
  }
*/
  Expr at = (1 + Expr(P(0, N, x))/Expr(Q(0, N, x))) / Expr(x);  
//  at /= pow (x2, N) * x;
  
  at.approx(CORE_posInfty, a);
  return at;
}

Expr machin(int prec) {
  // Computing Auxilliary Values
  Expr t1 = arctan(5, prec + 6);
  Expr t2 = arctan(239, prec + 4);

  Expr ans = (4 * t1 - t2) * 4;
  ans.approx(CORE_posInfty, prec);
  return ans;
}

BigFloat brent(int prec) {
  defBFsqrtAbsPrec = prec;
  BigFloat A = 1;
  BigFloat B = 1;
  B /= sqrt(BigFloat(2));
  BigFloat T = 0.25;
  BigInt X = 1;
  BigFloat Y;
  BigFloat eps = BigFloat::exp2(- prec +1);	
  BigFloat sq;
  int count = 0;

  while (A - B > eps) {
	Y = A; A = (A+B).div2(); B = sqrt(B*Y);
	sq = (A-Y)*(A-Y);
	T -= X * sq;
	X <<= 1;
	A.makeExact();
	B.makeExact();
	T.makeExact();
        count ++;
  }
  //Expr ans = Expr(A)*Expr(A)/Expr(T);
  //ans.approx(CORE_posInfty, prec);
  cout << "Number of iterations = " << count << endl;
  BigFloat ans = (A*A).div(T, prec + T.lMSB() - 2* A.uMSB() + 2);
  return ans;
} //brent

BigFloat brent2(int prec) {
  int iters = log(prec*log(2.0)/log(10.0))/log(2.0);
  cout << "Need " << iters << " iterations" << endl; 
  defBFsqrtAbsPrec = prec;
  BigFloat A = 1;
  BigFloat B = BigFloat(1)/sqrt(BigFloat(2));
  BigFloat T = 0.25;
  BigInt X = 1;
  BigFloat Y;
  BigFloat eps = BigFloat::exp2(-prec +1);
  BigFloat sq;
  BigFloat AA;
  int count = 0;

  for (int p = -1; p < prec*2+10; p = p*2+10) {
        Y = A*B; 
        A = (A+B).div2(); 
        B = sqrt(Y);
        AA = A*A;
        sq = AA - Y;
        T -= X * sq;
        X <<= 1;
        A.makeExact();
        B.makeExact();
        T.makeExact();
	count ++;
  }
  cout << "Number of Iteration = " << count << endl;
  BigFloat ans = (AA).div(T, prec + 2);
  return ans;
}
int main( int argc, char *argv[] ) {

  /* ***************************************************************************
  COMMAND LINE ARGUMENTS
  *************************************************************************** */
  int eps = 54; 	// Number of bits of absolute precision desired
			// default to 54 bits (= machine double precision)
  int arg = 1; 

  if (argc > 1) eps = atoi(argv[1]);	
  if (argc > 2) arg = atoi(argv[2]);

  //int DOvalidate = 1;   // 1st level of checking

  /* ***************************************************************************
  COMPUTING Pi
  *************************************************************************** */
  // compute pi to eps-bits of precision:
  Expr pi1;
  BigFloat pi2;
  Timer timer;
  timer.start();
  if (arg == 1)
    pi1 = machin(eps);
  else if (arg == 2)
    pi2 = brent(eps);
  else 
    pi2 = brent2(eps);

  timer.stop();
  cout << "Time = " << timer.getSeconds() << endl;

  // Translates eps (in bits) to outputPrec (in digits)

  int outputPrec;	// desired precision in digits
  outputPrec = (int) (eps * log(2.0)/log(10.0));
  cout << "Output precision is " << outputPrec << " digits \n";

  // Output of Pi
  cout << "outputPrec = " << outputPrec << endl;
  if (arg == 1)
    cout << " Pi = " << setprecision(outputPrec+1) << pi1 << endl;
  else 
    cout << " Pi = " << setprecision(outputPrec+1) << pi2 << endl;

  return 0;
}

