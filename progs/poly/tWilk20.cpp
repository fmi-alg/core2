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
   $Id: tWilk20.cpp,v 1.2 2006/08/07 14:29:04 exact Exp $
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
    int prec = 100;			// Do Sturm refinement till delta
					// is at most 2^{-prec}
    if (argc >= 2)
         prec = atoi(argv[1]);		// if user specifies

    setDefaultAbsPrecision(prec);

    bool slowTest = (argc >= 3)? true : false;
  
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
      << "TEST 6:   Wilkinson Polynomial of degree 10 (roots are 1,..,10)\n" 
      << "       	WARNING: VERY SLOW\n" <<
      "=============================================================" << endl;

    const char* s20[] = {"2432902008176640000", "-8752948036761600000",
		"13803759753640704000", "-12870931245150988800",
	    	"8037811822645051776", "-3599979517947607200",
		"1206647803780373360", "-311333643161390640",
		"63030812099294896", "-10142299865511450",
		"1307535010540395", "-135585182899530",
         	"11310276995381", "-756111184500",
		"40171771630", "-1672280820",
		"53327946", "-1256850",
		"20615", "-210", "1"};

    PolyNT P20(20, s20);
    P20.dump("P20=");
    testNewtonSturm(P20, prec, 20);

    return 0;
  }// main
