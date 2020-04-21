/* **************************************
   File: tWilk20deriv.cpp

   Description: 

   Usage:
	% tWilk20deriv [opt-prec=100] 

	where [opt-prec] is the optional precision bounding
	the bit size of the isolating intervals.

	Let P(X) be the Wilkinson Polynomial of degree 20,
	and Q(X) = dP/dX.   This program isolates roots of Q(X).
	An interesting feature of Q(X) is that the root between
	10 and 11 is EXACTLY 10.5.  Our Sturm-Newton Refinement 
	will detect this correctly.


   Author:  Vikram Sharma, Chee Yap 
   Date:    July 20, 2002

   Since Core Library  v1.4
   $Id: tWilk20deriv.cpp,v 1.3 2010/05/24 15:58:27 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"

using namespace std;

// ==================================================
// Setup
// ==================================================
// typedef int NT;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;
typedef Sturm<NT> SturmNT;


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
	 << "Isolating the roots of the derivative of the Wilkinson Polynomial of degree 20\n" <<
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

    NT cs20[21];
    for(int i=0; i<21; i++) {
	    cs20[i] = s20[i];
    }

    PolyNT P20(20, cs20);
    PolyNT P = differentiate(P20);

    P.dump("Differentiation of Wilk20 = ");
    testNewtonSturm(P, prec, 19);

    return 0;
  }// main
