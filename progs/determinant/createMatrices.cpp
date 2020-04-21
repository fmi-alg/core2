/* **************************************
   File: createMatrices.cpp

   Purpose:
	To generate random matrices for testing.
	Each entry is given as a pair of integers representing a binary rational
	The outfile format has the following format:

		line 1: N = number of matrices
		line 2: d = dimension of matrices

	Subsequent lines has a pair of integers.
	These pairs represent a single entry, and the entries
	are listed in row-major order.
	Each pair (m, L) represents m*2^{-L}.

   Usage:
        % createMatrices fname N d b

   where
	 fname = output file name
	 N     = the number of matrices
	 d     = dimension of the matrices
 	 b     = bit length of entries

   CORE Library,
   $Id: createMatrices.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 ************************************** */

#include <ctime>
#include <fstream>
#include "CORE.h"

int main( int argc, char *argv[] ) {
  int no_matrices;
  int dim;
  int digitLen;
  int base;
  char filename[256];

  if (argc != 5 && argc != 6) {
    std::cout << "Usage: createMatrices "
	 << " <output_file> <No_of_Matrices> <Dimension> <Bit_Length> <Base=2>" 
         << std::endl;
    exit(1);
  }

  strcpy(filename, argv[1]);
  no_matrices = atoi(argv[2]);
  dim = atoi(argv[3]);
  digitLen = atoi(argv[4]);
  base = (argc == 6) ? atoi(argv[5]) : 2;

  std::ofstream ofs(filename);
  if ( !ofs ) {
    std::cout << "Cannot open the output file!" << std::endl;
    exit(1);
  }
  
  std::cout << "Generate " << no_matrices << " random matrices..." << std::endl;

  // write the total number of randomly generated matrices
  ofs << no_matrices << std::endl;

  // write the base
  ofs << base << std::endl;

  // write the dimension of the matrix
  ofs << dim << std::endl;
  ofs << std::endl;

  // write the matrices
  BigInt x, y;

  BigInt BASE1 = 1;
  power(BASE1, BigInt(base), (digitLen-1));

  unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
  srand(seed1);

  for (int k=0; k< no_matrices; k++) {
    for (int i=0; i<dim*dim; i++) {
      x = BASE1 + randomize(BASE1);
      y = randomize(digitLen);
      ofs << x << " " << y << std::endl;
    }
    ofs << std::endl;
  }
  ofs.close();
  std::cout << "Done." << std::endl;
  return 0;
}
