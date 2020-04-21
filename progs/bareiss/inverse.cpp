/* **************************************
   File: inverse.cpp

   Usage:
	% make inverse
	% inverse

   Purpose:
	Unit test for the fraction-free form of matrix inversion.

	How to call the fraction-free inverse:
	let A, B be square matrices whose entries has the number
	type NT  (NT=Expr, int, BigInt, etc).
	Then invoke

  		NT d = A.bareissInverse(& B);

	will return the determinant of A in d, and store
	the adjoint of A into B.  Thus, the inverse of A is given by

	      	B/d  ( = adj(A)/det(A) ).

	This program runs the tests with various number types
	in each accuracy level:

		Level 3 (NT=Expr)
		Level 2 (NT=BigFloat, and NT=BigRat)
		Level 1 (NT=machine_double, and NT=DoubleWrapper)

   NOTES:
	The input matrices are hard-coded.

	We use the templated version of linear algebra
	classes found in $(CORE)/inc/CORE/linearAlgebraT.h.
	The inversion algorithm may be found there.
	
	The Fraction-Free form of Matrix Inversion is similar to
	Bareiss Algorithm.  Its theory may be found
	in "Fundamental Problems of Algorithmic Algebra",
	by C.Yap (Oxford Press, 2000), Chapter 10.

   Author: Narayan Kamath (Jun 2010)

   Since Core 2.0
   CORE Library, $Id: inverse.cpp,v 1.3 2010/06/08 15:08:22 exact Exp $
 ************************************** */

// NOTE: Level 4 is necessary for this program!

#ifndef CORE_LEVEL
#   define CORE_LEVEL 4
#endif

using namespace std;

#include "CORE/CORE.h"
#include "CORE/linearAlgebraT.h"

// run()
// is a templated function to perform a battery of tests
// The template type is a a number type called NT.
//
template <typename NT> void run() {
  NT e, f;

  NT A[] = { 512.0,  512.0,  512.0, 1.0,
                512.0, -512.0, -512.0, 1.0,
               -512.0,  512.0, -512.0, 1.0,
               -512.0, -512.0,  512.0, 1.0 };
  MatrixT<NT> m(4, 4, A);
  MatrixT<NT> Ident = identity<NT>(4); // identity matrix

  MatrixT<NT> adjoint(4);
  e = m.bareissInverse(&adjoint);
  cout << "A = " << A << endl;
  cout << "Ident = " << Ident << endl;

  cout << "Determinant of A = " << e << endl ;
  cout << "  Note: Determinant of A will overflow in CORE_LEVEL 1 \n";

  if (e == m.determinant())
     cout << "CORRECT!! Determinant of A computed by Inverse is OK \n";
  else
     cout << "ERROR!! Determinant of A computed by Inverse is bad \n";

  Ident *= e;

  if (Ident == m*adjoint)
     cout << "CORRECT!! det(A).Ident == A*adj(A)\n";
  else
     cout << "ERROR!!  det(A).Ident != A*adj(A)\n";

  cout << "-----------------" << endl;
  cout << "Determinant of A = " << e << endl ;
  cout << "Note: Determinant of A will overflow in CORE_LEVEL 1 \n";

  NT B[] = { 3.0, 0.0, 0.0, 1.0,
      0.0,   3.0, 0.0, 1.0,
      0.0,   0.0, 3.0, 1.0,
      1.0,   1.0, 1.0, 1.0 };
  MatrixT<NT> n(4, 4, B);
  f = n.bareissInverse(&adjoint);

  cout << "-----------------" << endl;
  cout << "Determinant of B = " << f << endl ;
  cout << "Note: Determinant of B should be 0, but shows non-zero in CORE_LEVEL 1\n";

  f = n.determinant();

  cout << "-----------------" << endl;
  cout << "Determinant of B = " << f << endl ;
  cout << "Note: Determinant of B should be 0, but shows non-zero in CORE_LEVEL 1\n";
  cout << "-----------------" << endl;
 
  NT C[] = { 1,4,5,10,
      15,78,90,1,
      1,3,4,5,
      1,1,1,1 };
  MatrixT<NT> p(4, 4, C);
  f = p.bareissInverse(&adjoint);
  cout << "The last row of the adjoint should be [-39, 1, 27, -3] " << endl;
  cout << "-----------------" << endl;
  cout << adjoint << endl;
  cout << "-----------------" << endl;
  cout << "Determinant (should be -257) :" << f << endl;
  cout << "-----------------" << endl;
  NT D[] = { 1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1 };
  MatrixT<NT> q(4, 4, D);
  f = q.bareissInverse(&adjoint);
  cout << "Inversion of an identity matrix :" << endl;
  cout << "-----------------" << endl;
  cout << adjoint << endl;
  cout << "-----------------" << endl;

  NT E[] = { 0,0,0,0,
             0,0,0,0,
             0,0,0,0,
             0,0,0,0 };
  MatrixT<NT> r(4, 4, E);
  f = r.bareissInverse(&adjoint);
  cout << "Inversion of a singular matrix :" << endl;
  cout << "-----------------" << endl;
  if (f == 0) {
    cout << "CORRECT ! 0 determinant returned" << endl;
  } else {
    cout << "ERROR ! non zero determinant" << endl;
  }
  cout << "-----------------" << endl;
}

int main(int argc, char **argv) {
  cout << "CORE LEVEL 3" << endl;
  cout << "=================================================" << endl;
  run<CORE::Expr>();
  cout << endl << endl << "CORE LEVEL 2 (BigFloat)" << endl;
  cout << "=================================================" << endl;
  run<CORE::BigFloat>();
  cout << endl << endl << "CORE LEVEL 2 (BigRat)" << endl;
  cout << "=================================================" << endl;
  run<CORE::BigRat>();
  cout << endl << endl << "CORE LEVEL 1 (DoubleWrapper)" << endl;
  cout << "=================================================" << endl;
  run<CORE::DoubleWrapper>();
  cout << endl << endl << "CORE LEVEL 1 (No wrapper)" << endl;
  cout << "=================================================" << endl;
  run<machine_double>();
}
