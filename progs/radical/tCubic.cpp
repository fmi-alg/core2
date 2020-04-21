/*************************************************************
 * File: tCubic.cpp
 * Purpose:
 * 	Test some interesting identities involving radicals.
 *
 * 	We use the "cbrt(x)" primitive for getting cube-root
 * 	of an expression x.  So it is analogous to "sqrt(x)".
 *
 * 	Note that there is a more general a "radical(x,n)" operator.
 * 	So, "cbrt(x)" and "radical(x,3)" are equivalent.
 *
 * Usage:   
 * 		> ./tCubic [prec=40]
 *
 * 	where the default values are shown above.
 *
 * Restriction/Bugs:
 * 	Currently, n must be an natural number (BigInt or int)
 *
 * Author: Chee Yap, Jihun Yu
 * Date:  June 11, 2010
 * Core Library version 2.0
 *************************************************************/

/*
 * We assume Core Level 3
 */

#include "CORE.h"

using namespace CORE;
using namespace std;

//typedef BigInt NT;
typedef Expr NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  outputPrec =  80;	// default output precision
  if (argc >= 4) {
    outputPrec = atoi(argv[3]);
  }

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  std::cerr << "outputPrec = " << outputPrec << ", eps = " << eps << std::endl;
 
  setDefaultOutputDigits(outputPrec);
  cout << std::setprecision(eps);

  /* **************************************************
   * Example 1 -- from Ramanujan (see \cite{mehlhorn:rasc:03})
   * 
   *   3 sqrt( X ) - cuberoot(2) - cuberoot(20) + cuberoot(25) = 0.
   *
   *        where   X  =  cuberoot(5) - cuberoot(4).
   *
   ************************************************** */
     Expr L, R, LL, RR;

	L = 3*sqrt(cbrt(NT(5)) - cbrt(NT(4)));
	R = cbrt(NT(2)) + cbrt(NT(20)) - cbrt(NT(25));

	LL = 3*sqrt(radical(NT(5),3) - radical(NT(4),3));
	RR = radical(NT(2),3) + radical(NT(20),3) - radical(NT(25),3);

	if ((L != LL)|| (R != RR)) 
	   cout << "ERROR! radical(x,3) and cbrt(x) unequal. " << endl;
	else
	   cout << "CORRECT! radical(x,3) and cbrt(x) unequal. " << endl;

	if (LL != RR) 
	   cout << "ERROR! version of Ramanujan failed " << endl;
	else
	   cout << "CORRECT! version of Ramanujan succeeded " << endl;

	if (L != R) {
	   cout << "ERROR! " <<
	   " 3 sqrt( crt(5)-crt(4) ) != crt(2) + crt(20) - crt(25) "
	   << endl;
	} else {
	   cout << "CORRECT! " <<
	   " 3 sqrt( crt(5)-crt(4) ) == crt(2) + crt(20) - crt(25) "
	   << endl;
	}
	cout << "       LHS = " << L << endl;
	cout << "       RHS = " << R << endl;
	cout << "       L-R = " << L-R << endl;
  /* **************************************************
   * Example 2 (Ramanujan -- see Yap, Chap.15)
   *   cuberoot(cuberoot(2)-1)  =
   *   		cuberoot(1/9) - cuberoot(2/9) + cuberoot(4/9).
   *
   *   Root Bound for R-L is 3156 (under BFMSS)
   * ...
   *
   ***************************************************/
        L = cbrt(cbrt(NT(2))-1);
        R = cbrt(NT(1)/NT(9)) - cbrt(NT(2)/NT(9)) + cbrt(NT(4)/NT(9));
	
        if (L != R) {
	   cout <<
	   "ERROR! crt(crt(2) -1) != crt(1/9) - crt(2/9) + crt(4/9)"
	   << endl;
	} else {
	   cout <<
	   "CORRECT!  crt(crt(1) -1) == crt(1/9) - crt(2/9) + crt(4/9)"
	   << endl;
	}
	cout << "       LHS = " << L << endl;
	cout << "       RHS = " << R << endl;
	cout << "       L-R = " << L-R << endl;
}
