/* **************************************
   File: pivotgaussian.cc

   Purpose: This is the pivoting version of  
	Gaussian elimination to compute matrix determinants.

   Usage:
	pivotgaussian <input_file> <number of iterations>

	where <input_file> contains the matrix
	  and <number_of_iterations> is the number of times
	  to run the program.

   Since CORE Library Version 1.2
   $Id: pivotgaussian.cpp,v 1.2 2007/10/09 21:32:50 exact Exp $
************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <fstream>
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
   double determinant();
   
   void exchangerows(Matrix& m, int r1, int r2);
   int getDim() { return n; }
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

/* since we adapted new memory management code ver 1.4, the following 
   functions are not supported now.
*/
/*
extern int num_expr_new;
extern int num_ConstRep_new;
int num_inloop_new = 0;

// Debugging tool: tells us how many new objects (Expr, ExprReps, etc)
//	are created
void print_num_mallocs() {
  std::cout << num_expr_new << ", " 
	<< num_ConstRep_new << ", " 
	<< ", " << num_inloop_new << "."
	<< std::endl ;
}
*/
void Matrix::exchangerows(Matrix& m, int r1, int r2) {
  double tmp;
  Matrix&A = m;

  for(int i = 0; i < A.getDim(); i++) {
    tmp = A(r1, i);
    A(r1, i) = A(r2, i);
    A(r2, i) = tmp;
  }
}

double Matrix::determinant() {
   Matrix A = *this;
   double det;
   int i, j, k;

   for (i = 0; i < n; i++) {
	// This is the pivoting version of
	// Gaussian elimination:
      	// if exchange rows to make sure that A(i,i) != 0
      if(A(i,i) == 0)
        for (j = i +1; j < n; j++)
          if (A(j, i) != 0) {
            exchangerows(A, i, j);
            break;
          }

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
       //       double d = M(i,j);
       o << M(i, j) << " ";
       //       o << d << std::endl;
     }
      o << std::endl;
   }
   return o;
}

int readMatrix(char *filename, double **A) {
    std::ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n;
    int a, b;
    double la, lb;

    ifs >> n;

    *A = new double[n*n];

    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
	ifs >> a;
        ifs >> b;
        la = a;
        lb = b;

        (*A)[j+i*n] =  la/lb;
      }
    }
    ifs.close();
   
    return n;
}
 
double fac(int n) {
   double f = 1.0;
   for (int i=2; i<=n; i++)
       f *= i;
   return f;
}

int main( int argc, char *argv[] ) {
  int i, imax;
  double e = 0;
  defRelPrec = 100;
  int defPrtDgt = 40;
  
  if (argc != 3) {
    std::cerr << "Usage: Gaussian <input_file> <num_of_execution>" << std::endl;
    exit(1);
  }
  
  double *A;
  int n = readMatrix(argv[1], &A);

  Matrix m(n, A);
  imax = atoi(argv[2]);
  for (i=0; i<imax; i++) {	
    e = m.determinant();
#if (CORE_LEVEL==3)
    e.approx( defRelPrec, defAbsPrec ); /* to force the evaluation,
		in order to get meaningful performance comparision */
#endif
  }

  std::cout << "Determinant = " << std::setprecision(defPrtDgt) << e << std::endl << std::endl;
  return 0;
}
