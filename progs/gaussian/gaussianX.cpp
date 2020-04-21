/* **************************************
   File: gaussianX.cc

   Purpose: This is a simple example using the straightforward
	Gaussian elimination to compute matrix determinants.
	It does not do pivoting -- so the input matrices
	must have non-zero principal minors! 

	The input matrix is hardcoded here

   Usage:
	% gaussianX

   Since CORE Library Version 1.2
   $Id: gaussianX.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

class Matrix {
private:
  int     n;
  double* _rep;

public:
   Matrix(int d) : n(d) { _rep   = new double [n*n]; }
   Matrix(int d, double M[]);
   Matrix(const Matrix&);
   const Matrix& operator=(const Matrix&);
   ~Matrix() { delete [] _rep; }
   const double& operator()(int r, int c) const { return _rep[r * n + c]; }
   double& operator()(int r, int c) { return _rep[r * n + c]; }
   double determinant() const;
   friend std::ostream& operator<<(std::ostream&, const Matrix&);
};

Matrix::Matrix(int d, double M[]) : n(d) {
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
   int i, j, k;

   for (i = 0; i < n; i++) {
      // assert(A(i,i) != 0);

      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
            A(j,k) -= A(j,i)*A(i,k)/A(i,i);
         }
   }

   det = 1.0;
   for (i = 0; i < n; i++)
      det *= A(i,i);

   return det;
}

std::ostream& operator<<(std::ostream& o, const Matrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.n; j++) {
      o << M(i, j) << " ";
     }
      o << std::endl;
   }
   return o;
}

int main() {
  double e, f;

  double A[] = { 512.0,  512.0,  512.0, 1.0,
 	       512.0, -512.0, -512.0, 1.0,
 	       -512.0,  512.0, -512.0, 1.0,
 	       -512.0, -512.0,  512.0, 1.0 };
  Matrix m(4, A);
  e = m.determinant();
  std::cout << "Matrix A = \n" << m << std::endl;
  std::cout << "Determinant of A = " << e << std::endl << std::endl;

  double B[] = { 3.0,  0.0,  0.0, 1.0,
	       0.0, 3.0, 0.0, 1.0,
	       0.0,  0.0, 3.0, 1.0,
	       1.0, 1.0,  1.0, 1.0 };
  Matrix n(4, B);
  f = n.determinant();
  std::cout << "Matrix B = \n" << n << std::endl;
  std::cout << "Determinant of B = " << f << std::endl << std::endl;
  
  return 0;
}

