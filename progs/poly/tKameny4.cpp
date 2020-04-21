/* **************************************
   File: tWilkinson.cpp

   Description: 

   Usage:
	% tSturm [opt-prec=100] [if-more-test-desired]

	where [opt-prec] is the optional precision bounding
	the bit size of the isolating intervals.
	It there is another argument (if-more-test-desired), then
	some slow polynomial evaluations will be invoked.

   Author:  Chee Yap, Sylvain Pion, Vikram Sharma
   Date:    July 20, 2002

   Since Core Library  v1.4
   $Id: tKameny4.cpp,v 1.3 2010/05/24 15:58:27 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 4
#endif

#include "CORE.h"

using std::cout;
using std::ios;
using std::endl;

// ==================================================
// Setup
// ==================================================
// typedef int NT;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;
typedef Sturm<NT> SturmNT;

// Polynomial tester:	P is polynomial to be tested
// -- THIS is a Kludge to get around "Poly" versus "Polynomial<NT>"
//  void testSturm(PolyNT &P, int prec, int n = -1)
//  {
//	  testSturm(P, prec, n);
//  }

// ==================================================
// Main
// ==================================================

int main(int argc, char** argv){
  
    // SET INTERNAL PRECISION
    int prec = 300;			// Do Sturm refinement till delta
					// is at most 2^{-prec}
    if (argc >= 2)
         prec = atoi(argv[1]);		// if user specifies

    setDefaultAbsPrecision(prec);

    bool slowTest = false;
    if (argc >= 3) 
	slowTest = true;		// if user specifies precision
  
    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits
    
    cout << "------------------------------------------------------------\n";
    cout << "Relative Precision is " << defRelPrec << endl;
    cout << "Print Digits is " << cout.precision() << endl;
    cout << "Iteration Stops when delta < " << BigFloat::exp2(-prec) << endl;
    //  cout << setprecision(defPrtDgt.asLong()); // already set above.
    cout << "------------------------------------------------------------\n";


    cout <<"=============================================================\n"
	 << "TEST :   Kameny Polynomial of degree 14 (4 real roots)\n" <<
      "=============================================================" << endl;

    const char* s[] = {"4", "0", "0", "0", "-4000000000000000000000000",
		 "0", "0", "4", 
		 "1000000000000000000000000000000000000000000000000", 
		 "0", "0","2000000000000000000000000","0", "0", "1"};


    NT cs[15];
    for(int i=0; i<15; i++) {
	    cs[i] = s[i];
    }


    PolyNT P(14, cs);
    P.dump("Kameny 4 ");
    testNewtonSturm(P, prec, 4);
    return 0;
  }// main
