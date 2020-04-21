/* **************************************
   File: cxy.cpp

   Description: 
	Implementation of the Cxy algorithm in SoCG paper of Long and Yap
	for approximation curves represented by a bi-variate polynomial.
	The input polynomial is given as a string in variables x and y,
		with very flexible format (see below).
		Only integer coefficients supported.
	The output is displayed in an OpenGL port.  The subdivision boxes
		are color coded: In-boxes are green, Out-boxes are
		red and unresolved-boxes are blue (i.e., it reached min_size).
   Usage:

        > ./cxy  ["curveDef"]   [Xmin] [Ymin] [Xmax] [Ymax] \
                       [max-size] [min-size] [offset] [drawbox] [display]

		There are 10 arguments.
		Every argument is optional (but since the arguments
		   are positional, you can omit the n-th argument only if
		   you omit (n+1)-st argument, etc.)
		The default values are:
              		(1) The default Curve    is   [x^2 - x^4 - y^2 +0.01]
	                (2-5) The default box      is   [-1.5, -1.5, 1.5, 1.5]
			(6) The default max-size is   100000
			(7) The default min-size is   0.0001
			(8) The default offset   is   0
			(9) The default drawbox  is   true
			(10) The default display  is   true

		EXPLANATION:
		-- The curve definition is a string representing a polynomial 
			in x and y.  The format is very flexible:
		-- max-size = m  means that ANY candidate box
				of size > m will be split
			(Thus, m is the resolution of the curve)
		-- min-size = m  means that NO candidate box
				of size < m will be split
			(This prevents an infinite loop at singularies)
		-- drawbox is true means we will display the subdivision boxes
		   (Otherwise, we just display the isotopic curve -- may look odd, but OK)
		-- display is false means do not show anything.
	           THIS IS IMPORTANT in non-interactive mode (e.g., make test).
		   Logically, "display" should come after "drawbox".
		   Also, we ought to print some information about the analyzed curve,
		   e.g., number of components, etc.  Future work.

	E.g.,
	> ./cxy				-- plot default curve
	> ./cxy "y - x^2" 	        -- plots a parabola 

	> ./cxy "y^2 = x^3 - 3 xy" 	-- plots a cubic 
				(Note we can use equality sign in the equation)

	> ./cxy "x^3y + x(y-2x) = 1" -1 -1 3 3
				-- plots the curve in the box [-1,-1,3,3]

	> ./cxy "y - x^2" -10 -10 10 10 0.1 0.07
				-- The last two arguments tells us that
				the plot resolution is 0.1, and
				the offset is 0.02.  This means we are
				plotting the curve "y - x^2 = 0.07"


   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)
   COMMENTS (Chee)
	Original code was slow because it assumes Level 3
	Narayan introduced Wrapper for Level 1, so code can compile in
	   all three Levels (Level 1 is much faster)
	We should allow position-independent arguments (using getopt).
	Should provide an option to spill the output into a file for 
	  subsequent viewing and analysis.

   Since Core Library  Version 2.0
   $Id: cxy.cpp,v 1.12 2011/02/09 22:41:14 exact Exp $
 ************************************** */

#include <iostream>
#include <string>
#include <sys/time.h>

#include "display.h"

#include "cxy-defs.h"
#include "predicates-inl.h"
#include "point-inl.h"
#include "algorithm-inl.h"
#include "display-list-inl.h"

using namespace std;

namespace cxy {

unsigned int ConvertGeneration(const double &sz, const Box &b0) 
{
	const double x_div = b0.getX()->width() / sz;
	const double y_div = b0.getY()->width() / sz;

	double choice = ceil(log2(std::max(x_div, y_div).doubleValue()));

	// Should we warn on this condition ? This means
	// the max box size is greater than the input area.
	if (choice < 0) {
		choice = 0;
	}

	return static_cast<unsigned int>(choice.doubleValue());
}

void cxy(Curve<double>& bi_poly, Box * b0, vector<pair<Point *, Point *> >& line_segs)
{
    double min_size = 0.0001;
    double max_size = 100000;

    // These parameters determine the fineness of the curve
    const unsigned int min_gen = 9;
    const unsigned int max_gen = 12;

    // The predicate supplier for the algorithm.
    CxyPredicates<double> preds(bi_poly, min_gen, max_gen);

    // Run the algorithm.
    Algorithm2D<CxyPredicates<double> > cxy(&preds, b0, max_gen, min_gen);

    // Run the Cxy algorithm:
    cxy.run(false);

    // Construct the curve from the algo.
    ConstructCurve(*(cxy.q3()), &line_segs);
}
	
void cxy(const std::string& curve_str, Box * b0, vector<pair<Point *, Point *> >& line_segs)
{
	Curve<double> bi_poly = bi_poly.getbipoly(curve_str);
	cxy(bi_poly,b0,line_segs);
}

} //namespace cxy 



