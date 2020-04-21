/* file: next.h
 *
 * Synopsis:
 * 	Main function here is 
 * 			next(double x)
 * 	which returns the next larger floating point value.
 * 	
 * 	The converse of next is
 * 			prev(double x)
 * 	which returns the next smaller floating point value.
 *
 * 	There are 2 mysteries here (see below).
 *
 * 	NOTE: it is in standard C library that
 * 	the function nextafter(double x, double y)
 * 	will do the same and "next/prev", but return
 * 	a value between x and y.
 * 	For floats, it is called nextafterf(x,y).
 *
 * Author: Chee
 * Date: Aug 2003
 */


/* **************************************************
 * Mystery 1:  I use the 52 because 53 does not work.  
 * 	But I believe 53 should be the correct value correct.
 * 	
 * 	Actually, it does work for numbers less than 1.
 * 	Try invoking "next 0" when value 53 (or larger) is used.  
 * 	But this can be explained by subnormal numbers.
 * **************************************************/

#include <CORE/CORE.h>
using namespace std;

#ifndef MACHINE_PREC
#	define MACHINE_PREC 53
#endif

#ifndef BITS
# 	define BITS 60
#endif


/* **************************************************
 * next(x) returns the next smallest machine double.
 * **************************************************/
double next(double x) {
	BigFloat e = x;
	long fl = floorLg(e.BigIntValue()); 	// e is auto-converted to Expr in 
				// order to call floorLg !!
cout << "fl=" << fl << endl;
	BigFloat bf = BigFloat::exp2(1 + fl- MACHINE_PREC );
cout << "bf=" << bf << endl;
cout << "2^{-52}=" << ldexp(1.0, -52) << endl;
	e += bf;
cout << "e=" << e << endl;
// #ifdef myDEBUG
// cout << "      fl = floorLg(" << x << ") = " << fl << endl;
// cout << "      bf = exp2(1 + fl - MACHINE_PREC) = " << bf << endl;
// cout << "      " << x << " + bf = " << e << endl;
// cout << "      next = " << e.doubleValue() << endl;
// #endif
	return e.doubleValue();
}

/* **************************************************
 * prev(x) returns the next largest machine double.
 * **************************************************/
double prev(double x) {
	BigFloat e = x;
	long fl = ceilLg(e.BigIntValue());
	BigFloat bf = BigFloat::exp2( fl- MACHINE_PREC );
	e -= bf;
// #ifdef myDEBUG
// cout << "      fl = ceilLg(" << x << ") = " << fl << endl;
// cout << "      bf = exp2( fl - MACHINE_PREC) = " << bf << endl;
// cout << "      " << x << " + bf = " << e << endl;
// cout << "      prev = " << e.doubleValue() << endl;
// #endif
	return e.doubleValue();
}

/* **************************************************
 * show(x, b) returns BigFloat value of x, to b bits
 *
 * 	Mystery 2: why does this show more bits than
 * 	the ieee double?
 * **************************************************/
BigFloat show(double x, int bits=54) {
	Expr e = x;
	e.approx(bits, CORE_INFTY);
	BigFloat b = e.BigFloatValue();
	// b.makeExact();  // not needed
	return b;
}

