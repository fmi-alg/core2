/* **************************************
   File: gaussian.ccp

   Purpose: This is a simple example using the straightforward
	Gaussian elimination to compute matrix determinants.
	It does not do pivoting -- so the input matrices
	must have non-zero principal minors! 

   Usage:
	gaussian <input_file> <number of iterations>

	where <input_file> contains the matrix
	  and <number_of_iterations> is the number of times
	  to run the program.

   Since CORE Library Version 1.2
   $Id: gaussian.cpp,v 1.8 2010/08/02 21:03:53 exact Exp $
************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <fstream>
#include "CORE.h"

using namespace std;

class SqMatrix {
private:
  int     n;
  double* _rep;

public:
   SqMatrix(int d) : n(d) { _rep   = new double [n*n]; }
   SqMatrix(int d, double M[]);
   SqMatrix(const SqMatrix&);
   const SqMatrix& operator=(const SqMatrix&);
   ~SqMatrix() { delete [] _rep; }
   const double& operator()(int r, int c) const { return _rep[r * n + c]; }
   double& operator()(int r, int c) { return _rep[r * n + c]; }
   double determinant() const;
   friend ostream& operator<<(ostream&, const SqMatrix&);
};

SqMatrix::SqMatrix(int d, double M[]) : n(d) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M[i * n + j];
}

SqMatrix::SqMatrix(const SqMatrix& M) : n(M.n) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M._rep[i * n + j];
}

const SqMatrix& SqMatrix::operator=(const SqMatrix& M) {
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
  cout << num_expr_new << ", " 
	<< num_ConstRep_new << ", " 
	<< ", " << num_inloop_new << "."
	<< endl ;
}
*/

double SqMatrix::determinant() const {
   SqMatrix A = *this;
   double det;
   int i, j, k;

   for (i = 0; i < n; i++) {
	// This is the non-pivoting version of
	// Gaussian elimination:
      	// assert(A(i,i) != 0);
      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
	    if (A(i,i)==0) { // simply catch zero pivot and abort!
		  cout << "Zero Pivot Element! Abort" << endl;
		  return(0);
	    }
            A(j,k) -= A(j,i)*A(i,k)/A(i,i);
         }
   }

   det = 1.0;
   for (i = 0; i < n; i++)
      det *= A(i,i);

   return det;
}

ostream& operator<<(ostream& o, const SqMatrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.n; j++) {
       //       double d = M(i,j);
       o << M(i, j) << " ";
       //       o << d << endl;
     }
      o << endl;
   }
   return o;
}

int readMatrix(char *filename, double **A) {
    ifstream ifs(filename);
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
    cerr << "Usage: Gaussian <input_file> <num_of_execution>" << endl;
    exit(1);
  }
  
  double *A;
  int n = readMatrix(argv[1], &A);

  SqMatrix m(n, A);
  imax = atoi(argv[2]);
  for (i=0; i<imax; i++) {	
    e = m.determinant();
#if (CORE_LEVEL==3)
    e.approx( defRelPrec, defAbsPrec ); /* to force the evaluation,
		in order to get meaningful performance comparision */
#endif
  }

  cout << "Determinant = " << setprecision(defPrtDgt) << e << endl << endl;
  return 0;
}
