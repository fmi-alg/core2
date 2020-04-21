/* **************************************
   File: pivotgaussianT.ccp

   Purpose:
	The code computes the determinant of a matrix using
	a simple gaussian elimination with determinants.

	This program tests the templated version of
	our linearAlgebra library (i.e., linearAlgebraT.h)

   Usage:
	pivotgaussianT <input_file> <number of iterations>

	where <input_file> contains the matrix
	  and <number_of_iterations> is the number of times
	  to run the program.

   Since CORE Library Version 1.7
   $Id: pivotgaussianT.cpp,v 1.4 2010/05/27 15:47:00 exact Exp $
************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <fstream>
#include <CORE.h>
#include <CORE/linearAlgebraT.h>

using namespace std;

typedef MatrixT<double> MatrixD;

double determinant(MatrixD& M) {
   MatrixD A = M;
   int n = A.dimension_1();
   double det;
   int i, j, k;

   for (i = 0; i < n; i++) {
	// This is the pivoting version of
	// Gaussian elimination:
      	// if exchange rows to make sure that A(i,i) != 0
      if(A(i,i) == 0)
        for (j = i +1; j < n; j++)
          if (A(j, i) != 0) {
            A.rowExchange(i, j);
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

  MatrixD m(n, n, A);
  imax = atoi(argv[2]);
  for (i=0; i<imax; i++) {	
    e = determinant(m);
#if (CORE_LEVEL==3)
    e.approx( defRelPrec, defAbsPrec ); /* to force the evaluation,
		in order to get meaningful performance comparision */
#endif
  }

  cout << "Determinant = " << setprecision(defPrtDgt) << e << endl << endl;
  return 0;
}
