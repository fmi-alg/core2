/************************************************************
 * file: tOverflow.cpp
 * author: Chee Yap and Juan
 * synopsis:	
 * 	Testing overflow
 *
 * usage: 
 *
 * date: Feb 1, 2018
 * Since Core Library Version 2.1
 * $Id: tDoubleValue.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#include <iostream>
#include <cmath>
#include <ieeefp.h>

//#pragma fenv_access (on)  


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
double test(int n, double a, double b) {
    double c = a;
    for (int i=1; i<n; i++) 
	c = c*a;
    return c;
}

int main(int argc, char ** argv) {
	int n = 1000;
	if (argc > 1) n = atoi(argv[1]);
	double a = 1000.0;
	if (argc > 2) a = atoi(argv[2]);
	double b = 2;
	if (argc > 3) b = atoi(argv[3]);

	cout << "===========================================================\n";
	cout << "   n= " << n << ", a= " << a << ", b= " << b << endl;
	cout << "===========================================================\n";

	for (int i=n/2; i<n; i++) {
		fpsetmask(0);
		test(i,a,b);
		if (fpgetmask() & FP_X_OFL)
		    cout << "overflow at i=" << i << endl;
	}
	return 0;
}
