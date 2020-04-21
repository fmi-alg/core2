// file:tZimmermann.cpp
//
// This file tests the numerical data in the paper of
// 	Vincent Lefevre, Dmien Stehle and Paul Zimmermann
// 	"Worst Cases for the Exponential Functions in the IEEE 754r Decimal64 Format"
// 	(LNCS 5045, 2008)
//
// 
// exp(.09407822313572878) =
// 	1.09864568206633850000000000000000278053679990505474865434063
// exp(.0000000000000009999999999999995) =
// 	1.00000000000000099999999999999999999999999999966666666666667
// exp(0.5091077534282133) =
// 	1.663806007261509500000000000000049244321749028987413508825415
// exp(0.7906867968553504) =
// 	2.204910231771509499999999999999916363030032777977030380162343
// exp(.001548443067391468) =
// 	1.00154964252437499999999999999992657648357147681107955413497
// exp(0.295337950477727) =
// 	1.34358034558906700000000000000008648800853072576413415564777
// exp(.000000006581539478341669) =
// 	1.0000000065815395000000000000000177317374058026209540111726
// exp(.00000002662858264545929) =
// 	1.00000002662858300000000000000003189061533791129447188266672
// exp(.00000003639588333766983) =
// 	1.00000003639588400000000000000002406217542759409326494118845
//
//
// Author: Jihun Yu (Oct 2008)
//
// This program MUST be run at CORE_LEVEL 3:

#define CORE_LEVEL 3
using namespace std;

#include <CORE.h>

void computeexp(Expr e, prec_t prec) {
  cout << "exp(" << e << ") = " << exp(e).approx(prec, CORE_INFTY) << endl;
}

int main( int argc, char *argv[] ) {

  prec_t prec = 200;	// relative digits
  int digits;
  int print = 0;

  if (argc > 1) {
    digits = atoi(argv[1]);
    prec = digits2bits(digits);
  }
  if (argc > 2)
    print = atoi(argv[2]);
 
  setDefaultRelPrecision(prec); 
  setDefaultOutputDigits(70);

  computeexp(Expr("9.407822313572878" / Expr("100")), prec);
  computeexp(Expr("9.999999999999995" / pow(Expr("10"), 16)), prec);
  computeexp(Expr("0.5091077534282133") , prec);
  computeexp(Expr("0.7906867968553504") , prec);
  computeexp(Expr("0.1548443067391468" / Expr("100")) , prec);
  computeexp(Expr("0.2953379504777270") , prec);
  computeexp(Expr("6.581539478341669" / Expr("1000000000")) , prec);
  computeexp(Expr("2.662858264545929" / Expr("100000000")) , prec);
  computeexp(Expr("3.639588333766983" / Expr("100000000")) , prec);

  return 0;
}
