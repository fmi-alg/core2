/************************************************************
 * file: next.ccp
 * synopsis:	Computes the next largest IEEE floating point number
 *
 * usage: 
 *		% next
 *
 * author: Chee Yap
 * date: Aug 26, 2003
 * Since Core Library Version 1.6
 * $Id: next.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#define CORE_LEVEL 4

#include <CORE/CORE.h>
#include "next.h"

using namespace std;

int main(int argc, char** argv) {

	setScientificFormat();
	cout << setprecision(55);
	// setDefaultPrecision(100, CORE_INFTY);  // relative 100 bits
	cout << "=========== Next Floating Point Value =================\n";
	cout << " Default Precision is (100,infty)" << endl;
	cout << " Machine Precision = " << MACHINE_PREC << endl;
	cout << " BITS of precision in evaluating Expr = " << BITS << endl;
	cout << " CORE_EPS (Unit Roundoff) = " << CORE_EPS << endl;
	cout << "         CORE_EPS as Expr = " << show(CORE_EPS, BITS) << endl;
	cout << "==================================================\n";
	
	int init = 1;
	if (argc>1) init = atoi(argv[1]);
	int stop = 10;
	if (argc>2) stop = atoi(argv[2]);

	double x = init;
	double xx;
	Expr X;
	cout << ">>>> FORWARD values starting from " << x << endl;
	for (int i = init; i <= stop; i++){
		xx = next(x);
		cout << "  next double value = " << xx << endl;
		if (x == xx)
		   cout << "ERROR! next value equal previous value" << endl;
#ifdef myDEBUG
		else {
		   X = xx; cout << "     >> As Expr    = " << X ;
		}
		cout << "     >> to " << BITS << " bits = "
				<< show(xx, BITS) << endl;
#endif
		x = xx;
		cout << endl;
	}
	cout << ">>>> BACKWARD values starting from " << xx << endl;
	for (int i = init; i <= stop; i++){
		xx = prev(x);
		cout << "  prev double value = " << xx << endl;
		if (x == xx)
		   cout << "ERROR! prev value equal previous value" << endl;
#ifdef myDEBUG
		else {
		   X = xx; cout << "     >> As Expr    =  " << X ;
		}
		cout << "     >> to " << BITS << " bits = "
				<< show(xx, BITS) << endl;
#endif
		x = xx;
		cout << endl;
	}

	cout << "=========== END Floating Point Value Test =============\n";


	double a = 1.0;
	double aa = nextafter(a, 2);
	double aaa = nextafter(aa, 2);
	cout << " (aa,aaa) = " << aa << ", " << aaa << endl;
	if (aa == next(a)) cout << " aa == next(a)" << endl;
	else cout << " aa != next(a)" << endl;
	if (aaa == next(a)) cout << " aaa == next(a)" << endl;
	else cout << " aaa != next(a)" << endl;
	return 0;
}
