/*************************************************************
 * File: tResultant.cpp
 * Purpose:
 * 	testing Resultant of Two Bivariate Polynomials
 * 	w.r.t. first variable and second variable
 *	Also discriminant wrt X and Y...
 *
 * 	Usage:   tResultant
 *
 * Author: Chee Yap, Narayan Kamath
 * Date:  Feb 11, 2010, Bugs fixed : July 2010
 * Since Core Library version 2.0
 *************************************************************/

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"

using namespace std;

typedef BigFloat		NT;
typedef Polynomial<NT>	PolyNT;

void TestConvertPoly();
void TestDiscriminantResultant();

int main(int argc, char* argv[]) {

  TestConvertPoly();
  TestDiscriminantResultant();

  return 0;
}//main


/* **************************************************
 * Subroutines:
 *************************************************** */

void TestConvertPoly() {
  BiPoly<NT> curve1("x^3 + 3x^2 y + 3xy^2 + y^3+x^2+2xy");
  curve1.dump("Original curve: ");

  BiPoly<NT> curve2(curve1);
  curve2.convertXpoly();

  /* **************************************************
   * THE FOLLOWING is a self-validating test
   * that the convertXpoly routine in Curves.tcc is CORRECT:
   * ************************************************** */

  if (curve1 == curve2) {
    cout << "\n ERROR!! Curve equal: X-Y exchange of curve has error!" << endl;
    curve2.dump();
  } else {
    cout << "\n CORRECT! Curve not equal: X-Y exchange of curve is OK!" << endl;
  }

  curve2.convertXpoly();
  if (curve1 == curve2) {
    cout << "\n CORRECT! Curve equal: X-Y exchange of curve is OK!" << endl;
  } else {
    cout << "\n ERROR!! Curve not equal: X-Y exchange of curve has error!" << endl;
    curve2.dump();
  }
}//TestConvertPoly

void TestDiscriminantResultant() {
  cout << endl
	<< "NOTE : Core1 computed resultant or discriminant only up to some non-zero\n"
        << "constant multiple -- Core2 does it correctly, but at a cost. \n"
	<< "We ought to keep the cheap option available for most applications."
	<< endl;

  Polynomial<NT> output;
  // Test case 1
  BiPoly<NT> poly1("2*x^2 - 2*x + 1");
  cout << "Discriminant of : ";
  poly1.dump();
  cout << endl;
  output = poly1.discriminantX(poly1);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("8")) {
    output.dump();
    cout << "\n CORRECT! Discriminant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in discriminant calculation !" << endl;
  }
  output = poly1.discriminantY(poly1);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("0")) {
    output.dump();
    cout << "\n CORRECT! Discriminant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in discriminant calculation !" << endl;
  }

  //---------------------
  BiPoly<NT> poly2("y^2 + y + x");
  cout << "Discriminant of : ";
  poly2.dump();
  cout << endl;
  output = poly1.discriminantX(poly2);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("1")) {
    output.dump();
    cout << "\n CORRECT! Discriminant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in discriminant calculation !" << endl;
  }
  output = poly1.discriminantY(poly2);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("4x-1")) {
    output.dump();
    cout << "\n CORRECT! Discriminant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in discriminant calculation !" << endl;
  }

  //----------------------
  BiPoly<NT> poly3("xy + 1"), poly4("y + x");
  cout << "Resultant of : ";
  poly3.dump();
  poly4.dump();
  cout << endl;
  output = resX(poly3, poly4);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("x^2 - 1")) {
    output.dump();
    cout << "\n CORRECT! Resultant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in Resultant calculation !" << endl;
  }
  output = resY(poly3, poly4);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("x^2 - 1")) {
    output.dump();
    cout << "\n CORRECT! Resultant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in Resultant calculation !" << endl;
  }

  BiPoly<NT> poly5("y^2 + y + x"), poly6("xy + 1");
  cout << "Resultant of : ";
  poly5.dump();
  poly6.dump();
  cout << endl;
  output = resX(poly5, poly6);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("1 - x^3 - x^2")) {
    output.dump();
    cout << "\n CORRECT! Resultant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in Resultant calculation !" << endl;
  }
  output = resY(poly5, poly6);
  // Note the " " around the 8 . Polynomial<NT>(8) := x^8 .
  if (output == Polynomial<NT>("x^3 - x + 1")) {
    output.dump();
    cout << "\n CORRECT! Resultant calculated correctly!" << endl;
  } else {
    output.dump();
    cout << "\n ERROR!! Error in Resultant calculation !" << endl;
  }
} //TestDiscriminantResultant

/* **************************************************
 * END
 *************************************************** */
