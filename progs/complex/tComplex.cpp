/* **************************************
   File: tComplex.cpp

   Description: 
	This file contains unit tests for
	a templated complex number classes.
	(inc/CORE/ComplexT.h).

	Using CORE_LEVEL 4, we test this for class with each 
	of four number types:

		Expr, BigFloat, machine_double, DoubleWrapper.

	Note that DoubleWrapper is just machine_double with
	a thin wrapper around it.

	The tests are self-validating.

	NOTE: It is important that the compiler flags do not
	contain -DNDEBUG, as this turns off assertion checks.
	In particular, in the standard Core environment, you should
	use VAR=Debug and not VAR="" or VAR=Opt.

   Usage:
	> make
	> ./tComplex

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

  Author/History:
	June 2010: Narayan Kamath implemented this for use for
	     implementing CEVAL in progs/mesh.
 ************************************** */

#include <iostream>
#include <vector>
#include <assert.h>

// Define CORE_LEVEL to 4 to allow for mixed mode operation.
#ifndef CORE_LEVEL
  #define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"
#include "CORE/ComplexT.h"

using namespace std;

// Testing the complex number constructors:
template <typename NT> void TestConstruction();
// Testing basic complex arithmetic:
template <typename NT> void TestArithmetic();
// Identity tests:
template <typename NT> void TestEquality();
// Tests for the two argument operators.
template <typename NT> void TestAdditional();
// Apply the above tests:
template <typename NT> void RunAllTests();

int main(int argc, char **argv) {
  cout << "Unit Test for Templated Complex number Class" << endl;
  cout << "LEVEL (3)" << endl;
  RunAllTests<Expr>();
  cout << "Passed." << endl;

  cout << "LEVEL (2) - BigFloat " << endl;
  RunAllTests<BigFloat>();
  cout << "Passed." << endl;

  // DID NOT WORK: 
  //
  //cout << "LEVEL (2) - BigRat " << endl;
  //RunAllTests<BigRat>();
  //cout << "Passed." << endl;

  // DID NOT WORK: this is important, because this
  // 	case gives us Gaussian Integers!!!
  //
  //cout << "LEVEL (2) - BigInt" << endl;
  //RunAllTests<BigInt>();
  //cout << "Passed." << endl;

  cout << "LEVEL (1) - No wrappers" << endl;
  RunAllTests<machine_double>();
  cout << "Passed." << endl;

  cout << "LEVEL (1) - Wrappers" << endl;
  RunAllTests<DoubleWrapper>();
  cout << "Passed." << endl;

  return 0;
}//main

// Tests constructors for Intervals:
template <typename NT> void TestConstruction() {
  typedef ComplexT<NT> Complex;
  typedef PolarComplexT<NT> PComplex;

  // Cartesian.
  // The default constructor should construct a 0.
  Complex t_1;
  assert(0 == t_1.re());
  assert(0 == t_1.im());
  // Constructing with a real number.
  Complex t_2(1);
  assert(1 == t_2.re());
  assert(0 == t_2.im());
  // Constructing 1 + 2i
  Complex t_3(1, 2);
  assert(1 == t_3.re());
  assert(2 == t_3.im());

  // Polar complex
  // The default constructor should construct a 0.
  PComplex p_1;
  assert(0 == p_1.re());
  assert(0 == p_1.im());
  // Note by def: The arg can be anything if the mod
  // is zero.
  assert(0 == p_1.mod());
  // Constructing with a real number.
  PComplex p_2(1);
  assert(1 == p_2.mod());
  assert(0 == p_2.arg());
  // Constructing e^2i
  PComplex p_3(1, 2);
  assert(1 == p_3.mod());
  assert(2 == p_3.arg());
  PComplex p_4(1, 10);
  assert(1 == p_4.mod());
  // Double check that the argument wraps
  // back after 2*PI
  assert(7 > p_4.arg());

}//TestConstruction

// Tests basic Interval Arithmetic:
template <typename NT> void TestArithmetic() {
  typedef ComplexT<NT> Complex;
  typedef PolarComplexT<NT> PComplex;

  const Complex a(NT(1) ,NT(2));
  const Complex b(3, 4);
  const Complex zero;
  const Complex one(1, 0);
  const NT one_re = 1;

  // Cartesian form
  // ------------------------
  // Addition.
  {
    const Complex a_b = a + b;
    const Complex a_zero = a + zero;
    assert(4 == a_b.re());
    assert(NT(6) == a_b.im());
    // Addition with zero should not change
    // anything.
    assert(a_zero.re() == a.re());
    assert(a_zero.im() == a.im());
  }
  // Subtraction.
  {
    const Complex a_b = b - a;
    const Complex a_zero = a - zero;
    assert(2 == a_b.re());
    assert(2 == a_b.im());
    // Addition with zero should not change
    // anything.
    assert(a_zero.re() == a.re());
    assert(a_zero.im() == a.im());
  }
  // Multiplication.
  {
    const Complex b_a = b * a;
    const Complex a_b = a * b;
    const Complex a_zero = a * zero;
    const Complex a_one = a * one;
    // NOTE(narayan) :
    // Argh, due to the ambiguity with the operators in
    // BigFloat2.
    const Complex a_one_re = operator*<NT>(a, one_re);
    // a*b = b*a
    assert(b_a == a_b);
    // The results of the multiplication.
    assert(-5 == a_b.re());
    assert(10 == a_b.im());
    // Multiplication by zero.
    assert(a_zero.im() == 0 && a_zero.re() == 0);
    // Multiplication by one.
    assert(a_one == a_one_re);
    assert(a_one == a);
  }
  // Division.
  {
    const Complex c(4, 8);
    const Complex d(1, 1);
    const Complex c_d = c / d;
    const Complex zero_c = zero / c;
    const Complex c_one = c / one;
    // NOTE(narayan) :
    // Argh, due to the ambiguity with the operators in
    // BigFloat2.
    const Complex c_one_re = operator/<NT>(c, one_re);
    // The results of the division.
    assert(6 == c_d.re());
    assert(2 == c_d.im());
    // Dividing something by zero..
    assert(zero_c.im() == 0 && zero_c.re() == 0);
    // Dividing by 1.
    assert(c_one == c_one_re);
    assert(c_one == c);
  }
  // Polar form
  // ---------------
  // We test only  multiplication and division, since
  // other ops are inexact.
  const PComplex p_zero;
  const PComplex p_one(1, 0);
  const PComplex p_a(4, 2);
  const PComplex p_b(1, 1);
  // Multiplication.
  {
    const PComplex a_b = p_a * p_b;
    const PComplex b_a = p_b * p_a;
    const PComplex a_zero = p_a * p_zero;
    const PComplex a_one = p_a * p_one;

    assert(4 == a_b.mod() && 3 == a_b.arg());
    assert(a_b == b_a);
    assert(a_zero.im() == 0 && a_zero.re() == 0);
    assert(a_one == p_a);
  }
  // Division.
  {
    const PComplex zero_a = p_zero / p_a;
    const PComplex a_one = p_a / p_one;
    const PComplex a_b = p_a / p_b;

    assert(a_one == p_a);
    assert(zero_a == p_zero);
    assert(a_b.mod() == 4 && a_b.arg() == 1);
  }
}

template <typename NT> void TestEquality() {
  typedef ComplexT<NT> Complex;
  typedef PolarComplexT<NT> PComplex;

  const Complex zero;
  const PComplex p_zero;
  const Complex one(1, 0);
  const PComplex p_one(1, 0);

  // Test between various representations.
  assert(zero == p_zero);
  assert(one == p_one);
  // Tests on the real line.
  assert(zero == NT(0));
  assert(one == NT(1));
}

template <typename NT> void TestAdditional() {
  typedef ComplexT<NT> Complex;
  //typedef PolarComplexT<NT> PComplex;

  const Complex zero;
  const Complex one(1, 0);
  const Complex i(0, 1);
  const Complex minusOne(-1, 0);
  const Complex a(123, -456);
  const Complex three(3, 0);

  // Some simple tests on |z| and arg(z) .
  assert(0 == zero.mod());
  assert(1 == one.mod() && 1 == i.mod());
  assert(0 == one.arg());

  // Chee, 12/2016: 
  assert(minusOne == i*i);
  assert(a * three == a+a+a);
  // assert(0==1);	// Deliberate error to check assertions 
}

// RUN ALL TESTS:
template <typename NT>
void RunAllTests() {
  TestConstruction<NT>();
  TestArithmetic<NT>();
  TestEquality<NT>();
  TestAdditional<NT>();
}

// End-of-File

