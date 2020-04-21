/* **************************************
   File: plotCurve.cpp

   Description: 
	This read a curve definition from the command line,
		possibly with other arguments (eps, xmin, ymin, etc).
	A plot this curve is stored in the file data/plot.
	To display this plot, you can call our openGL
              drawcurve program found in CORE_PATH/ext/graphics.
	      (Using our Makefile, just type "make show" or "make s")

   Usage:
        > ./plotCurve <curveDef> [eps] [Xmin] [Ymin] [Xmax] [Ymax] [extraParam]

	E.g.,

	> ./plotCurve "y - x^2" 	        -- plots a parabola 

	> ./plotCurve "y^2 - x^3 + 3 xy" 0.01 	-- plots a cubic at x-resolution
	                                           of 0.01.

       Only the first argument is mandatory:
                    <curveDef> is a string defining a bivariate
		    polynomial in the variables 'x' and 'y'  (case sensitive)

       If eps, Xmin, etc, are not specified, they default to some values:
              eps = 0.03,
	      Xmin = Ymin = -10
	      Xmax = Ymax = 10
	      extraParam = "" (string)

   TODO LIST:
	--Plot should be able to do a simple automatic analysis of the curve
	  to determine the domain of most interest.
	--Also plot point using the y-projection if the gaps between
	  plot points is large.

   Author:  Chee Yap 
   Date:    June 20, 2004

   Since Core Library  Version 1.7
   $Id: plotCurve.cpp,v 1.3 2010/05/27 17:19:07 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "localCurves.h"	// this is the local version of Curves.h
//#include "CORE/poly/Curves.h"	// this is the official version of Curves.h

using namespace std;

typedef BigInt				NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;

// =========================================================
// GLOBAL PLOT PARAMETERS
// =========================================================
#define NaN 100000
string curveDef;	// string defining the curve polynomial e.g., y^2 -x^3
BigFloat eps = 0;	// precision of the plot (i.e., delta-x)
BigFloat xmin = NaN;	// min x 
BigFloat ymin = NaN;	// min y
BigFloat xmax = NaN;	// max x
BigFloat ymax = NaN;	// max y
string extra = "";	// extra curve parameter (depending on curve)

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
	if (argc <= 1){
	   cout << "usage: plotCurve <polynomial-as-string> [eps] " 
		<< "             [xmin] [ymin] [xmax] [ymax] ..." << endl
		<< " (where <...> is mandatory argument)" << endl
		<< "E.g. plotCurve \"y - x^2\" 0.02 -1 -1 1 1" << endl
		<< "The output file is data/plot" << endl;
	   exit(1);}
	if (argc>1) curveDef = argv[1];
		cout << "Your curve is: " << curveDef << endl;
	eps = 0.03;
		if (argc>2) eps = BigFloat(argv[2])/*.makeExact()*/;
	xmin = -10.0;
	if (argc>3) xmin = BigFloat(argv[3])/*.makeExact()*/;
	ymin = -10.0;
	if (argc>4) ymin = BigFloat(argv[4])/*.makeExact()*/;
	xmax = 10.0;
	if (argc>5) xmax = BigFloat(argv[5])/*.makeExact()*/;
	ymax = 10.0;
	if (argc>6) ymax = BigFloat(argv[6])/*.makeExact()*/;
	extra = "";
	if (argc>7) extra = argv[7];	// extra parameter

    // SET INTERNAL PRECISION
    //setDefaultRelPrecision(CORE_posInfty); // This puts defAbsPrec in control
    int prec = 10;	// Do Sturm refinement till delta
                        // is at most 2^{-prec}
    //setDefaultAbsPrecision(prec);

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits

   // CONSTRUCT EQUATION OF CURVE
   

cout<< "DEBUG: before segment fault" << endl;
   Curve<NT> CC(curveDef);
cout<< "DEBUG: after segment fault" << endl;

   setParams(eps,xmin,ymin,xmax,ymax);
   // PLOTTING CURVES
   cout << "==========================" << endl;
   cout << extra << endl;
   CC.plot(eps, xmin, ymin, xmax, ymax);
   cout << "\n Plot parameters: " << endl;
   showParams();
   
   return 0;
} 


