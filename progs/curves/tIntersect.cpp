/* **************************************
   File: tIntersect.cpp

   Description: 
   	basic intersection of two curves

   Usage:
        > ./tIntersect [n=1] [m=2] [eps=0.5]

	will plot the union of the n-th curve with
	the m-th curve in this program
	(currently, n and m range in 0,1,...,4).

	The output is in the file data/plot2.
	You can display this file by calling drawcurve
	    program, found in CORE_PATH/ext/graphics.

   BUGS and TODO LIST:
   	Should be able to do automatic analysis to determine
	interesting area for intersection

   Author:  Chee Yap and Vikram Sharma
   Date:    Apr 20, 2004

   Since Core Library  Version 1.7
   $Id: tIntersect.cpp,v 1.3 2010/11/08 13:52:21 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
// #include "localCurves.h"	// this is the local version of Curves.h
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h

using namespace std;

typedef BigInt				NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;


int main(int argc, char* argv[])
{
	// GET ARGUMENTS
	int curveNo1 = 2;	// First of 2 curves to intersect
	if (argc>1) curveNo1 = atoi(argv[1]);
	int curveNo2 = 3;	// Second of 2 curves to intersect
	if (argc>2) curveNo2 = atoi(argv[2]);
	BigFloat eps =  1000000;	// x-precision of plot (dummy value)
	if (argc>3) eps = BigFloat(argv[3]);
	BigFloat xmin = 1000000;	// min x (dummy value)
	if (argc>4) xmin = BigFloat(argv[4]);
	BigFloat ymin = 1000000;	// min y (dummy value)
	if (argc>5) ymin = BigFloat(argv[5]);
	BigFloat xmax = 1000000;	// max x (dummy value)
	if (argc>6) xmax = BigFloat(argv[6]);
	BigFloat ymax = 1000000;	// max y (dummy value)
	if (argc>7) ymax = BigFloat(argv[7]);
/*
    void setParam(BigFloat eps1,
		BigFloat xmin1, BigFloat ymin1,
		BigFloat xmax1, BigFloat ymax1) {
	if (eps  >= 1000000) eps  = eps1;
	if (xmin >= 1000000) xmin = xmin1;
	if (ymin >= 1000000) ymin = ymin1;
	if (xmax >= 1000000) xmax = xmax1;
	if (ymax >= 1000000) ymax = ymax1;
    }
*/
    // SET INTERNAL PRECISION
    setDefaultRelPrecision(CORE_posInfty); // This puts defAbsPrec in control
    int prec = 10;			// Do Sturm refinement till delta
					// is at most 2^{-prec}
    setDefaultAbsPrecision(prec);

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits

	// CONSTRUCT EQUATION OF CURVE
	// cissoid  2Y^2 - X^3
	  NT cissoid_coeff0[] = {0, 0, 0, -1};
	  NT cissoid_coeff2[] = {2};
	// ellipse: X^2 + 2Y^2 - 1
	  NT ellipse_coeff0[] = {-1, 0, 1};	// x^2 - 1
	  NT ellipse_coeff2[] = {2};
	// cubic1: Y^3 - X^3 + 2XY
	//       = Y^3 +  (2X)Y + (-X^3)
	  NT cubic1_coeff0[]  = {0, 0, 0, -1};
	  NT cubic1_coeff1[]  = {0, 2};
	  NT cubic1_coeff3[]  = {1};
	// cubic2: Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2
	// 	 = Y^3 + (3X)Y^2 + (4X^2)Y + (-2X^3+2X^2).
	  NT cubic2_coeff0[]  = {0, 0, 2, -2};
	  NT cubic2_coeff1[]  = {0, 0, 4};
	  NT cubic2_coeff2[]  = {0, 3};
	  NT cubic2_coeff3[]  = {1};
	// cubic3: Y^2 - X(X^2-1) 
	// 	component C0 is an oval through (0,0), left of the Y-axis,
	// 	component C1 is a simple curve through (1,0) symmetric about
	// 	the X-axis
	  NT cubic3_coeff0[]  = {0, 1, 0, -1};
	  NT cubic3_coeff3[]  = {1};

	VecPoly vP1;

	switch (curveNo1){
	  case 0:
	    cout << " The Cissoid of Diocles [Brieskorn-Knorrer, p.9]\n"
	       << "    Equation is  - X^3 + 2Y^2 = 0." << endl;
	    vP1.push_back( Polynomial<NT>(3, cissoid_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP1.push_back( Polynomial<NT>(0, cissoid_coeff2) );	// coeff of y^2
	    eps = 0.5; xmin = -2.0; ymin = -10.0; xmax = 10.0; ymax = 10.0;
	    break;
	  case 1:
	    cout << " Ellipse \n" 
	         << "    Equation is  X^2 + 2Y^2 = 1." << endl;
	    vP1.push_back( Polynomial<NT>(2, ellipse_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP1.push_back( Polynomial<NT>(0, ellipse_coeff2) );	// coeff of y^2
	    eps = 0.05; xmin = -2.0; ymin = -2.0; xmax = 2.0; ymax = 2.0;
	    break;
	  case 2:
	    cout << " Cubic1: Y^3 - X^3 + 2XY = Y^3 +  (2X)Y + (-X^3)" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic1_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>(1, cubic1_coeff1) );	// coeff of y
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic1_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -5.0; ymin = -5.0; xmax = 5.0; ymax = 5.0;
	    break;
	  case 3:
	    cout << " Cubic2:  Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic2_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>(2, cubic2_coeff1) );	// coeff of y
	    vP1.push_back( Polynomial<NT>(1, cubic2_coeff2) );	// coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic2_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -2.0; ymin = -2.0; xmax = 2.0; ymax = 2.0;
	    break;
	  case 4:
	    cout << " Cubic3: Y^2 - X(X^2-1)" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic3_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );         // coeff of y
	    vP1.push_back( Polynomial<NT>::Zero() );         // coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic3_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -3.0; ymin = -3.0; xmax = 5.0; ymax = 5.0;
	    break;
	  default:
	    cout << "Choose a smaller number for the curve to display!" << endl;
	    exit(0);
	}

	VecPoly vP2;

	switch (curveNo2){
	  case 0:
	    cout << " The Cissoid of Diocles [Brieskorn-Knorrer, p.9]\n"
	       << "    Equation is  - X^3 + 2Y^2 = 0." << endl;
	    vP2.push_back( Polynomial<NT>(3, cissoid_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP2.push_back( Polynomial<NT>(0, cissoid_coeff2) );	// coeff of y^2
	    eps = 0.5; xmin = -2.0; ymin = -10.0; xmax = 10.0; ymax = 10.0;
	    break;
	  case 1:
	    cout << " Ellipse \n" 
	         << "    Equation is  X^2 + 2Y^2 = 1." << endl;
	    vP2.push_back( Polynomial<NT>(2, ellipse_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP2.push_back( Polynomial<NT>(0, ellipse_coeff2) );	// coeff of y^2
	    eps = 0.05; xmin = -2.0; ymin = -2.0; xmax = 2.0; ymax = 2.0;
	    break;
	  case 2:
	    cout << " Cubic1: Y^3 - X^3 + 2XY = Y^3 +  (2X)Y + (-X^3)" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic1_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>(1, cubic1_coeff1) );	// coeff of y
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic1_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -5.0; ymin = -5.0; xmax = 5.0; ymax = 5.0;
	    break;
	  case 3:
	    cout << " Cubic2:  Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic2_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>(2, cubic2_coeff1) );	// coeff of y
	    vP2.push_back( Polynomial<NT>(1, cubic2_coeff2) );	// coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic2_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -2.0; ymin = -2.0; xmax = 2.0; ymax = 2.0;
	    break;
	  case 4:
	    cout << " Cubic3: Y^2 - X(X^2-1)" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic3_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );         // coeff of y
	    vP2.push_back( Polynomial<NT>::Zero() );         // coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic3_coeff3) );	// coeff of y^3
	    eps = 0.05; xmin = -3.0; ymin = -3.0; xmax = 5.0; ymax = 5.0;
	    break;
	  default:
	    cout << "Choose a smaller number for the curve to display!" << endl;
	    exit(0);
	}
	// TESTING CURVES
	BiPoly<NT> C1(vP1);
	BiPoly<NT> C2(vP2);
	C1 *= C2; //product of the two curves

//Kludge
eps = 0.05; xmin = -8.0; ymin = -8.0; xmax = 8.0; ymax = 8.0;

	Curve<NT> CC(C1);
	CC.dump("test curve");
	CC.plot(eps, xmin, ymin, xmax, ymax, 2); // output in data/plot2 file
	
	BiPoly<NT> CC1("3 x^2y^31 + 7 xy^2 - 4 x + 13xy+25");
	cout << "CC1 degree = "<< CC1.getYdegree() << endl;
	CC1.dump();

	
	return 0;
}

