/* ************************************************
   File: testSqrt.cpp

   Purpose:  
	If e is an Expression, then e.doubleValue() returns a machine double,
	say X.  This is a machine double that approximates e.  
	How accurate is this value?

	We compare X to native machine arithmetic, which
	is presumably the IEEE standard.  Say e=sqrt(n), and the IEEE
	value for sqrt(n) is IEEE_sqrt(n).  CORE does not guarantee that
	X equals IEEE_sqrt(n).  But we guarantee that

		|X - IEEE_sqrt(n)| < 4 * CORE_EPS * sqrt(n)
	
	where 2*CORE_EPS is the standard "machine epsilon".

   Usage:
        % testSqrt [n=2] [k=1] [showFlag=false]

	All arguments are optional, with defaults shown.
	We test the Expressions
	      sqrt(i), for i = n, n+1, ..., n+k-1.
	So if k=1, we only test  sqrt(n).
	If there is any third argument at all, showFlag is set to
	true, and then the output is "verbose".

   Theory:
	How is e = sqrt(n) tested? 
	Note that e is the exact square root of n.  Let X = e.doubleValue().  
	We compare X against the machine evaluation of sqrt(n).  
	There are 3 possibile outcomes for this test:

	  (1) ERROR:    |machine sqrt(n) - X | > 4 * CORE_EPS * e
	  (2) WARNING:  |machine sqrt(n) - X | > 0 (but not an error)
	  (3) CORRECT:  |machine sqrt(n) - X | = 0 
	
	Here,  2 * CORE_EPS = 2^{-52} = 2.22045e-16 is the relative
	error.  This value is also known as the machine epsilon.  

	Why did we use 4 * CORE_EPS = 4.44089e-16? 
	Because the machine arithmetic may introduce an error of
	2 * CORE_EPS, and our doubleValue() may introduce an error of
	2 * CORE_EPS.  
	
   Results:
	In a test of sqrt(i) (for i=2,...,1200),
	the "running" worst relative error is achieved at
	i=2, 17, 66, 257, 1028.  Note that these are just above powers of 2,
	a fact that could perhaps be expected (though we did not know it
	in advance).  Interestingly, no new maximum relative error value
	was achieved just above 128 in this sequence.
	The largest relative error achieved is 4.43224e-16.
	For i around 16,000, the largest relative error is achieved
	at sqrt(16,385) with 4.44075e-16.  So our 4*CORE_EPS bound
	is essentially tight.

	Update (Jun 2010):
	The above results were for Core 1.  In Core 2.0, we get the following
	"running" worst relative error for i=2,...,1200:

	    Maximum relative error, RelErr = 1.57009e-16 achieved at sqrt(2)
	    Maximum relative error, RelErr = 2.15415e-16 achieved at sqrt(17)
	    Maximum relative error, RelErr = 2.17017e-16 achieved at sqrt(67)
	    Maximum relative error, RelErr = 2.21182e-16 achieved at sqrt(258)
	    Maximum relative error, RelErr = 2.21828e-16 achieved at sqrt(1026)

	The sequence 2, 17, 67, 258, 1026 is consistent with, but differs from
	that in Core 1.   Moreover, in testing n up to 16,500, the 
	running maximum relative error is achieved at:

	    Maximum relative error, RelErr = 2.22018e-16 achieved at sqrt(16388)
	This suggests that Core 2's worst case error is bounded by
	2 * CORE_EPS = 2^{-52} = 2.22045e-16 rather than 4 * CORE_EPS.
	 
   Bug/Feature:
        -- doubleValue() does not check for overflow or underflows.
   	-- Expr::doubleValue() can result in error -- this should be fixed.
	-- It is possible to tighten Core Library's doubleValue() to
		that we avoid all WARNINGS altogether.  But it does not
		appear to be generally worthwhile (except as an option). 

   Author: Chee Yap (yap@cs.nyu.edu)
   Date:   Sept 28, 2003.  Revised, Mar 19, 2004.

   Since Core Library Version 1.6
   $Id: testSqrt.cpp,v 1.5 2010/07/22 11:23:33 exact Exp $
 ************************************************ */  

//# define CORE_DEBUG

#ifndef CORE_LEVEL
# define CORE_LEVEL 4
#endif

#include "CORE.h"

using namespace std;
using namespace CORE;

Expr test(double n, bool showFlag){

     Expr e	 = sqrt(Expr(n));	// this value is exact!
     double d	 = e.doubleValue();	// this value is what we want to check
     double dd	 = sqrt(n);		// this value is the IEEE approximation
     std::string s = e.toString(); //using default Output Precision
     Expr relerr(0.0);
  
     if (showFlag) {
     cout << endl;
     cout << "===================== SETPRECISION(15) ==================\n";
     cout << setprecision(15);
     cout << "   Expr:      sqrt(" << n << ") = " << e << endl;
     cout << "   doubleVal: sqrt(" << n << ") = " << d << endl;
     cout << "   Machine:   sqrt(" << n << ") = " << dd << endl;
     cout << "   STRING OUTPUT = " << s << endl;

     cout << "===================== SETPRECISION(14) ==================\n";
     cout << setprecision(14);
     cout << "   Expr:      sqrt(" << n << ") = " << e << endl;
     cout << "   doubleVal: sqrt(" << n << ") = " << d << endl;
     cout << "   Machine:   sqrt(" << n << ") = " << dd << endl;
     cout << "   ======= NOTE: the last three output should agree =====\n";
     cout << "===================== SELF-VALIDATION ===================\n";
     } else 
     cout << "tested expression " << e << endl;

     /* **************** DO SELF-VALIDATION *********    */

     if (d != dd) {
        relerr = core_abs(Expr(d) - Expr(dd))/e;
        if ( (relerr > 4*Expr(CORE_EPS))  && showFlag ) {
          cout<<"   ERROR:    | doubleVal - MachineVal | = " << relerr
		  << " >  4 * CORE_EPS " << endl;
	  core_error("Wrong Output", __FILE__, __LINE__, true);
	} 
	else if (showFlag) {
	  cout<< "   WARNING: | doubleVal - MachineVal | = " << relerr
		  << endl;
	  core_error("Inaccurate Output", __FILE__, __LINE__, false);
        } else if (showFlag)
	   cout << "   CORRECT:  doubleVal = MachineVal !! " << endl;
     }
     return relerr;	// this ought to be bounded by 4*CORE_EPS
}//test

int main(int argc, char** argv)
{
   // DEFAULT ARGUMENTS
   double n = 2.0;
   int k = 0;
   bool showFlag = false;

   // USER SUPPLIED ARGUMENTS
   if ((argc == 1)||(argc>4)) {
        cout << "Using default arguments:\n"
	     << "      ./testSqrt [n=2] [k=0] [showFlag=false]" << endl;
   }
   if (argc > 1) n = atoi(argv[1]);
   if (argc > 2) k =atoi(argv[2]);
   if (argc > 3) showFlag = true;

   // DO THE TEST
   Expr maxError(0.0), curError;
   double worsti = 0;

   for (double i=n; i<n+k; i++) {
	  curError = test(i, showFlag);
	  if (maxError < curError) {
		worsti = i;
	  	maxError = curError;
	  }

   	cout<< " =================\n SUMMARY:" << endl;
   	cout<< "    Maximum relative err (RelErr) = " << maxError
	   	<< " achieved at sqrt(" << worsti << ")" << endl;
   	cout<< "    Allowed relative err (ErrBound) = " << 4*CORE_EPS << endl;
   	if (maxError > 4 * Expr(CORE_EPS)){
       		cout<< "    ERROR!  RelErr > ErrBound" << endl;
	  	core_error("Wrong Output", __FILE__, __LINE__, true);
	} else
       		cout<< "    CORRECT!  RelErr <= ErrBound" << endl;
   }//for   
   return 0;
}//main
