/************************************************************
  file: compare.cpp
  purpose:
	demonstrate the (non)progressive evaluation features
	and their effects on speed of comparing expressions
  Usage:
	compare [B] [N]

	where B is the number of bits in random numbers to test
	and N is number of times to run the test.
	Both arguments are optional, and defaults to B=10, N=1

  author: Chen Li and Chee Yap
  Since CORE Library version 1.4
  $Id: compare.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
************************************************************/
 
#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include <ctime>
#include "CORE.h"

int main(int argc, char* argv[]) {
  double x;
  double x1, x2;
  double y;
  double y1, y2;

  int L = 10, N = 1;
  if (argc > 2) {
    L = atoi(argv[1]);
    N = atoi(argv[2]);
  } else if (argc > 1) {
    L = atoi(argv[1]);
  } else {
    std::cerr << "Usage: compare <L_bits> <N_times>" << std::endl;
    exit(1);
  }

  //setIncrementalEvalFlag(false);
  unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
  srand(seed1);

  BigInt BASE = 1;
  BASE <<= (L - 1);

  machine_long t1=0, t2=0, total=0;
  BigRat r1, r2;
  
  for (int i = 0; i<N; i++) {
    x1 = BASE + randomize(BASE);
    x2 = BASE + randomize(BASE);
    y1 = BASE + randomize(BASE);
    y2 = BASE + randomize(BASE);
    x = x1 / x2;
    y = y1 / y2;
    std::cout << "\t x (" << x1 << "/" << x2 << ") " << std::endl;
    std::cout << "\t y (" << y1 << "/" << y2 << ") " << std::endl;

    double e = sqrt(x) + sqrt(y);
    double f = sqrt(x + y + 2 * sqrt(x * y));

    t1 = clock();
    std::cout << "\t e == f ? ";
    std::cout.flush();
    std::cout << ((e == f) ?  "yes (CORRECT)" : "no (INCORRECT)") << std::endl;
    t2 = clock();
    std::cout << "e = " << e << ", f = " << f << std::endl;
    total += (t2 - t1);
  }
  std::cout << "Total USER time spent: " << (float)total / CLOCKS_PER_SEC 
	<< " seconds" << std::endl;
  std::cout << "Avg. USER time spent: " << (float)total / (CLOCKS_PER_SEC * N) 
	<< " seconds" << std::endl;
  return 0;
}
