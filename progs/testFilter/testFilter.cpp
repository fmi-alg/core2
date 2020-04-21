/* ********************************************************************
   File: testFilter.cpp

   Purpose: This program is an adaptation of gaussian.ccp
   	to test the effectiveness of our implementation
	of the BFS filter.  
	
	Please see the README file in $(CORE_PATH)/progs/gaussian 
	for details about the original program.

   Usage:
	% testFilter <input_file> 

	where 
	
	<input_file> is the name of an ascii file containing <n> 
	matrices, each of dimension <d>.  The first two integers in
	in <input_file> are <n> and <d>.  The remaining entries
	contains the entries of each of the matrices in the usual row
	major order.  In particular, the program "createMatrices"
	produce such input files.

	A sample input file named "inputs/1000x4matrices"
	may be found in this directory.

   Author: Sylvain Pion

   Since CORE Library Version 1.4
   $Id: testFilter.cpp,v 1.2 2010/06/12 07:52:11 exact Exp $
 ******************************************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <ctime>
#include <fstream>
#include "CORE.h"

using namespace std;

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
   double determinant(int k) const;
   friend ostream& operator<<(ostream&, const Matrix&);
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

// This is the pivoting version of  Gaussian elimination:
//
double Matrix::determinant(int ID) const {
   Matrix A = *this;
   double det;
   int * permutation = new int[n];
   int swap_counter = 0;
   int i, j, k;

   // init permutation
   for (i = 0; i < n; i++) 
	   permutation[i]=i;

   // main loop
   for (i = 0; i < n-1; i++) {

      //  pivot test, want (A(i,i) != 0);
      int pivot = i;
      while (pivot < n && A(i, permutation[pivot]) == 0)
	 pivot++;

      // check if determinant is 0:
      if (pivot == n) {
	      delete[] permutation;
	      return 0;
      }

      // check if permutation needs update:
      if (pivot != i) {
	      swap(permutation[i], permutation[pivot]);
	      swap_counter++;
      }

      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
            A(j,permutation[k]) -= A(j,permutation[i]) *
		    	A(i,permutation[k])/A(i,permutation[i]);
         }
   }

   det = 1.0;
   for (i = 0; i < n; i++)
      det *= A(i,permutation[i]);

   if ((swap_counter & 1) != 0) 
	   det = -det;

   delete[] permutation;
   return det;
}

ostream& operator<<(ostream& o, const Matrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.n; j++) {
       o << M(i, j) << " ";
     }
     o << endl;
   }
   return o;
}

void testing(char* filename) {

  machine_long u1=0, u2=0, total1=0, total2=0;

  int number_of_matrices;
  int dim = 0;

  ifstream ifs(filename);
  if ( !ifs ) {
    cout << "Cannot open the input file!" << endl;
    exit(1);
  }

  ifs >> number_of_matrices;
  ifs >> dim;

  double x, y;
  double* A = new double[dim*dim];
  double* B = new double[dim*dim];

  int sign1, sign2;
  int count_errors = 0;	// count_errors= number of different signs 
  

  for (int k=0; k< number_of_matrices; k++) {
    for (int i=0; i<dim*dim; i++) {
      ifs >> x;
      ifs >> y;
      A[i] = x/y;
      B[i] = x/y;
    }
    Matrix m1(dim, A);
    Matrix m2(dim, B);

    setFpFilterFlag(true);
    u1 = clock();
    sign1 = (m1.determinant(k)).sign();
    u2 = clock();

    total1 += (u2 - u1);

    setFpFilterFlag(false);
    u1 = clock();
    sign2 = (m2.determinant(k)).sign();
    u2 = clock();

    total2 += (u2 - u1);

    if (sign1 != sign2) {
      count_errors ++;
      cout << "ID = " << k << endl;
    }
  }//for k

  ifs.close();

  if (count_errors > 0)
    cout << "INCORRECT! " << count_errors << " error(s) found." << endl;
  else
    cout << "CORRECT!" << endl;

  cout << "T1 = Total USER time spent w/ filter : " << \
	((float)total1) / CLOCKS_PER_SEC << " seconds" << endl;

  cout << "T2 = Total USER time spent w/o filter : " << \
	((float)total2) / CLOCKS_PER_SEC << " seconds" << endl;

  cout << "Speedup = 100*(T2-T1)/T1 %: " <<
	(float)((total2-total1)*100)/total1 << "%" << endl;

  return ;
}

int main( int argc, char *argv[] ) {
  char filename[256];
 
  if (argc != 2) {
    cout << "Usage: testFilter <input_file>" << endl;
    exit(1);
  }
  
  strcpy(filename, argv[1]);

  testing(filename); 

  return 0;
}
