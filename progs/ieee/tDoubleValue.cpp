/************************************************************
 * file: tDoubleValue.cpp
 * author: Chee Yap and Zilin Du
 * synopsis:	
 * 	Testing two conversion routines in Expr.h:
 *
 * 		Expr::doubleValue()
 * 			-- returns the closest machine double value
 * 		Expr::doubleInterval( lb, ub) 
 * 			-- returns an enclosing interval [lb,ub]
 * 			   formed by two machine doubles lb, ub.
 * 				
 *	NOTE: we use two builtin functions: 
 *	      Suppose d=double and e=int.  Then
 *	        frexp(d, &e)  -- sets e to the exponent part of d
 *		ldexp(d, e)    -- returns the value (d * 2^e);
 *
 * usage: 
 *		% tDoubleValue [dValue="1.23"] [prec=17]
 *
 * KNOWN BUG:  
 *      The print out for Expr(e) does not
 *      seem to be always consistent with machine double interval.
 *
 *      E.g., 
 *          	Expr e = 1 + 2^{-52} 
 *      has the printout value = 1.00000000000000044 
 *	But e.doubleInterval(lb, ub) gives
 *
 *          	[lb, ub]  = (1.0000000000000004, 1.0000000000000004)
 *
 *
 * date: June 16, 2003
 * Since Core Library Version 1.6
 * $Id: tDoubleValue.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#include <CORE/CORE.h>
using namespace std;

// test( s )
// 	where s is string of digits (called baseVal)
//
//	(1)
// 	We convert s to Expr e, and to machine double d.
// 	We compare e.doubleValue() with d:
// 		if they are not equal, we have error.	
//
//	(2)
//	We also call e.doubleInterval(lb, ub) which returns
//		the interval defined by the machine doubles lb and ub.
//	We compare to see if  lb <= d <= ub.
//		If not, we have an error.
//
void test(const char * baseVal) {

	cout << "   =====================================================\n";
	machine_double d = atof(baseVal);
        Expr e(baseVal);
	cout << " baseVal             = " << baseVal << endl;
	cout << "   e = Expr(baseVal) = " << e << endl;
	cout << "   d = atof(baseVal) = " << d << endl;
        cout << "   e.doubleValue()   = " << e.doubleValue() 
	     <<	" (should be same as previous value)" << endl;

	machine_double lb, ub;
        e.doubleInterval(lb, ub);
	cout << "   [lb, ub]  = (" << lb << ", "  << ub << ")" << endl;
	if ((e > Expr(ub)) || (e < Expr(lb))) 
		cout << "ERROR!  e is not inside [lb, ub]" << endl;
	else
		cout << "CORRECT!  e is inside [lb, ub]" << endl;

	cout << " NEXT, modify e to next representable double value " << endl;
	int exp;
	frexp(d, &exp);   // frexp(d, &e) sets e to the exponent part of d
	e += ldexp(1.0, -52+exp);	// ldexp(x,y)  = x * 2^y;
	cout << "   e+ 2^{-52+e.exp}  = " << e << endl;
	d += ldexp(1.0, -52+exp);	// ldexp(x,y)  = x * 2^y;
	cout << "   d+ 2^{-52+d.exp}  = " << d << endl;
        cout << "   e.doubleValue()   = " << e.doubleValue()
	     <<	" (should be same as previous value)" << endl;

	cout << " NEXT, get the double Interval containing e, d " << endl;
        e.doubleInterval(lb, ub);
	cout << "   [lb, ub]  = (" << lb << ", "  << ub << ")" << endl;
	if ((e > Expr(ub)) || (e < Expr(lb))) 
		cout << "ERROR!  e is not inside [lb, ub]" << endl;
	else
		cout << "CORRECT!  e is inside [lb, ub]" << endl;
	cout << "   =====================================================\n";
}

int main(int argc, char ** argv) {
	setDefaultRelPrecision(60);

	const char * baseVal = "0.123456789012345";
	if (argc >= 2) baseVal = argv[1];

	int prec = 18;
	if (argc >= 3) prec = atoi(argv[2]);

	setDefaultOutputDigits(prec);

	cout << "===========================================================\n";
	cout << "   OUTPUT PRECISION : " << prec << endl;
	cout << "===========================================================\n";

	test(baseVal);
	test("0");
	test("1");
	test("-1");
	test("123");
	test("0.123");
	test(".123");

	return 0;
}
