/* **************************************
   File: interval_t.cpp

   Description: 
	This file contains unit tests for a templated
	interval arithmetic class (inc/CORE/IntervalT.h).  
	You use the class IntervalT<> by
	by giving it a number type (NT): 

	  typedef IntervalT<NT> Interval;

	Using CORE_LEVEL 4, we test this for class with each 
	of the following choices for NT:

		Expr, BigFloat, BigRat, machine_double, DoubleWrapper.

	Note that DoubleWrapper is just machine_double with
	a thin wrapper around it.

	The tests are self-validating.

	NOTE: It is important that the compiler flags do not
	contain -DNDEBUG, as this turns off assertion checks.
	In particular, in the standard Core environment, you should
	use VAR=Debug and not VAR="" or VAR=Opt.

   Usage:
	> make
	> ./interval_t

   Notes:
	(1) Narayan (June 2010).
	There are very many advantages to using a standard
	C++ unit testing framework, such as
		http://code.google.com/p/googletest/
	or CppUnit. The overhead is very low, and it makes writing tests
	elegant , and encourages people to write more of them.
	This is a general TODO for CoreLib!

	(2) Narayan (June 2010)
	These tests are not intended as a test of precision.
	Rather, they test that we got our logic correct, so you will
	see generally small integers used in the test cases.

	(3) Chee (June 2010)
	More tests for:
	-- Extended Interval Arithmetic (which allows division by
	   zero, resulting in two intervals) 
	-- Intervals with left side -CORE_INFTY or right side +CORE_INFTY.

   Author/History:
	June 2010: Narayan Kamath implemented this for use for
	     Newton method codes in progs/mesh.

   Since Core Library  Version 2.0
   $Id: interval_t.cpp,v 1.9 2010/06/25 11:31:02 exact Exp $
 ************************************** */

#include <iostream>
#include <vector>

// Define CORE_LEVEL to 4 to allow for mixed mode operation.
#ifndef CORE_LEVEL
  #define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"
#include "CORE/IntervalT.h"

using namespace std;

// Testing the interval constructors:
template <typename NT> void TestConstruction();
// Testing basic interval arithmetic:
template <typename NT> void TestArithmetic();
// Identity tests:
template <typename NT> void TestEquality();
// Tests for intersection:
template <typename NT> void TestAdditional();
// I/O tests:
template <typename NT> void TestIO();
// TODO(narayan): Write more tests for this class.
template <typename NT> void TestExtension();
// Apply the above tests:
template <typename NT> void RunAllTests();

int main(int argc, char **argv) {
  cout << "Unit Test for Templated Interval Arithmetic Class\n";
  cout << "LEVEL (3)\n";
  RunAllTests<Expr>();
  cout << "Passed." << endl;

  cout << "LEVEL (2) - BigFloat\n";
  RunAllTests<BigFloat>();
  cout << "Passed." << endl;

  cout << "LEVEL (2) - BigRat\n";
  RunAllTests<BigRat>();
  cout << "Passed." << endl;

  cout << "LEVEL (1) - No wrappers\n";
  RunAllTests<machine_double>();
  cout << "Passed." << endl;

  cout << "LEVEL (1) - Wrappers\n";
  RunAllTests<DoubleWrapper>();
  cout << "Passed." << endl;

  return 0;
}//main


// Tests constructors for Intervals:
template <typename NT> void TestConstruction() {
  typedef IntervalT<NT> Interval;
  // Default constructor.
  Interval x;
  assert(x.getL() == 0);
  assert(x.getR() == 0);
  // Two argument constructor.
  Interval x_1(5, 6);
  assert(x_1.getL() == 5);
  assert(x_1.getR() == 6);
  // Copy constructor.
  Interval x_2(x_1);
  assert(x_2.getL() == NT(5));
  assert(x_2.getR() == 6);
  // Single argument int.
  Interval x_3(5);
  assert(x_3.getL() == 5);
  assert(x_3.getR() == 5);
  // Single argument NT.
  NT a = 5.5;
  Interval x_4(a);
  assert(x_4.getL() == a);
  assert(x_4.getR() == a);
}//TestConstruction

// Tests basic Interval Arithmetic:
template <typename NT> void TestArithmetic() {
  typedef IntervalT<NT> Interval;
  const Interval x_1(-1, 1);
  const Interval x_2(2, 2);
  const Interval x_3(5, 6);
  const Interval x_4(3, 7);
  const Interval one(1, 1);
  const Interval num(8, 16);
  const Interval den(2, 4);
  // The default zero.
  const Interval zero;

  // Addition
  Interval x_5 = x_1 + x_2;
  assert(x_5.getL() == 1);
  assert(x_5.getR() == 3);
  // Should remain unchanged.
  x_5 += 0;
  assert(x_5.getL() == 1);
  assert(x_5.getR() == 3);

  // Subtraction
  Interval x_6 = x_3 - x_4;
  assert(x_6.getL() == -2);
  assert(x_6.getR() == 3);
  x_6 -= 0;
  assert(x_6.getL() == -2);
  assert(x_6.getR() == 3);
  x_6 -= x_2;
  assert(x_6.getL() == -4);
  assert(x_6.getR() == 1);

  // Multiplication
  Interval x_7 = x_1 * x_2;
  assert(x_7.getL() == -2);
  assert(x_7.getR() == 2);

  Interval x_8 = x_2 * x_3;
  assert(x_8.getL() == 10);
  assert(x_8.getR() == 12);
  
  x_7 = x_8;	// hack: to avoid warning that x_8 is not used;

  // Multiplication by thin and thick zeroes.
  // NOTE(narayan) :  Multiplication by a thin zero tends to be a bit
  // of a problem because of an ambiguous overload with some operator.
  // hmmph.
  //
  // NT zero_thin = 0;
  // Interval x_9 = x_8 * zero_thin;
  // assert(x_9.getL() == 0);
  // assert(x_9.getR() == 0);
  //
  Interval x_10 = x_7 * zero;
  assert(x_10.getL() == 0);
  assert(x_10.getR() == 0);
  x_8 = x_10;	// hack: to avoid warning that x_10 is not used;

  // Division.
  Interval x_11 = num / one;
  assert(x_11.getL() == num.getL());
  assert(x_11.getR() == num.getR());

  x_11 /= one;
  assert(x_11.getL() == num.getL());
  assert(x_11.getR() == num.getR());

  Interval x_12 = zero / one;
  assert(x_12.getL() == zero.getL());
  assert(x_12.getR() == zero.getR());
  x_10 = x_12;	// hack: to avoid warning that x_12 is not used;

  Interval x_13 = num / den;
  assert(x_13.getL() == 2);
  assert(x_13.getR() == 8);
  x_12 = x_13;	// hack: to avoid warning that x_13 is not used;
}

// NOTE(narayan) :
// Note, these tests in this fn. are pretty pointless, should we
// get rid of them i wonder.
template <typename NT> void TestEquality() {
  typedef IntervalT<NT> Interval;
  const Interval zero, zero_1;
  const Interval one(1, 1);

  assert(zero == zero);
  assert(zero == zero_1);

  assert(zero != one);
  Interval zero_2 = zero + one;
  assert(zero != zero_2);
  zero_2 = zero_2+zero_2 ; // hack: to avoid warning that zero_2 is not used;
}
// Tests for intersect,
template <typename NT> void TestAdditional() {
  typedef IntervalT<NT> Interval;

  // The entire Real line.
  const Interval infinity(Interval::I_NEG_INFTY, Interval::I_POS_INFTY);
  const Interval neg(Interval::I_NEG_INFTY, 0);
  const Interval pos(0, Interval::I_POS_INFTY);
  const Interval i_2(-20, 30);
  const Interval i_1(-10, 10);
  const Interval i_3(10, 15);
  const Interval i_4(8, 15);
  const Interval i_5(-12, -8);

  // Contains
  assert(Contains(infinity, neg));
  assert(Contains(infinity, pos));
  assert(Contains(infinity, infinity));
  assert(!Contains(neg, infinity));
  assert(!Contains(pos, infinity));
  assert(Contains(infinity, i_1));
  assert(!Contains(neg, i_1));
  assert(!Contains(pos, i_1));
  assert(Contains(i_2, i_1));

  // Overlap
  assert(Overlap(i_2, i_1));
  assert(Overlap(i_1, i_2));
  // They actually do overlap at [0, 0]
  assert(Overlap(neg, pos));
  assert(Overlap(i_3, i_4));
  assert(Overlap(i_3, i_1));
  assert(Overlap(i_5, i_1));
  assert(Overlap(i_1, i_5));

  // Intersect.
  Interval x = Intersect(infinity, i_2);
  Interval x_r = Intersect(i_2, infinity);
  assert(x == x_r);
  assert(x_r == i_2);
  x_r = x;	// hack: to avoid warning that RHS is not used;

  Interval s_1 = Intersect(i_5, i_1);
  Interval s_2 = Intersect(i_1, i_5);
  assert(s_1 == s_2);
  assert(s_1.getL() == -10);
  assert(s_1.getR() == -8);
  s_1 = s_2+s_1 ; // hack: to avoid warning that RHS is not used;
}

// I/O tests:
template <typename NT> void TestIO() {
  typedef IntervalT<NT> Interval;
  Interval x = Interval(NT(0), NT(2));
  cout << "Interval(0,1) = " << x << endl;
  Interval y = Interval(NT(123));
  cout << "Interval(123) = " << y << endl;
  Interval z = Interval(NT(0.034), NT(1.2));
  cout << "Interval(0.034, 1.2) = " << z << endl;
}

// Extension tests:
template <typename NT> void TestExtension() {
  // TODO(narayan): Write tests for this class.
}

// RUN ALL TESTS:
template <typename NT>
void RunAllTests() {
  TestConstruction<NT>();
  TestArithmetic<NT>();
  TestEquality<NT>();
  TestIO<NT>();
  TestAdditional<NT>();
  TestExtension<NT>();
}

// End-of-File

