/* ************************************************
   File: prog5

   Purpose:  Illustrating the introduction of algebraic numbers
	     
	     
   Usage: 

        % prog5

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog5.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main(int argc , char ** argv) {

   NT coeffs[] = {-1, -1, 1}; // so coeffs[i] is the coefficient of X^i
   PolyNT P(2, coeffs);	// so P = X^2 - X - 1

   Expr phi1 = rootOf(P, 2);  // phi1 is the 2nd smallest root of P
   cout << "Let P(X) = X^2-X-1 " << endl;
   cout << "The second smallest root of P(X) is phi1 = rootOf(P,2) = "
	   << phi1 << endl;

   BFInterval I(-1, 0);    // I is the interval [-1, 0]
   Expr phi2 = rootOf(P, I); // phi2 is the unique negative root of P
   cout << "phi2 = rootOf(P,[-1,0])  = " << phi2 << endl;

   if (phi1 + phi2 == 1) cout << "CORRECT! phi1 + phi2 = 1"  << endl;
   else cout << "ERROR! phi1 + phi2 != 1" << endl;

   Expr goldenRatio = (1 + radical(5,2))/2; //  another way to specify phi
   cout << "golden ratio = (1 + sqrt(5))/2  = " << goldenRatio << endl;
   if (phi1 == goldenRatio) cout << "CORRECT! phi1 = goldenRatio" << endl;
   else cout << "ERROR! phi1 != goldenRatio" << endl;

   if (phi1 *phi1 - phi1 - 1 == 0)
   	cout << "CORRECT! phi1^2 - phi1 - 1 = 0" << endl;
   else
   	cout << "ERROR! phi1^2 - phi1 - 1 != 0" << endl;
}


