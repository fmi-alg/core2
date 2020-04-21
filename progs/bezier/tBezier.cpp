/* **************************************
   File: tBezier.cpp

   Description: 
	We test the basic functionalities of the class
	of Bezier Curves. 

   Usage:

        > ./tBezier

   Author:  Chee Yap (yap@cs.nyu.edu)

   Date:    Aug 2, 2004

   Since Core Library  Version 1.7
   $Id: tBezier.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 ************************************** */

#define CORE_LEVEL 4

#include <iostream>
#include <CORE/CORE.h>
#include "Bezier.h"

// #include "BernsteinPoly.h"

using namespace std;

typedef BigFloat NT;
typedef pair<NT, NT> NTInterval;
typedef vector<NTInterval> NTVecInterval;

int main(int argc, char** argv)
{
  // SET INTERNAL PRECISION
  setDefaultRelPrecision(CORE_posInfty); // This puts defAbsPrec in control
  int prec = 100;		// Do refinement till delta at most 2^{-prec}
  if (argc >= 2) prec = atoi(argv[1]);		// if user specifies
  setDefaultAbsPrecision(prec);
  // SET OUTPUT PRECISION
  cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
  cout.precision(prec/3); 		// convert prec bits into prec/3 digits
  
  cout << "------------------------------------------------------------\n";
  cout << "Relative Precision is " << defRelPrec << endl;
  cout << "Print Digits is " << cout.precision() << endl;
  cout << "Iteration Stops when delta < " << BigFloat::exp2(-prec) << endl;
  cout << "------------------------------------------------------------\n";

  cout <<"=============================================================\n"
  << "TEST 0 :  Control Polygon (0,0), (1,0), (0.5, -0.5), (0.5, 0.5) \n" <<
  "=============================================================" << endl;
/*
  VecPoint vp;
  vp.push_back(Point(0,0));
  vp.push_back(Point(0,1));
  vp.push_back(Point(1,2));
  vp.push_back(Point(3,-2));
  vp.push_back(Point(0,3));
  vp.push_back(Point(-2,1));
  vp.push_back(Point(2,2));
  vp.push_back(Point(-1,-1));
  vp.push_back(Point(3,1));
  vp.push_back(Point(1,-1));

  CHull ch(vp);
  ch.diameter();
  ch.plothull();
  ch.convexify();
  ch.plothull(2);
*/
  VecPoint vp2;
  // vp2.push_back(Point(0,0));
  // vp2.push_back(Point(1,0));
  // vp2.push_back(Point(0.5,-0.5));
  // vp2.push_back(Point(0.5,0.5));
  vp2.push_back(Point(0,0));
  vp2.push_back(Point(1,0));
  vp2.push_back(Point(0,1));

  Bezier bc(vp2);
  Point myp = bc.getPoint(0.5);
  bc.mydump("Bezier Curve");
  cout << "getPoint(0.5) = " << myp << endl;
//  Bezier L, R;
//  bc.subdivision(0.5, L, R);
//
  
//bc.plothull();

  // L.plot(5,1);
  // R.plot(5,2);

  // bc.plot(5);
  
  cout << "End of Bezier test" << endl;
  return 0;
}
