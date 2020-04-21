/* tMultRoots.cpp
 *
 *  Created on: Nov 17, 2010
 *      Author: cheeyap
 *
 * Experiments with automatically detect multiplicity.
 *
 * Calling sequence:
 *
 * 	> tMultRoots [n=2] [p(x)="x^2-x-1"] [x0="1.618"]
 *
 * will do Newton iteration for the roots of polynomial p(x)^n.
 * If x0 = 0, then we do all roots.
 * If x0 is non-zero, we will only do Newton Iteration for
 * one of the roots, starting from r0.
 *
 * E.g., 
 * 	> tMultRoots 3 
 *
 * will do Newton iteration on the polynomial (x^2 -x -1)^3 starting from
 * x0=1.618 (this converges to a root of multiplicity 3).
 *
 * 	> tMultRoots 3 "x^3 - 2x + 2" 1
 *
 * will do Newton iteration on Smale's polynomial which does not
 * converge when x0 is in the neighborhood of 0 and 1.
 *
 *
 * COMMENTS:
 *
 * (1) I see that Newton does not keep doubling the number of
 * 	bits.  It probably means our computation is truncated
 * 	at some arbitrary precision.  MUST fix.
 * (2) I believe our rule for deciding when to increase the
 * 	multiplicity is not full proof.  Must prove rigorously.
 *
 * (3) ToDo: We should try to combine with qir method of Kerber-Abbott.
 *
 * (4) ToDo: try to truncate the coefficients of p(x) ala Sagraloff
 *
 * (5) ToDo: Combine with EVAL
 *
 * (6) Run with different choices of NT
 *
 * -- Chee and Vikram (Chennai, Nov 17, 2010)
 *  
 *************************************************** */
#define CORE_LEVEL 4
#include "CORE/CORE.h"
//#include "readpoly.h"
#include "CORE/poly/Descartes.h"

using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

PolyNT diff(PolyNT & PP) {	// self-differentiation
  int deg = PP.degree();
  std::cout << "inside differentiate" << std::endl;
  NT * c = new NT[deg];
  if (deg >= 0) {
    for (int i=1; i<=deg; i++)
      c[i-1] = PP.coeff()[i] * i;
    deg--;
    std::cout << "done set" << std::endl;
    // This fixes a memory leak, note that set( ) makes a copy
    // of c and does not use c by itself.
  }
  return PolyNT(deg, c);
}// differentiation


int main(int argc, char* argv[]) {

  int pow = 2;
  if (argc>1) pow = atoi(argv[1]);

  PolyNT p = "(x^2 -x -1)";
  p.power(pow);

  cout << "p=" << p.toString() << endl;
  cout << "multiplicity, m = " << pow << endl;

  PolyNT r = p.sqFreePart();
  cout << "r=" << r.toString() << endl;

  PolyNT q;
  q=diff(p);
  cout << "q = diff(p) = " << q.toString() << endl;

  BigFloat xold, x,xprime, fx, fprimex, delx, olddelx;
  BigFloat yold, y,yprime, fy, fprimey, dely, olddely;

  x = xprime = y = yprime = "1.618";
  cout << "xprime=" << xprime << endl;

  cout.precision(40);
  BigFloat m(1.0);

  for (int i=0; i< 20; i++){
	  xold = x; yold = y;
	  x = xprime; y = yprime;
	  fx = p.eval(x); fy = p.eval(y);
	  fprimex = q.eval(x); fprimey = q.eval(y);
	  olddelx = delx; delx = fx/fprimex;
	  olddely = dely; dely = fy/fprimey;
	  if (delx.lMSB() > 1+ dely.uMSB()) {
	    m += 1.0; cout<< "m increased to : " << m << endl;
	    cout<< "delx.lMSB,uMSB = " << delx << ", " << delx.lMSB() << ", " << delx.uMSB()
	      << ",  dely.lMSB,uMSB = " << dely << ", " << dely.lMSB() << ", " << dely.uMSB() << endl;
	  }
	  //
	  xprime = x - delx * m;
	  yprime = y - dely * (m+1.0);
	  cout << "(delx, uMSB) = " << delx << ", " << delx.uMSB()
		  << ",  (dely, lMSB) = " << dely << ", " << dely.lMSB()<<endl;
	  cout << "xprime = " << xprime << ",  yprime = " << yprime <<endl;
  }

  cout<< "FINAL value of m : " << m << endl;
  cout <<"CHECK, using Sq free poly" << endl;
  PolyNT pp="x^2 -x -1"; PolyNT qq="2x-1";
  xprime = "1.6180339887";
  for (int i=0; i< 3; i++){
	  x = xprime;
	  fx = pp.eval(x);
	  fprimex = qq.eval(x);
	  xprime = x - fx/fprimex;
	  cout << "xprime = " << xprime <<endl;
  }
  return 0;
}


