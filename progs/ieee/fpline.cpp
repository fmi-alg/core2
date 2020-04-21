/************************************************************
 * file: fpline.cpp
 * author: Chee Yap
 * synopsis:	
 * 		Test to understand ``fp-lines'' (lines defined
 * 		with fixed precision floating-point numbers)
 *
 *
 * usage: 
 *		% ./fplines [x= 0 [a=2 [b=3 [c=-1]]]]
 *
 *		This looks at the the line equation
 *
 *		    aX + bY + c = 0
 *
 * 		starting at X=x.  The default values are indicated.
 *
 * date: June 16, 2003
 * Since Core Library Version 1.6
 * $Id: fpline.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#ifndef CORE_LEVEL
# define CORE_LEVEL 4
#endif

#include <CORE.h>
#include <CORE/geometry2d.h>
using namespace std;


// testLine(x, a, b, c):
// 	Line equation is aX + bY + c = 0.
// 	Initially, X = x.
// 	We print the various values of Y as x increases.

void testLine(double x, double a, double b, double c) {

  cout << "=================== Begin testLine ==========================\n";

  cout << "Line equation = " << a << "X + " << b << "Y + " << c << " = 0\n";
  Expr A = a;
  Expr B = b;
  	assert(b != 0);
  Expr C = c;
  Expr X, Y;
  double y;

  //Initial Value:
  X = x;

  //Loop
  for (int i = 0; i<100; i++) {
	cout << "  X    = " << X << endl;
	cout << "     x = " << x << endl;
	Y = -  (C + (A*X))/B;
	cout << "  Y = -(C + (A*X))/B  = " << Y << endl;
	cout << "     Y.doubleValue()  = " << Y.doubleValue() << endl;

	y = -  (c + (a*x))/b;
	cout << "                    y = " << Y << endl;
	X += CORE_EPS; 
	x = X.doubleValue();
  }	
  cout << "===================== End testLine ==========================\n";
}// testLine

int main(int argc, char ** argv) {

	double x = 0.0;
		if (argc > 1) x = atof(argv[1]);
	double a = 2;
		if (argc > 2) a = atof(argv[2]);
	double b = 3;
		if (argc > 3) b = atof(argv[3]);
	double c = -1;
		if (argc > 4) c = atof(argv[4]);

	int prec = 16;
	setDefaultOutputDigits(prec);
	double relEps = 1.0 + CORE_EPS;

	cout << "===========================================================\n";
	cout << "   OUTPUT PRECISION : " << prec << endl;
	cout << "   CORE_EPS         : " << CORE_EPS << endl;
	cout << "   relEps           : " << relEps << endl;
	cout << "   ========================================================\n";
	testLine(x, a, b, c);
	cout << "   ========================================================\n";

	return 0;
}
