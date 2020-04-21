/* ************************************************
   File: bench.cpp

   Purpose:  Bench is used to print out the root bounds for
             various expressions:

		E_1: sqrt(x) + sqrt(y) - sqrt(x+y + 2*sqrt(x*y))
		E_2: (sqrt(x) - sqrt(y))/(x-y)

	      Then bench_1 computes the root bound for E_1.
	      Then bench_2 computes the root bound for E_2 - E_2.

   Usage:
        % bench [number of iterations]

   Author: Sylvain Pion, August 2002.

   Since Core Library 1.5
   $Id: bench.cpp,v 1.5 2010/08/05 02:48:12 exact Exp $
 ************************************************ */        

#include <iostream>
#include <cmath>

#ifdef CORE_LEVEL
#undef CORE_LEVEL
#define CORE_LEVEL 4
#endif
#include "CORE.h"

// Verifies that 0 == (sqrt(x)+sqrt(y))-sqrt(x+y-2*sqrt(x*y))
void bench_1(const Expr &x, const Expr &y)
{
  Expr e = (sqrt(x) + sqrt(y)) - sqrt(x + y + 2*sqrt(x*y));
  sign(e); // force to evaluate
  assert(e == 0); 
  // Expr e = (sqrt(x) + sqrt(y)) + sqrt(x + y + 2*sqrt(x*y));
  // assert(e != 0);

  // Print the root bound of e
  std::cerr << "root bound = " << e.rep()->get_rootBd().get_bound(e.rep()->get_Deg()) << std::endl;
}

void bench_2(const Expr &x, const Expr &y)
{
  Expr c = (sqrt(x) - sqrt(y)) / (x - y);
  Expr d = (sqrt(x) - sqrt(y)) / (x - y);
  Expr e = c-d;
  sign(e); // force to evaluate
  assert(e == 0); 

  // Print the root bound of e
  std::cerr << "root bound = "
      << e.rep()->get_rootBd().get_bound(e.rep()->get_Deg()) << std::endl;
}

void experiment_1(int loops) {
  for (int i=0; i<loops; i++) {
    bench_1(0.666666789, 0.987654321);
    bench_1(0.524556789, 0.687279321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
    bench_1(0.666666789, 0.987654321);
    bench_1(0.524556789, 0.687279321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
  }
}

void experiment_2(int loops) {
  for (int i=0; i<loops; i++) {
    bench_2(0.666666789, 0.987654321);
    bench_2(0.524556789, 0.687279321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
    bench_2(0.666666789, 0.987654321);
    bench_2(0.524556789, 0.687279321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
  }
}

int main(int argc, char **argv)
{
  int loops = (argc < 2) ? 10 : atoi(argv[1]);

  std::cout << "Doing " << loops << " iterations." << std::endl;

  // experiment_1(loops);

  experiment_2(loops);

  return 0;
}
