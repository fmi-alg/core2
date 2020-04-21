/* **************************************
   File: createMatrices.cpp

   Purpose:
	To generate random matrices for testing.
	Each entry is given as a pair of integers.
	The outfile format has the following format:

		line 1: N = number of matrices
		line 2: d = dimension of matrices

	Subsequent lines has a pair of integers.
	These pairs represent a single entry, and the entries
	are listed in row-major order.

   Usage:
        % createMatrices fname N d b

   where
	 fname = output file name
	 N     = the number of matrices
	 d     = dimension of the matrices
 	 b     = bit length of entries

   Since CORE Library 1.4
   $Id: createMatrices.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <ctime>
#include <fstream>
#include "CORE.h"

int main( int argc, char *argv[] ) {
  int matrices;
  int dim;
  int bitLen;
  char filename[256];

  if (argc != 5) {
    std::cout << "Usage: createMatrices "
	 << " <output_file> <No_of_Matrices> <Dimension> <Bit_Length>" 
         << std::endl;
    exit(1);
  }

  strcpy(filename, argv[1]);
  matrices = atoi(argv[2]);
  dim = atoi(argv[3]);
  bitLen = atoi(argv[4]);

  std::ofstream ofs(filename);
  if ( !ofs ) {
    std::cout << "Cannot open the output file!" << std::endl;
    exit(1);
  }
  
  std::cout << "Generate " << matrices << " random matrices..." << std::endl;

  // write the total number of randomly generated matrices
  ofs << matrices << std::endl;

  // write the dimension of the matrix
  ofs << dim << std::endl;
  ofs << std::endl;

  // write the matrices
  BigInt x, y;

  BigInt BASE1 = 1;
  BASE1 <<= (bitLen - 1);

  BigInt BASE2 = 1;
  BASE2 <<= bitLen;
  
  unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
  srand(seed1);

  for (int k=0; k< matrices; k++) {
    for (int i=0; i<dim*dim; i++) {
      x = BASE1 + randomize(BASE1);
      y = BASE2 + randomize(BASE2);
      ofs << x << " " << y << std::endl;
    }
    ofs << std::endl;
  }
  ofs.close();
  std::cout << "Done." << std::endl;
  return 0;
}
