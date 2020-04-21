/*************************************************************
 * File: tRootOf.cpp
 * Purpose:
 * 	Illustrating the RootOf(P) construction
 * 		-- sqrt(6) = sqrt(2) * sqrt(3)
 * 		-- phi^2 = phi + 1   where phi is golden ratio
 *		-- phi = (sqrt(5) +/- 1)/2
 *
 * 	Usage:   tRootOf [n]
 *
 * 	     where n is optional number of bits of precision
 * 	     Default is n = 60.
 *
 * Author: Zilin Du
 * Date:  May 22, 2003
 * Since Core Library version 1.6
 *************************************************************/

#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  outputPrec =  40;	// default output precision
  if (argc >= 2) {
    outputPrec = atoi(argv[1]);
  }

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cout << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
 
  // compute sqrt(2)
  NT cs[] = {-2, 0, 1};		// coefficients of Polynomial
  Polynomial<NT> P(2, cs);

  Expr root1 = rootOf(P, 1);
  Expr root2 = rootOf(P, 2);
  Expr e = sqrt(Expr(3));
  Expr f1 = root1 + e;
  Expr f2 = root2 + e;

  f1.approx(CORE_INFTY, eps);
  f2.approx(CORE_INFTY, eps);
  
  setDefaultOutputDigits(outputPrec);
  
  cout << "-sqrt(2) = " << root1 << endl;  
  cout << " sqrt(2) = " << root2 << endl;  
  cout << " sqrt(3) = " << e << endl;  
  cout << "-sqrt(2) + sqrt(3) = " << f1 << endl;  
  cout << " sqrt(2) + sqrt(3) = " << f2 << endl;  

  //////////////////////////////////////////////////////////////
  // test sqrt(6)-sqrt(2)*sqrt(3)
  //////////////////////////////////////////////////////////////

  NT cs1[] = {-2, 0, 1};		// coefficients of Polynomial
  Polynomial<NT> P1(2, cs1);
  Expr e1 = rootOf(P1, 2); 
  
  NT cs2[] = {-3, 0, 1};		// coefficients of Polynomial
  Polynomial<NT> P2(2, cs2);
  Expr e2 = rootOf(P2, 2); 

  NT cs3[] = {-6, 0, 1};		// coefficients of Polynomial
  Polynomial<NT> P3(2, cs3);
  Expr e3 = rootOf(P3, 2); 
  
  cout << "sqrt(2) = " << e1 << endl;  
  cout << "sqrt(3) = " << e2 << endl;  
  cout << "sqrt(6) = " << e3 << endl;  

  Expr ee = e3 - e1 * e2;
  if (ee == 0) {
    cout << "CORRECT!   sqrt(2) * sqrt(3) = sqrt(6)" << endl;
  } else {
    cout << "ERROR!   sqrt(2) * sqrt(3) != sqrt(6)" << endl;
  }

  //////////////////////////////////////////////////////////////
  // More on sqrts of 2, 3, 6
  //////////////////////////////////////////////////////////////
  
  Expr e12 = e1 / e2;
  ee.approx(CORE_INFTY, eps);
  e12.approx(CORE_INFTY, eps);

  cout << "sqrt(6)-sqrt(2)*sqrt(3) = " << ee << endl;  
  cout << "sqrt(2)/sqrt(3) = " << e12 << endl;  
  
  //////////////////////////////////////////////////////////////
  // test almost sqrts
  //////////////////////////////////////////////////////////////
  
  BigFloat BFeps = BigFloat(1).exp2(-100); // very small value

  Expr ee1 = sqrt(Expr(2)-Expr(BFeps.exp2(-100)));
  Expr ee2 = sqrt(Expr(3)+Expr(BFeps.exp2(-100)));
  
  Expr eee = e3 - ee1 * ee2;
  if (eee == 0) {
    cout << "ERROR!   sqrt(2 - eps) * sqrt(3 + eps) = sqrt(6)" << endl;
  } else {
    cout << "CORRECT!   sqrt(2 - eps) * sqrt(3 + eps) != sqrt(6)" << endl;
  }

  //////////////////////////////////////////////////////////////
  // test golden ratio
  //////////////////////////////////////////////////////////////

  NT cs4[] = {-1, -1, 1};	// coefficients of Polynomial x^2 - x -1
  Polynomial<NT> P4(2, cs4);
  Expr goldenRatio1 = rootOf(P4, 1);
  Expr goldenRatio2 = rootOf(P4, 2);

  cout << "GoldenRatio1 = " << goldenRatio1 << endl;  
  cout << "GoldenRatio2 = " << goldenRatio2 << endl;  
  if (goldenRatio2 * goldenRatio2 == 1 + goldenRatio2) {
    cout << "CORRECT! GoldenRatio^2 = GoldenRatio + 1 (first root)" << endl;
  } else {
    cout << "ERROR! GoldenRatio^2 != GoldenRatio + 1 (first root)" << endl;
  }
  if (goldenRatio1 * goldenRatio1 == 1 + goldenRatio1) {
    cout << "CORRECT! GoldenRatio^2 = GoldenRatio + 1 (second root)" << endl;
  } else {
    cout << "ERROR! GoldenRatio^2 != GoldenRatio + 1 (second root)" << endl;
  }

  Expr g1 = (1 - sqrt(Expr(5)))/2;
  Expr g2 = (1 + sqrt(Expr(5)))/2;

  if ((goldenRatio1 == g1) && (goldenRatio2 == g2)) {
    cout << "CORRECT! GoldenRatio = (1 +/- sqrt(5))/2 " << endl;
  } else {
    cout << "ERROR! GoldenRatio != (1 +/- sqrt(5))/2 " << endl;
  }

}
