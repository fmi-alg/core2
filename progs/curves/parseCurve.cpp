/* **************************************
   File: parseCurve.cpp

   Description: 
   	testing the parsing of bivariate polynomials

   Usage:

        > ./parseCurve

   Author:  Vikram Sharma
   Date:    Aug 20, 2004

   Since Core Library  Version 1.7
   $Id: parseCurve.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif


#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h

using namespace std;

typedef BigInt	NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;


// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[])
{

  //Fundamental test cases
  string s1 = "";//Emtpy string should be zero
  //basic terms
  string s2 = "x";
  string s3 = "y";
  string s4 = "x^23";
  string s5 = "y^21";
  string s6 = "2354354";
  string s7 = "- x^3";//Unary minus
  string s8 = "(y^7)";//Parentheses with a basic term
  string s9 = "(x^2)^2";//Exponentiation
  //Terms
  string s10 = "234 x^2 y^3";
  string s11 = "-234 x^2 y^3";//Unary minus
  string s12 = "234 x^2 (y^3)";//Parentheses
  string s13 = "234 x^2 (y^3)^2";//Parentheses with exponential
  string s14 = "234 x^2 (y^3) * 2 x";

  //More complex cases:
  string s15="x^2 y ( 3x - y^2 + (x^2+xy)  -1)^3 + 4x(x+y)*2(x^2-y^2)";
  //Constructing the bipoly explicitly for self-testing.
  string ss15="x^8*y+3*x^7*y^2+9*x^7*y+18*x^6*y^2+24*x^6*y-5*x^5*y^4-9*x^5*y^3+21*x^5*y^2+9*x^5*y-18*x^4*y^4-24*x^4*y^3-18*x^4*y^2-24*x^4*y+8*x^4+3*x^3*y^6+9*x^3*y^5+6*x^3*y^4+18*x^3*y^3+3*x^3*y^2+17*x^3*y-x^2*y^7-3*x^2*y^5-3*x^2*y^3-8*x^2*y^2-x^2*y-8*x*y^3";

  //Test the behaviour with parentheses
  string s16="(x+y)^2 - (x^2 + y^2 + 2xy) + (x+y)(x+y)";
  string s17="32 u^34 v + 23 - 32 u^3 - 34 v^72";//Any chars cane be used as
                                              //placeholders
  string s18="32 X^34 Y + 23 - 32 X^3 - 34 Y^72";//Capital case

  string s19="-32 X^34 Y + 23 - 32 X^3 - 34 Y^72";//Capital case
  //Explicit construction of the BiPolys corresponding to the above two tests
  NT c1[] = {23, 0, 0, -32};
  NT c2[] = {-34};
  Polynomial<NT> p(34);
  p.setCoeff(34, 32);
  p.setCoeff(0,0);
  Curve<NT> CC2(72);
  CC2.setCoeff(0, Polynomial<NT>(3, c1));
  CC2.setCoeff(1,p);
  CC2.setCoeff(72, Polynomial<NT>(0, c2));


  Curve<NT> CC(s1);
  cout <<" String is " << s1 <<  endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s2);
  cout <<" String is " << s2 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s3);
  cout <<" String is" << s3 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s4);
  cout <<" String is " << s4 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s5);
  cout <<" String is " << s5 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s6);
  cout <<" String is " << s6 <<  endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s7);
  cout <<" String is " << s7 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s8);
  cout <<" String is  " << s8 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s9);
  cout <<" String is  " << s9 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s10);
  cout <<" String is  " << s10 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s11);
  cout <<" String is  " << s11 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s12);
  cout <<" String is  " << s12 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s13);
  cout <<" String is  " << s13 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  CC = CC.getbipoly(s14);
  cout <<" String is  " << s14 << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;

  CC=CC.getbipoly(s16);
  cout <<" Curve should be  x^2 + 2*x y + y^2" << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;


CC=CC.getbipoly("y^2 - x^3");
cout <<"Curve should be y^2 - x^3 "<< endl;
CC.dump("Curve is");
cout << endl; cout << endl;
  CC = CC.getbipoly(s15);
  Curve<NT> CC1(ss15);
  if(CC == CC1)
    cout <<"CORRECT! Bipoly created correctly from string " << endl;
  else
    cout <<"ERROR!!! Bipoly has error " << endl;


  CC = Curve<NT>(s17, 'u', 'v');
  cout <<" Using different place holders : string is  " << s17 << endl;
  if(CC == CC2)
    cout <<"CORRECT! BiPoly created correctly from string " << endl;
  else
    cout <<"ERROR!!! BiPoly created incorrectly from string " << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;


  CC = CC.getbipoly(s18);
  cout <<" Using capital 'X' and 'Y' : string is  " << s18 << endl;
  if(CC == CC2)
    cout <<"CORRECT! BiPoly created correctly from string " << endl;
  else
    cout <<"ERROR!!! BiPoly created incorrectly from string " << endl;
  CC.dump("Curve is");
  cout << endl;cout << endl;
  
   return 0;
} 
