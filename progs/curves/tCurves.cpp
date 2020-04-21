/* **************************************
   File: tCurves.cpp

   Description: 
	We plot and display a variety of curves.
	This includes the special case of plotting
		a univariate polynomial.

   Usage:

        > ./tCurves [n] [eps] [Xmin] [Ymin] [Xmax] [Ymax] [extraParam]

       This will output a plot of the n-th curve in this program
	      (currently, n=0,1,...,4).
       If eps, Xmin, etc, are not specified, they will default to some
              values that we have choosen for that particular curve.
       The output is in the file data/plot.
       You can display this curve on your screen using our simple
              drawcurve program found in CORE_PATH/ext/graphics.
	      (Using our Makefile, just type "make show" or "make s")

   Author:  Chee Yap and Vikram Sharma
   Date:    Apr 20, 2004

   Since Core Library  Version 1.7
   $Id: tCurves.cpp,v 1.4 2010/11/08 16:51:14 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
// #include "localCurves.h"	// this is the local version of Curves.h
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h

typedef BigInt	NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;

// =========================================================
// GLOBAL PLOT PARAMETERS
// =========================================================
#define NaN 100000
BigFloat eps = 0;	// precision of the plot (i.e., delta-x)
BigFloat xmin = NaN;	// min x 
BigFloat ymin = NaN;	// min y
BigFloat xmax = NaN;	// max x
BigFloat ymax = NaN;	// max y
int      a = NaN;	// extra curve parameter (depending on curve)

// Set default values if these are not specified by user:
inline void setParams(BigFloat e, BigFloat xxmin, BigFloat yymin,
		BigFloat xxmax, BigFloat yymax){
    if (eps == 0) eps = e;
    if (xmin == NaN) xmin = xxmin;
    if (ymin == NaN) ymin = yymin;
    if (xmax == NaN) xmax = xxmax;
    if (ymax == NaN) ymax = yymax;
}
inline void showParams(){
    cout << "eps = " << eps << endl;
    cout << "xmin = " << xmin << ", xmax = " << xmax << endl;
    cout << "ymin = " << ymin << ", ymax = " << ymax << endl;
}

// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[])
{
    // GET INPUT ARGUMENTS
	int curveNo = 0;	// default curve to display
	if (argc>1) curveNo = atoi(argv[1]);
	if (argc>2) eps = BigFloat(argv[2]);
	if (argc>3) xmin = BigFloat(argv[3]);
	if (argc>4) ymin = BigFloat(argv[4]);
	if (argc>5) xmax = BigFloat(argv[5]);
	if (argc>6) ymax = BigFloat(argv[6]);
	if (argc>7) a = atoi(argv[7]);	// extra parameter

    // SET INTERNAL PRECISION
    int prec = 15;	
    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits

   // CONSTRUCT EQUATION OF CURVE
   // 	We illustrate two methods of doing this:
   //
   //	(1) Slow way: build up the vector of coefficients of the Y-powers
   //		The first 3 curves are constructed this way.
   //
   //	(2) String way: specify the polynomial as a string in x and y
   //
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

   Curve<NT> CC;
   VecPoly vP;
   switch (curveNo){
     case 0:
       cout << " The Cissoid of Diocles [Brieskorn-Knorrer, p.9]\n"
          << "    Equation is  - X^3 + 2Y^2 = 0." << endl;
       vP.push_back( Polynomial<NT>(3, cissoid_coeff0) );	// coeff of 1
       vP.push_back( Polynomial<NT>::Zero() );		// coeff of y
       vP.push_back( Polynomial<NT>(0, cissoid_coeff2) );	// coeff of y^2
       CC= Curve<NT>(vP);
       setParams(0.03, -2.0, -5.0, 5.0, 5.0);
       break;
     case 1:
       cout << " Ellipse \n" 
            << "    Equation is  X^2 + 2Y^2 = 1." << endl;
       vP.push_back( Polynomial<NT>(2, ellipse_coeff0) );	// coeff of 1
       vP.push_back( Polynomial<NT>::Zero() );		// coeff of y
       vP.push_back( Polynomial<NT>(0, ellipse_coeff2) );	// coeff of y^2
       setParams(0.03, -2.0, -2.0, 2.0, 2.0);
       CC= Curve<NT>(vP);
       break;
     case 2:
       cout << " Cubic1: Y^3 - X^3 + 2XY = Y^3 +  (2X)Y + (-X^3)" << endl;
       vP.push_back( Polynomial<NT>(3, cubic1_coeff0) );	// coeff of 1
       vP.push_back( Polynomial<NT>(1, cubic1_coeff1) );	// coeff of y
       vP.push_back( Polynomial<NT>::Zero() );		// coeff of y^2
       vP.push_back( Polynomial<NT>(0, cubic1_coeff3) );	// coeff of y^3
       setParams(0.03, -5.0, -5.0, 5.0, 5.0);
       CC= Curve<NT>(vP);
       break;
     case 3:
       cout << " Cubic2:  Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2" << endl;
       vP.push_back( Polynomial<NT>(3, cubic2_coeff0) );	// coeff of 1
       vP.push_back( Polynomial<NT>(2, cubic2_coeff1) );	// coeff of y
       vP.push_back( Polynomial<NT>(1, cubic2_coeff2) );	// coeff of y^2
       vP.push_back( Polynomial<NT>(0, cubic2_coeff3) );	// coeff of y^3
       CC= Curve<NT>(vP);
       setParams(0.03, -2.0, -2.0, 2.0, 2.0);
       break;
     case 4: // From Sendra's paper
       cout << "Plot of curve from Sendra" << endl;
       cout << "(This takes a bit longer time to compute)" << endl;
       CC = Curve<NT>("-4 y^4 x^2 + 2 y^7 x^2 + y^9 + 3 y^7 - 9 y^6 x^2 - 2 x^8 + 2 x^8 y + 3 x^4 y^5 - y^6 + 4 x^6 y^3 - 7 x^6 y^2 + 5 x^6 y + 10 x^2 y^5 - 11 x^4 y^4 + 9 x^4 y^3 - 4 x^4 y^2 + y^3 x^2 - 3 y^8 ");
       setParams(0.03, -3, -0.2, 3, 2.2);
       break;
     case 5:
       cout << " Cubic3: Y^2 - X(X^2-1)" << endl;
       cout << "  -- Comprises of an oval and a y-monotone component" << endl;
   //component C0 is an oval through (0,0), left of the Y-axis,
   //component C1 is a y-monotone curve through (1,0) symmetric about X-axis
       CC= Curve<NT>("Y^2 - X^3 + X");  // 'X', 'Y' are needed here
       setParams(0.04, -2, -4.5, 2.5, 4.5);
       break;
     case 51: // Cubic3b: Y^2 - X^3
       cout << " Cubic3b: Y^2 - X^3  (variant of Cubic3)" << endl;
       CC= Curve<NT>("y^2 - x^3");
       setParams(0.04, -2, -4.5, 2.5, 4.5);
       break;
     case 6: // Cubic3a: Y^3 - X(X^2-1)
       cout << " Cubic3a: Y^3 - X(X^2 -1)" << endl;
       cout << "  -- Curious variant of Cubic3" << endl;
       CC= Curve<NT>("y^3 - x^3 + x");
       setParams(0.04, -3.0, -3.0, 5.0, 5.0);
       break;
     case 7: // Fulton, p.65
       cout << "Three leaf clover, (X^2+Y^2)^2 + 3X^2 Y - Y^3" << endl;
       CC = Curve<NT>("X^4 + Y^4 + 2X^2Y^2 + 3X^2 Y - Y^3");
       setParams(0.016, -1, -1, 1, 1.5);
       break;
     case 8: // Fulton, p.65
       cout << "Four leaf clover, (X^2+Y^2)^3 - 4 X^2 Y^2" << endl;
       CC = Curve<NT>("X^6 + Y^6 + 3X^2 Y^4 + 3 X^4 Y^2 - 4 X^2 Y^2");
       setParams(0.03, -2, -2, 2, 2);
       break;
     case 9: // Node at origin (Fulton, p.65)
       cout << "Nodal cubic: Y^2 - X^2 - X^3" << endl;
       CC = Curve<NT>("y^2 - x^2 - x^3");
       setParams(0.04, -5.0, -5.0, 5.0, 5.0);
       break;
     case 10: // Example of plotting a univariate polynomial
       // Chebyshev Polynomials of first kind:
       // T0=1, T1=x,
       // 		T(i+1) =  2x* T(i) - T(i-1).
       // T(2) = 2x^2 - 1
       // T(3) = 4x^3 - 3x
       // T(4) = 8x^4 - 8x + 1
       // T(5) =16x^5 -20x^3 + 5x
       cout << "Plot of Chebyshev polynomial (!) T(4)=8X^4 - 8X^2 + 1" << endl;
       CC = Curve<NT>("y - 8 x^4 + 8 x^2 - 1");
       setParams(0.03, -1.1, -1.1, 1.1, 1.1);
       break;
     case 11: // Example of plotting a univariate polynomial
       cout << "Plot of polynomial (!) X^3 + X - 1" << endl;
       CC = Curve<NT>("y - x^3 -  x + 1");
       setParams(0.03, -1.0, -2.0, 1.0, 2.0);
       break;
     case 12: // Folium of Descartes
       cout << "Folium of Descartes, X^3 + Y^3 - 3aXY (a=2)" << endl;
       CC = Curve<NT>("x^3 + y^3 -  6 x y");
       a = 2;
       setParams(0.04, -3*a, -2*a, 3*a, 3*a);
       break;
     case 21: // variant of case 4 (cubic3)
       cout << "Class example: Y^3 - 2X^3 + 4X^2 Y + 2XY^2 + 2X^2" << endl;
       CC= Curve<NT>("y^3 - 2x^3 + 4x^2 y + 2x y^2 + 2x^2");
       setParams(0.04, -3.0, -3.0, 5.0, 5.0);
       break;
     case 22: // Cubic3a: Y^3 - X(X^2-1)
       cout << " Cubic3a: Y^3 - X(X^2 -1)" << endl;
       CC= Curve<NT>("y^3 - x(x^2 - 1)"); // true is needed 'cos of (..)
       setParams(0.04, -3.0, -3.0, 5.0, 5.0);
       break;
     case 23: // Projection Cone from a QSIC (figure of 8)
       cout << " Projection Cone: Y^4 + X^2 Y^2 - X^2 Z^2 [Set Z=1]" << endl;
       CC= Curve<NT>("y^4 - x^2 y^2 - x^2"); 
       setParams(0.03, -3.0, -3.0, 5.0, 5.0);
       break;
     case 24: // Projection Cone from a QSIC (figure of 8)
       cout << " Projection Cone: Y^4 + X^2 Y^2 - X^2 Z^2 [Set Z=0]" << endl;
       CC= Curve<NT>("y^4 - x^2y^2"); 
       setParams(0.03, -3.0, -3.0, 5.0, 5.0);
       break;
     case 25: // Projection Cone from a QSIC (figure of 8)
       cout << " Projection Cone: Y^4 + X^2 Y^2 - X^2 Z^2 [Set Y=1]" << endl;
       CC= Curve<NT>("1 - x^2 -x^2 y^2");  // y is really z
       setParams(0.03, -3.0, -3.0, 5.0, 5.0);
       break;
     case 26: // Projection Cone from a QSIC (figure of 8)
       cout << " Projection Cone: Y^4 + X^2 Y^2 - X^2 Z^2 [Set X=1]" << endl;
       CC= Curve<NT>("y^4 + y^2 - x^2");  // x is really z
       setParams(0.03, -3.0, -3.0, 5.0, 5.0);
       break;
     default:
       cout << "Choose a smaller number for the curve to display!" << endl;
       exit(0);
   }
   // PLOTTING CURVES
   CC.dump("==========================");
   CC.plot(eps, xmin, ymin, xmax, ymax);
   cout << "\n Plot parameters: " << endl;
   showParams();
   
   return 0;
} 
