/*************************************************************
 * File: tMoreRadical.cpp
 * Purpose:
 * 	Test some interesting identities involving radicals.
 *
 * Usage:   
 * 		> ./tMoreRadicals [prec=40]
 *
 * 	where the default values are shown above.
 *
 * Restriction/Bugs:
 * 	Currently, n must be an natural number (BigInt or int)
 *
 * Author: Chee Yap, Vikram Sharma
 * Date:  August 15, 2003
 * Core Library version 1.6
 *************************************************************/

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
     Expr L, R;

	L = 3*sqrt(radical(NT(5), 3) - radical(NT(4), 3));
	R = radical(NT(2), 3) + radical(NT(20),3) - radical(NT(25),3);

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
   ***************************************************/
	L = radical(radical(NT(2),3)-1,3);	
	R = radical(NT(1)/NT(9), 3) -
		 radical(NT(2)/NT(9), 3) + radical(NT(4)/NT(9), 3); 
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
