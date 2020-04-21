/* ********************************************************************
   File: timeFilter.cc

   Purpose: This program is an adaptation of gaussian.cc
   	to test the effectiveness of our implementation
	of the BFS filter.  
	
	Please see the README file in $(CORE_PATH)/progs/gaussian 
	for details about the original program.

   Usage:
	% timeFilter <input_file> 

	where 
	
	<input_file> is the name of an ascii file containing <n> 
	matrices, each of dimension <d>.  The first two integers in
	in <input_file> are <n> and <d>.  The remaining entries
	contains the entries of each of the matrices in the usual row
	major order.  In particular, the program "createMatrices"
	produce such input files.

	A sample input file named "inputs/1000x4matrices"
	may be found in this directory.

   Since CORE Library Version 1.4
   $Id: timeFilter1.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ******************************************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <ctime>
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
	// This is the non-pivoting version of
	// Gaussian elimination:
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

int timing(char* filename, bool filterFlag) {

  machine_long u1=0, u2=0, total=0;

  int number_of_matrices;
  int dim;

  std::ifstream ifs(filename);
  if ( !ifs ) {
    std::cout << "Cannot open the input file!" << std::endl;
    exit(1);
  }

  ifs >> number_of_matrices;
  ifs >> dim;

  double x, y;
  double e;
  double* A = new double[dim*dim];

  int sign;
  int count[3] = {0, 0, 0};
	// count[0]= number of negative determinants, etc
  
  // do test with filter flag
  setFpFilterFlag(filterFlag);

  for (int k=0; k< number_of_matrices; k++) {
    //std::cout << "k=" << k << ": ";
    for (int i=0; i<dim*dim; i++) {
      ifs >> x;
      ifs >> y;
      //std::cout << "x=" << x << ", y=" << y << std::endl;
      A[i] = x; // we only take numerator.
    }

    // assign A to the matrix 
    Matrix m(dim, A);

    u1 = clock();
    sign = (m.determinant()).sign();
    u2 = clock();

    total += (u2 - u1);

    count[1 + sign]++;  // how many determinants of each sign is produced

  }

  ifs.close();

  std::cout << " Number of (-, 0, +) determinants = (" 
  	<< count[0] << ", " << count[1] << ", " << count[2] << ")" << std::endl;

  return total;
}

int main( int argc, char *argv[] ) {
  char filename[256];
  int total1, total2;
 
  if (argc != 2) {
    std::cout << "Usage: timeFilter <input_file>" << std::endl;
    exit(1);
  }
  
  strcpy(filename, argv[1]);

  total1 = timing(filename, true);
  total2 = timing(filename, false); 

  std::cout << "Total USER time spent w/ filter : " << \
	((float)total1) / CLOCKS_PER_SEC << " seconds" << std::endl;

  std::cout << "Total USER time spent w/o filter : " << \
	((float)total2) / CLOCKS_PER_SEC << " seconds" << std::endl;

  std::cout << "Speedup: " << (total2-total1)*100/total2 << "%" << std::endl;

  return 0;
}
