/* **************************************
   File: Curves.h

   Description: 
	The templated class Sturm implements Sturm sequences.
	Basic capabilities include:
		counting number of roots in an interval, 
		isolating all roots in an interval
		isolating the i-th largest (or smallest) root in interval
	It is based on the Polynomial class.

   BUGS and TODO LIST:
	The copy constructor seems to fail
	Should have a test for square-free polynomials

   Author:  Chee Yap and Sylvain Pion
   Date:    July 20, 2002

   Since Core Library  Version 1.5
   $Id: Curves.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

// ==================================================
// Curve Class
// ==================================================

typedef Expr NT;
typedef std::vector<Polynomial<NT> >	VecPoly;

template < class NT >
class Curve {
public:
  int ydeg;		// y-degree of the curve 
  VecPoly  vPoly;	// array of (1+ydeg) polynomials 

  		// So the equation of the curve is  C(x,y)
		//  = (y^d * vPoly[d]) + (y^{d-1} * vPoly[d-1]) +...+ (vPoly[0]).

  // CONSTRUCTORS
  Curve() {		// null constructor
    ydeg = 0;
  }

  Curve(VecPoly vp) {	// construct from a vector of polynomial
    vPoly = vp;
    ydeg = vp.size() - 1;
  }

  // METHODS

  // dump functions
  void dump(string msg) const {
    cerr << msg << endl;
    cerr << "Equation of Curve is:" << endl;
    for (int i=0; i <= ydeg; i++)
      cerr << " vPoly[" << i << "] = " << vPoly[i] << endl;
  }
  // dump without message
  void dump() const {
    dump("");
  }

  // The polynomial (in y) of the Curve when x is fixed
  Polynomial<NT> yPolynomial(const Expr & x) {
	VecExpr vE;

	for (int i=0; i<= ydeg; i++) {
		vE.push_back(vPoly[i].eval(x));
	}

	return Polynomial<NT>(vE);
  }

  void verticalIntersections(const Expr & x, BFVecInterval & vI) {
	Sturm<NT> SS(yPolynomial(x));
	SS.isolateRoots(vI);
  }
  
};// Curve class

int main()
{
	VecPoly vP;
	NT coeff0[] = {2};
	NT coeff3[] = {0, 0, 0, -1};
	vP.push_back( Polynomial<NT>(3, coeff3) );	// coeff of 1
	vP.push_back( Polynomial<NT>::polyZero() );	// coeff of y
	vP.push_back( Polynomial<NT>(0, coeff0) );	// coeff of y^2

	// THIS is the equation x^3 - 2y^2 = 0.
	// It is called the Cissoid of Diocles [Brieskorn-Knorrer, p.9]
	//
	Curve<NT> C(vP);

C.dump();

	BFVecInterval vI;

	double eps = 0.1;
	for (int i=0; i<10; i++) {
		C.verticalIntersections(eps*i, vI);

	   	cout << " Number of roots : " << vI.size() << endl;

		for (BFVecInterval::const_iterator it = vI.begin();
	             it != vI.end(); ++it) {
	      	cout << " Root in [" << it->first << " ; "
	                           << it->second << "]" << endl;
    		}
		vI.clear();
	}//for
}

