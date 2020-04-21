/* ************************************************
   File: dagstuhl-2017.cpp

   Purpose:  Some examples arising at the
   			Dagstuhl Seminar 17481
   		"Reliable Computation and Complexity on the Reals"
			Nov 26-Dec 1, 2017.

   Usage:
        % dagstuhl-2017		-- non-verbose output
        % dagstuhl-2017 1	-- verbose output

   Author: Chee Yap

   Since Core Library 2.0
   $Id: testBigFloat.cpp,v 1.3 2010/06/25 09:04:03 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;
using namespace CORE;


  Expr min(Expr a, Expr b){
      if (a>b) return b;
      return a;
  };
  // Define "erf" function to be identity (hack)
  Expr erf(Expr a){
      return a;
  }

  // logistic function
  Expr logistic(int n){
      Expr x = Expr("1/2");
      Expr c = Expr("375/100");
      for (int i=0; i< n; i++){
	  x = c*x*(1-x);
	  cout << "x[" << i << "] = " << x << endl;
      }
      return x;
  }

//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

  // Command line arguments:
  int verb = 0;
  coretest_verbose = false;
  if (argc>1) verb = atoi(argv[1]);
  	if (verb > 0) coretest_verbose = true;
  int prec = 52;
  if (argc>2) prec = atoi(argv[2]);	// prec is operation precision
  	int oprec = 20;
  if (argc>3) oprec = atoi(argv[3]);	// oprec is output precision
  	int n = 10;
  if (argc>4) n = atoi(argv[4]);	// n is number of iterations
  
  // Global unit test variables:
  setDefaultPrecision(prec, CORE_INFTY); // prec is in relative bits
  cout.precision(oprec);		// oprec is in number of digits

  coretest_error=false;
  coretest_verbose=false;

  //////////////////////////////////////////////////
  // Evaluating Rump's Polynomial (Dagstuhl talk of Paul Zimmermann)
  //////////////////////////////////////////////////

  cout <<  "========================================\n"
      << "(Example 1) Evaluating Rump's Polynomial \n"
      << "       poly = 21*bb - 2*aa + 55*bb*bb - 10*aa*bb + (a/(2*b));\n"
      << " where \n"
      << "       a=77617, b=33096, aa=a^2,  bb=b^2. \n"
      <<  "========================================"
      << endl;

  BigFloat a("77617");
  BigFloat b("33096");
  BigFloat aa = a*a;
  BigFloat bb = b*b;

  BigFloat poly =  21*bb - 2*aa + 55*bb*bb - 10*aa*bb + (a/(2*b));

  core_test(poly, poly, "poly is wrong");

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  // Norbert Mueller's Logistic Map Example at Dagstuhl talk
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  
  cout <<  "========================================\n"
       << "(Example 2) Computing the logistic map\n" 
       <<  "            x' = c*x*(1-x)   where c = 1/2, x=3.75 \n"
       <<  "========================================"
       << endl;

  logistic(n);
  
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  // Siegfried Rump's Example at Dagstuhl talk
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  //
  // 		erf(min(1000, abs(R, sin(4* atan(1)))))
  //
  // 	where R is very large.
  //
  // 	Note that atan(1)=Pi/4, and so sin(4*atan(1)) = sin(Pi)=0.
  // 		Thus ans should be 0.   Thus erf(0)=0.
  // 		Since Core Library does not have erf(x),
  // 		we just define erf(x)=x.
  
  cout <<  "========================================\n"
       << "(Example 3) Computing Rump's Example \n"
      << "      ans = erf(min(1000, abs(R * sin(4* atan(1))))) \n"
      << " where erf is re-defined as the erf(x)=x (hack).\n"
      <<  "========================================"
      << endl;

  Expr R=1000000000000;
  Expr at = atan(1);
  Expr ans = erf(min(Expr(1000), abs(R * sin(4* at))));

  core_test(0, ans, "ans is wrong");


  return 0;
}//main


