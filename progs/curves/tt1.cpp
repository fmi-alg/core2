/* **************************************
   File: moreCurves.cpp

   Description: 
   	more plot and display of Curves

   Usage:
        > ./tCurves [n=1] [eps=0.5] [minX=-2] [minY=-10] [maxX=10] [maxY=10]

       This will output a plot of the n-th curve in this program
	      (currently, n=0,1,...,4).
       The output is in the file data/plot.
       You can display this curve on your screen using our simple
              drawcurve program found in CORE_PATH/ext/graphics.

   BUGS and TODO LIST:
       Plot should be able to do a simple automatic analysis of the curve
       to determine the domain of most interest.

   Author:  Chee Yap and Vikram Sharma
   Date:    Apr 20, 2004

   Since Core Library  Version 1.7
   $Id: tt1.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "Curves.h"

using namespace std;

typedef BigInt				NT;
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
	int curveNo = 0;	// which curve to display
	if (argc>1) curveNo = atoi(argv[1]);
	if (argc>2) eps = BigFloat(argv[2]);
	if (argc>3) xmin = BigFloat(argv[3]);
	if (argc>4) ymin = BigFloat(argv[4]);
	if (argc>5) xmax = BigFloat(argv[5]);
	if (argc>6) ymax = BigFloat(argv[6]);


    // SET INTERNAL PRECISION
    //setDefaultRelPrecision(CORE_posInfty); // puts defAbsPrec in control
    int prec = 10;       // Do Sturm refinement till delta
                         // is at most 2^{-prec}
    //setDefaultAbsPrecision(prec);

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); // convert prec bits into prec/3 digits

    string str;
    //Curve<NT> CC("Y^2 - X^2 - X^3");
    Curve<NT> CC(" 2 x^4 - 3 x^2 y + y^2 - 2y^3 + y^4");
    CC.dump();
    CC.plot(0.1,-5, -10, 10, 10, 2);
    
    return 0;
}

