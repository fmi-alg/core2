/* **************************************
   File: tBigFloat.cpp

   Description: 
   	BigFloat has some unusual functions

		isDivisible, gcd, div_exact, etc

	which are needed to make Polynomial<BigFloat>
	compatible with Polynomial<BigInt>.
	This is a test of these functions.

	These functions do not normally make sense for BigFloats,
	especially when a BigFloat is viewed as approximation.
	But we can define them sensibly when the BigFloat is exact
	(no error), in which case, they are just generalizations
	of the same concepts for rational numbers.   I.e.,
	we can define gcd(x,y), isDivisible(x,y) when x,y
	are rational numbers (see Yap's book).

   Usage:
   	> tBigFloat

   Author:  Chee Yap 
   Date:    July 20, 2004

   Since Core Library  Version 1.7
   $Id: tBigFloat.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"

using namespace std;

typedef BigInt				NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;



// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[])
{
    // GET INPUT ARGUMENTS
	//int curveNo = 0;	// which curve to display
	//if (argc>1) curveNo = atoi(argv[1]);

    // SET INTERNAL PRECISION
    setDefaultAbsPrecision(CORE_posInfty); // puts defAbsPrec in control
    int prec = 100; 
    setDefaultRelPrecision(prec); // put defRelPrec in control
    //cout << setprecision(6);	  // output precision (6 digits)
    cout << setprecision(52);	  // output precision (52 digits)

    // THE FOLLOWING USED TO WORK (before 2017)
    // double x=2.4;
    // double y=1.2;
    // cout << "2.4 / 1.2 = " << x/y << endl; // output 2
    	// Error message (2017):
	//	> ./tBigFloat
	//	2.4 / 1.2 = 2
	//	init2.c:52: MPFR assertion failed: p >= 2 &&
	//	  p <= ((mpfr_prec_t)((mpfr_uprec_t)(~(mpfr_uprec_t)0)>>1))
	//	make: *** [Makefile:36: r] Aborted (core dumped)
	// But after commenting out the above 3 lines of code,
	// the same error persists elsewhere.
	// See, e.g., 
	// https://stackoverflow.com/questions/32255374/mpfr-program-crashes-with-high-precision
    
    BigFloat b( 35,0, -4);
    BigFloat c( 49,0, 3);
    BigFloat d( 7,0, 5);
    BigFloat e( 49,0, 1);	// this is 49*2^{14}=802816
    BigFloat f( 16,0, -1);
    BigFloat g( 10,0,0);
    BigFloat h( 1,0, 1);	// this is 2^{14}=16384
/* output:
BF(35,0,-4) = b =4.85723e-16
BF(49,0,3)  = c =2.15504e+14
BF(7,0,5)   = d =8.26414e+21
BF(49,0,1)  = e =8.02816e+5
BF(16,0,-1) = f =0.00098
BF(10,0,0)  = g =10.
BF(1,0,1)   = h =16384.
e*g =8.02816e+6
gcd(b,c) = 9.71445e-17
gcd(d,c) = 3.07863e+13
gcd(b,d) = 9.71445e-17
gcd(e,d) = 1.14688e+5
e/h = 49.
*/
    cout << "BF(35,0,-4) = b =" << b << endl;
    cout << "BF(49,0,3)  = c =" << c << endl;
    cout << "BF(7,0,5)   = d =" << d << endl;
    cout << "BF(49,0,1)  = e =" << e << endl;
    			// output here should be 802816 = 8.02816e5
    			// but Core prints 8.02816e6 !!!
    cout << "BF(16,0,-1) = f =" << f << endl;
    cout << "BF(10,0,0)  = g =" << g << endl;
    cout << "BF(1,0,1)   = h =" << h << endl;
    cout<<"##################################################" << endl;
    BigFloat zBF(0);
    cout << "BigFloat(0) = " << zBF << endl;
    return 0;
}

