/* ************************************************
   File: floor_ceil.cpp

   Purpose:  To test the floor(Expr) and ceil(Expr) functions.

   Usage:
        % floor_ceil

   Author: Sylvain Pion, June 2002.

   Since Core Library 1.5
   $Id: floor_ceil.cpp,v 1.2 2010/08/05 02:48:13 exact Exp $
 ************************************************ */        

#include <iostream>
#include <cmath>

#ifdef CORE_LEVEL
#undef CORE_LEVEL
#define CORE_LEVEL 4
#endif
#include "CORE.h"
using namespace std;


// Verifies if floor(e) and ceil(e) are equal to the expected values f and c.
bool floor_ceil_test(const CORE::Expr   & e,
                     const CORE::BigInt & f,
                     const CORE::BigInt & c)
{
  bool result = (CORE::floor(e) == f) && (CORE::ceil(e) == c);
  if (!result)
  {
    cerr << "ERROR :" << endl;
    cerr << "  Expression " << e << " was expecting :" << endl;
    cerr << "  floor " << f << " but got instead : " << CORE::floor(e) << endl;
    cerr << "  ceil  " << c << " but got instead : " << CORE::ceil(e) << endl;
  }
  return result;
}


// Same as above, but also tests that :
// floor(-e) == -ceil(e) and ceil(-e) == -floor(e).
bool floor_ceil_test_2(const CORE::Expr   & e,
                       const CORE::BigInt & f,
                       const CORE::BigInt & c)
{
  return floor_ceil_test(e, f, c) && floor_ceil_test(-e, -c, -f);
}


int main()
{
  bool OK = true;

  for (int i=0; i<10; i++) {

    // Integers
    OK = OK && floor_ceil_test_2(i, i, i);

    // Integers but constructed from doubles.
    OK = OK && floor_ceil_test_2(1.0*i, i, i);

    // Large integers to check for possible overflow bugs.
    CORE::BigInt bi = 1000000*CORE::BigInt(1000000)*CORE::BigInt(1000000);
    OK = OK && floor_ceil_test_2(CORE::Expr(bi), bi, bi);

    // Square roots of integers (some of which are perfect squares...).
    double d = sqrt(1.0*i);
    CORE::Expr e = sqrt(CORE::Expr(i));
    OK = OK && floor_ceil_test_2(e, (long) floor(d), (long) ceil(d));

    // More could be added...
  }

  if (!OK) {
    cerr << "There was at least one ERROR!!" << endl;
    return -1;
  }

  cout << "CORRECT!! floor() and ceil() tested successfully" << endl;

  return 0;
}
