/* **************************************
   File: bareiss.cpp

   Purpose: Implementation of the Bareiss algorithm for
	computing determinants of matrices.  

	This version performs pivoting
	(cf. Gaussian.cc in ${CORE}/progs/gaussian)
	The book "Fundamental Problems of Algorithmic Algebra",
	by C.Yap (Oxford U.Press, 2000) treats this algorithm.

	Input matrices are hard-coded.

	We include a simple implementation of the Matrix class.
	   But see $(CORE)/ext/linearAlgebra.h and
	   $(CORE)/inc/CORE/linearAlgebraT.h for more complete
	   implementations for use in Core Library.

   Usage:
	% bareiss

   Since Core 1.7
   CORE Library, $Id: bareiss.cpp,v 1.2 2010/06/08 11:43:41 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

class Matrix {
private:
  int     n;
  double* _rep;

public:
   Matrix(int d) : n(d) { _rep   = new double [n*n]; }
   Matrix(int d, double M[]);
   Matrix(int d, int M[]);
   Matrix(const Matrix&);
   const Matrix& operator=(const Matrix&);
   ~Matrix() { delete [] _rep; }
   const double& operator()(int r, int c) const { return _rep[r * n + c]; }
   double& operator()(int r, int c) { return _rep[r * n + c]; }
   double determinant() const;
   friend ostream& operator<<(ostream&, const Matrix&);
   int size() const;
};

int Matrix::size() const {return n;}

Matrix::Matrix(int d, double M[]) : n(d) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M[i * n + j];
}
Matrix::Matrix(int d, int M[]) : n(d) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M[i * n + j];
}

Matrix::Matrix(const Matrix& M) : n(M.n) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M._rep[i * n + j];
}

const Matrix& Matrix::operator=(const Matrix& M) {
   int i, j;
   if (n != M.n) {
      delete [] _rep;
      n = M.n;
      _rep = new double [n*n];
   }
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M._rep[i * n + j];
   return *this;
}

double Matrix::determinant() const {
   Matrix A = *this;
   double det;
   int i, j, k, c, tmp;

   int* p;
   p = new int [A.size()]; // p[1..n] is a permutation of the columns
			    // of matrix A, used for pivoting
   for (i = 0; i< n; i++)
	p[i]=i;		    // p is initially the identiy permutation

   // OUTER LOOP:
   for (i = 0; i < n-1; i++) {

      // PIVOT:
      for (c=i; c< n+1; c++)  // search for non-zero entry a(i,c)
	if (c==n || A(i,p[c]) != 0) break;
      if (c==n) {
	cout << "Input matrix is singular!!" << endl;
	return 0;
      } else { // swap the i-th and c-th column:
	tmp = p[i]; p[i] = p[c]; p[c] = tmp;
      }
      // assert(a(i, p[i]) == 0);

      // INNER LOOPS:
      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
            A(j,p[k]) = (A(j,p[k])*A(i,p[i])-A(j,p[i])*A(i,p[k]));
	    if (i) A(j, p[k]) /= A(i-1,p[i-1]);
         }
   }

   return A(n-1,p[n-1]);
}

ostream& operator<<(ostream& o, const Matrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.n; j++) {
       double d = M(i,j);
       o << M(i, j) << " ";
       o << d << endl;
     }
      o << endl;
   }
   return o;
}

int main( int argc, char *argv[] ) {
  double e, f;

  double A[] = { 512.0,  512.0,  512.0, 1.0,
                512.0, -512.0, -512.0, 1.0,
               -512.0,  512.0, -512.0, 1.0,
               -512.0, -512.0,  512.0, 1.0 };
  Matrix m(4, A);
  e = m.determinant();
  cout << "Determinant of A = " << e << endl ;
  cout << "  Note: Determinant of A will overflow in CORE_LEVEL 1 \n\n";

  double B[] = { 3.0, 0.0, 0.0, 1.0,
               0.0,   3.0, 0.0, 1.0,
               0.0,   0.0, 3.0, 1.0,
               1.0,   1.0, 1.0, 1.0 };
  Matrix n(4, B);
  f = n.determinant();
  cout << "Determinant of B = " << f << endl;
  cout << "  Note: Determinant of B is 0, but shows non-zero in CORE_LEVEL 1\n\n";
  
  if (f == 0) cout << "CORRECT!! Determinant of B is 0" << endl;
  else cout << "ERROR!! Determinant of B is non-zero" << endl; 
  return 0;

}

