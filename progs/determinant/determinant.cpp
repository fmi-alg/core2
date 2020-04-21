/* ********************************************************************
 File: determinant.cpp

   Purpose: 
   	Compares the computation of determinants using 3 methods:
		1) Gaussian elimination
		2) Factorial expansion method
		3) Dynamic programming method

	The results of this comparison is reported in the paper

	"Constructive Root Bound Method for k-Ary Rational Input Numbers"
		by Sylvain Pion and Chee Yap,
        	To appear, Proc. 19th ACM Symp. Comp.Geom., June 2003.

   Usage:
	% determinant <input_file> 

	where 
	
	<input_file> is the name of an ascii file containing <n> 
	matrices, each of dimension <d>.  The first two integers in
	in <input_file> are <n> and <d>.  The remaining entries
	contains the entries of each of the matrices in the usual row
	major order.  In particular, the program "createMatrices"
	produce such input files.

	A sample input file named "inputs/1000x4matrices"
	may be found in this directory.

   Author:
   	Sylvain Pion 

   $Id: determinant.cpp,v 1.8 2010/06/25 10:15:49 exact Exp $
 ******************************************************************** */


#include <fstream>
#include <CORE.h>
#include "CORE/linearAlgebra.h"
#include "determinant.h"

using namespace CORE;
using namespace std;

double factorial_determinant(const Matrix & A) {
  int dim = A.dimension_1();
  if (dim == 1)
    return A(0,0);

  double det = 0;
  int parity = 1;
  Matrix B(dim-1,dim-1);
  for (int i=0; i<dim; ++i) {
    for (int j=0; j<dim; j++) {
      if (j==i)
        continue;
      for (int k=0; k<dim-1; k++)
        B(j>i ? j-1 : j, k) = A(j, k+1);
    }
    if (parity>0)
      det += factorial_determinant(B) * A(i, 0);
    else
      det -= factorial_determinant(B) * A(i, 0);
    parity = -parity;
  }
  return det;
}

double gaussian_determinant(Matrix A)
{
   int dim = A.dimension_1();
   int *permutation = new int[dim];
   int swap_counter = 0; // Number of swaps done.
   int i;

   // Initialize the permutation of rows to Identity
   for (i = 0; i < dim; i++)
     permutation[i] = i;

   // This is the pivoting version of Gaussian elimination:
   for (i = 0; i < dim-1; i++) {

      // Look for a non-zero pivot.
      int pivot = i;
      while (pivot < dim && A(i, permutation[pivot]) == 0)
        pivot++;

      // All candidates are zero => det == 0
      if (pivot == dim) {
	delete[] permutation;
	return 0;
      }

      // Do we need to swap two rows ?
      if (pivot != i) {
        swap(permutation[i], permutation[pivot]);
	swap_counter++;
      }

      for (int j = i + 1; j < dim; j++)
         for (int k = i + 1; k < dim; k++) {
	    assert(A(i, permutation[i]) != 0);
            A(j, permutation[k]) -= A(j, permutation[i]) * A(i, permutation[k])
		                  / A(i, permutation[i]);
         }
   }

   double det = 1;
   for (i = 0; i < dim; i++)
      det *= A(i, permutation[i]);

   if ((swap_counter & 1) != 0) // Odd number of swaps
     det = -det;

   delete[] permutation;
   return det;
}

enum Method {Gaussian, Dynamic, Factorial, All};

int main(int argc, char **argv)
{
  defRelPrec = 100;
  defAbsPrec = CORE_INFTY;
  int defPrtDgt = 40;
  int base;
  Method method;

  // setProgressiveEvalFlag(false);

cout << "start..." << endl;

  if (argc != 3 && argc != 4) {
    cerr << "Usage: determinant <input_file> <num_of_execution> "
	         "<0,default=gaussian, 1=dynamic_programming, 2=factorial, other=all>"
		 << endl;
    exit(1);
  }

  if (argc < 4) {
    method = All;
  }
  else if (atoi(argv[3]) == 0) {
    method = Gaussian;
  }
  else if (atoi(argv[3]) == 1) {
    method = Dynamic;
  }
  else if (atoi(argv[3]) == 2) {
    method = Factorial;
  }
  else 
    method = All;

cout << "reading input file" << endl;

  ifstream ifile(argv[1]);
  int number_of_matrices;
  ifile >> number_of_matrices; //throw it away.
  ifile >> base;
  int dim;
  ifile >> dim;
  double *A = new double[dim*dim];
  for (int i=0; i<dim; i++)
    for (int j=0; j<dim; j++) {
	    int L;
	    BigInt bi;
	    ifile >> bi;
//cout << "bi=" << bi << endl;
	    ifile >> L;
	    BigInt den; pow(den, base, L);
	    BigRat br (bi, den);
	    A[i*dim+j] = br;
//cout << "br=" << br << endl;
    }

//cout << A[0] << endl;
cout.flush();
  Matrix M(dim, dim, A);

  int iterations = atoi(argv[2]);
  double e, e1, e2;
  e1=0; e2=0;

  // cout << M << endl; // without the print, I get crashes in gaussian.  Why ???

  switch (method) {
  case Gaussian:
	  cout << " Using Gaussian elimination" << endl; break;
  case Dynamic:
	  cout << " Using Dynamic programming" << endl; break;
  case Factorial:
	  cout << " Using Factorial expansion" << endl; break;
  case All:
	  cout << " Using all 3 methods " << endl;
  }

  for (int i=0; i<iterations; i++) {
    switch (method) {
    case Gaussian:
	    e = gaussian_determinant(M); break;
    case Dynamic:
	    e = dynamic_programming_determinant(M); break;
    case Factorial:
	    e = factorial_determinant(M); break;
    case All:
	    e = gaussian_determinant(M); 
	    e1 = dynamic_programming_determinant(M); 
	    e2 = factorial_determinant(M); 
    }//switch
  }//for

//    (void) sign(e); // the cast is to kill warning
#if (CORE_LEVEL==3)
//    e.approx( defRelPrec, defAbsPrec );
    /* to force the evaluation, in order to get meaningful performance
     * comparison */
#endif
  
  // cout << "Matrix : " << endl << M << endl; // takes time
  if(method==All) {
    cout << "Determinant root bound from Gaussian elimination = "
  	   << e.rep()->get_constructive_bound() << endl;
    cout << "Determinant root bound from dynamic programming  = "
  	   << e1.rep()->get_constructive_bound() << endl;
    cout << "Determinant root bound from factorial expansion  = "
  	   << e2.rep()->get_constructive_bound() << endl;
    cout << "Determinant from Gaussian = "
         << setprecision(defPrtDgt) << e << endl;
    cout << "Determinant from dynamic  = "
         << setprecision(defPrtDgt) << e1 << endl;
    cout << "Determinant from factorial= "
         << setprecision(defPrtDgt) << e2 << endl;
  } else {
    cout << "Determinant root bound = "
  	 << e.rep()->get_constructive_bound() << endl;
    cout << "Depth of the exprression tree = " << e.rep()->getDepth()
         << endl;
    cout << "Determinant = " << setprecision(defPrtDgt) << e
         << endl << endl;
  }
#ifdef COMPARE_ROOT_BOUNDS
  cout << " BFMSS   winner count = " << BFMSS_counter << endl;
  cout << " Measure winner count = " << Measure_counter << endl;
  cout << " Cauchy  winner count = " << Cauchy_counter << endl;
  cout << " Li/Yap  winner count = " << LiYap_counter << endl;

  cout << " Root Bound has been hit " << rootBoundHitCounter << " times" << endl;
#endif

  return 0;
}
