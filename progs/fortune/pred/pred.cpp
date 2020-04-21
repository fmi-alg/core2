/************************************************************
  file: pred.cc
  purpose:
      test the expression 
                (a + sqrt(b)) / d - (a' + sqrt(b')) / d'.
      where a and a' are 3L-bits, b and b' are 6L-bits,
	d and d' are 2L-bits, resp.

      The basic predicate in Fortune's algorithm reduces
	to such an expression.
      The program will generate random values for these
	parameters with *exactly* XL-bits (X=3,6,2).

  author: Chen Li and Chee Yap
  CORE Library
  $Id: pred.cpp,v 1.1 2006/03/07 04:54:37 exact Exp $
************************************************************/
 
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif
#include <ctime>
#include "CORE.h"

static int L = 20;
/*
BigInt bigRand(int k) { // generate a k-bit random big integer
  BigInt r = 0;
  BigInt q = 1;
  q <<= (k-1); // the leading bit is one.
  int bits = 0;
  unsigned int m;
  while (bits < k) {
    m = rand() % (1 << 15); // [0, 2^15 - 1]
    //  std::cout << "m = " << m  << std::endl;
    int rest = k - bits; 
    if (rest > 15) {
      r <<= 15;
      r += m;
      bits += 15;
    } else {
      r <<= rest;
      r += (m % (1 << rest));
      bits += rest;
    }
  }
  if (r < q) r+= q;
  std::cout << "r = " << r << ", lg(r) = " << lg(r) << std::endl;
  assert(r > 0);
  if ((rand() % 2) == 0) r = - r;
  return r;
}
*/

BigInt bigRand(int k) { // generate a k-bit random big integer
   BigInt range = 1;
   range <<= k;
   BigInt r = randomize(range);
   // std::cout << "r = " << r << ", lg(r) = " << lg(r) << std::endl;
   if ((rand() % 2) == 0) r = - r;
   return r;
}
   

int main(int argc, char* argv[]) {
  double a1, b1, d1;
  double a2, b2, d2;

  int k = 1;
  if (argc == 2) {
    L = atoi(argv[1]);
  } else if (argc == 3) {
    L = atoi(argv[1]);
    k = atoi(argv[2]);
  }

  unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
  srand(seed1);


  machine_long t1, t2; 
  machine_long total = 0;
  for (int i = 0; i < k; i++) {
    a1 = bigRand(3 * L);
    b1 = abs(bigRand(6 * L));
    d1 = bigRand(2 * L);
    a2 = a1;
    b2 = b1;
    d2 = d1;

    t1 = clock();

    double e1 = (a1 + sqrt(b1)) / d1; 
    double e2 = (a2 + sqrt(b2)) / d2;
    std::cout << "e1 == e2 ? " << ((e1 == e2) ?  "yes" : "no") << std::endl;

    t2 = clock();

    total += (t2 - t1);
  }

  std::cout << "Total USER time spent: " << (double)total / CLOCKS_PER_SEC << " seconds" << std::endl;
  std::cout << "Average user time:     " << (double)total / (CLOCKS_PER_SEC * k) << "seconds" << std::endl;
  return 0;
}

