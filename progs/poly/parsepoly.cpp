/* **************************************
   File: ParsePoly.cpp
	Test the usage of constructor from strings for Polynomials.

   Author:  Vikram Sharma
   Date:    Oct. 1 2004

   Dec'2018:
   	getpoly() is found in inc/CORE/poly/Poly.h.
	We needed to make it public to compile this code.

   Since Core Library  Version 1.7
   $Id: parsepoly.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#define CORE_LEVEL 4
#include "CORE/CORE.h"

using namespace std;

typedef BigInt	NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;

// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[])
{

  //Fundamental test cases
  string s1 = "";//Empty string should be zero
  //basic terms
  string s2 = "x";
  string s3 = "x^23";
  string s4 = "23543544543543";
  string s5 = "- x^3";//Unary minus
  string s6 = "(x^2)^2";//Exponentiation
  //Terms
  string s7 = "234 x^2";
  string s8 = "-234 x^2";//Unary minus
  string s9 = "234 x^2 (x^3)";//Parentheses
  string s10 = "234 x^2 (x^3)^2";//Parentheses with exponential
  string s11 = "234 x^2 (x^3) * 2 x";
  //More complex ones
  string s12="x^2*( 3*x -  (x^2+x)  -1)^3+ 4*x^2*2*(x^2-1)*(x+2)";
  //
  string ss12="28x^5 - 15x^6 + x^4 + 6x^7 - 2x^3 - x^8 - 17x^2";
  string s13="(x+1)^2 - (x^2 + 1 + 2x) + (x+1)(x+1)";
  string s14="32 u^34  + 23 - 32 u^3 - 34*u ";//Any chars can be used as
                                              //placeholders
  string s15="32 X^34 + 23 - 32 X^3 - 34*X";//Capital case
  string s16="-32 X^34 + 23 - 32 X^3 - 34*X";//Capital case
  //Explicit construction of the BiPolys corresponding to the above two tests
  Polynomial<NT> p(34);
  p.setCoeff(34, 32);
  p.setCoeff(3, -32);
  p.setCoeff(1, -34);
  p.setCoeff(0, 23);


  Polynomial<NT> CC(s1);
  cout <<" String is " << s1 <<  endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s2);
  cout <<" String is " << s2 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s3);
  cout <<" String is " << s3 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s4);
  cout <<" String is " << s4 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s5);
  cout <<" String is " << s5 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s6);
  cout <<" String is " << s6 <<  endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s7);
  cout <<" String is " << s7 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s8);
  cout <<" String is  " << s8 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s9);
  cout <<" String is  " << s9 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;
  CC = CC.getpoly(s10);
  cout <<" String is  " << s10 << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;

  CC=CC.getpoly(s13);
  cout <<" Polynomial should be  x^2 + 2*x + 1" << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;



  CC = CC.getpoly(s12);
  Polynomial<NT> CC1(ss12);
  if(CC == CC1)
    cout <<"CORRECT! Polynomial created correctly from string " << endl;
  else
    cout <<"ERROR!!! Polynomial created incorrectly from string " << endl;


  CC = Polynomial<NT>(s14, 'u');
  cout <<" Using different place holders : string is  " << s14 << endl;
  if(CC == p)
    cout <<"CORRECT! Polynomial created correctly from string " << endl;
  else
    cout <<"ERROR!!! Polynomial created incorrectly from string " << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;


  CC = CC.getpoly(s15);
  cout <<" Using capital 'X'  : string is  " << s15 << endl;
  if(CC == p)
    cout <<"CORRECT! Polynomial created correctly from string " << endl;
  else
    cout <<"ERROR!!! Polynomial created incorrectly from string " << endl;
  CC.dump("Polynomial is");
  cout << endl;cout << endl;

} 
