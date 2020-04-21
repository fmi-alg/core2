/************************************************************
  file: pred2.cc
  purpose:
      comparison involving a generalization of
      basic predicate in Fortune's algorithm:

            (a + sqrt[2^k](b)) / d - (a' + sqrt[2^k](b')) / d'.

  author: Chen Li and Chee Yap
  Core Library 
  $Id: pred2.cpp,v 1.1 2006/03/07 04:54:37 exact Exp $
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
   std::cout << "r = " << r << ", lg(r) = " << bitLength(r) << std::endl;
   if ((rand() % 2) == 0) r = - r;
   return r;
}


int main(int argc, char* argv[]) {
  double a1, b1, d1;
  double a2, b2, d2;

  int k = 1;
  int r = 1;
  if (argc == 2) {
    L = atoi(argv[1]);
  } else if (argc == 3) {
    L = atoi(argv[1]);
    r = atoi(argv[2]);
  } else if (argc == 4) {
    L = atoi(argv[1]);
    r = atoi(argv[2]);
    k = atoi(argv[3]);
  }

  unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
  srand(seed1);

  a1 = bigRand(3 * L);
  b1 = abs(bigRand(6 * L));
  d1 = bigRand(2 * L);
  
  /*
  a2 = bigRand(3 * L);
  b2 = abs(bigRand(6 * L));
  d2 = bigRand(2 * L);
  */

  a2 = a1;
  b2 = b1;
  d2 = d1;

  machine_long t1 = clock();
  for (int i = 0; i < k; i++) {
    double tmp1 = b1;
    double tmp2 = b2;
    for (int j=0; j<r; j++) {
      tmp1 = sqrt(tmp1);
      tmp2 = sqrt(tmp2);
    }
    double e1 = (a1 + tmp1) / d1; 
    double e2 = (a2 + tmp2) / d2;
    std::cout << "e1 == e2 ? " << ((e1 == e2) ?  "yes" : "no") << std::endl;
  }

  machine_long t2 = clock();
  std::cout << "TOTAL time spent: " << double(t2 - t1)/(CLOCKS_PER_SEC) << " seconds" << std::endl;
  return 0;
}

