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
#include "CORE/poly/Curves.h"
#include "box.h"
#include "predicates-inl.h"
#include "point-inl.h"
#include "algorithm-inl.h"
#include "display-list-inl.h"

using namespace std;

void startGlutLoop(int, char**);

// Convert a given maximum or minimum size into a generation
// ID, so all box size comparisons can be performed in
// integer arithmetic.
unsigned int ConvertGeneration(const double &, const Box &);

int main(int argc, char* argv[]) {

  string curve_str = "x^2 - x^4 - y^2 + 1";  // default curve

  // DEFAULT  VALUES
	  double offset_value;
	  bool draw_box = true;	
	  bool display_flag = true;
	  double min_size = 0.0001;
	  double max_size = 100000;
	  Curve<double> bi_poly;
	  double box_v1,box_v2,box_v3,box_v4;
	      box_v1 = -1.5; // xmin
	      box_v2 = -1.5; // ymin
	      box_v3 = 1.5; // xmax
	      box_v4 = 1.5; // ymax

  switch (argc) { // process inputs
	    case 1:
	      // These are the default arguments.
	      bi_poly = bi_poly.getbipoly(curve_str);
	      cout << "Your input Curve is " << bi_poly.toString() << endl;
	      cout << "-------------------" << endl;
	      break;
	    case 11:
	      display_flag = static_cast<bool>(std::atoi(argv[10]));
	    case 10:
	      draw_box = static_cast<bool>(std::atoi(argv[9]));
	    case 9:
	      offset_value =argv[8];
	    case 8:
	      min_size = argv[7];
	    case 7:
	      max_size = argv[6];
	    case 6:
	      box_v4 = argv[5];
	    case 5:
	      box_v3 = argv[4];
	    case 4:
	      box_v2 = argv[3];
		if (box_v2 >= box_v4) {
		  cout << "Input error: Ymin must be less than Ymax\n";
		  exit(1);}
	    case 3:
	      box_v1 = argv[2];
		if (box_v1 >= box_v3) {
		  cout << "Input error: Xmin must be less than Xmax\n";
		  exit(1);}
	    case 2:
	      curve_str = argv[1];
	      bi_poly = bi_poly.getbipoly(curve_str);
	      bi_poly.dump("Your input Curve is ");
	      if (offset_value != 0) {
	          cout<<"\n============="<<endl;
	          (bi_poly.coeffX[0]).setCoeff(0,
	              (bi_poly.coeffX[0]).getCoeff(0) + offset_value);
	          bi_poly.dump("Input Curve with offset is");
	      }
	      cout<<"\n-------------"<<endl;
	      break;
	    default:
	      cout << "Synopsis:" << endl;
	      cout << "./cxy \"curveDef\" [Xmin] [Ymin] [Xmax] [Ymax] \n"
		  << "       [max-size] [min-size] [offset] [show-boxes-flag]  "<< endl
		  << "All arguments are optional" << endl
	          << "The default curve is \"x^2 - x^4 - y^2 +0.01\" " << endl
	          << "The default box is \"-1.5,-1.5,1.5,1.5\" " << endl
	          << "E.g. ./cxy   \"y - x^2\" " << endl
	          << "E.g. ./cxy   \"(y - x^2)(x+y-2)\" -1.4 -1.4 1.6 1.6" << endl
	          << "E.g. ./cxy   \"y - x^2 = x+y-2\" -1.4 -1.4 1.6 1.6" << endl;
	      exit(1);
	  }

  // The bounding box for the algorithm to run on.
  Point P1(box_v1, box_v2);
  Point P2(box_v3, box_v4);
  Box B0(P1, P2);

  // These parameters determine the maximum and minimum extent
  // of the subdivision.
  const unsigned int min_gen = ConvertGeneration(max_size, B0);
  const unsigned int max_gen = ConvertGeneration(min_size, B0);

  // The predicate supplier for the algorithm.
  CxyPredicates<double> preds(bi_poly, min_gen, max_gen);
  // Run the algorithm.
  Algorithm2D<CxyPredicates<double> > cxy(&preds, &B0, max_gen, min_gen);

  // start timing code.
  struct timeval start;
  struct timeval end;
  struct timezone DUMMY;
  gettimeofday(&start, &DUMMY);
  // end timing code.

  // Run the Cxy algorithm:
  cxy.run(draw_box);
  // Construct the curve from the algo.
  vector<pair <Point *, Point *> > line_output;
  display_list::ConstructCurve(*(cxy.q3()), &line_output);

  gettimeofday(&end, &DUMMY);
	cerr << "The total time taken was = " 
	  << (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000
	  << " ms" << endl;

  // Set display parameters.
  if (! display_flag) return 0;

  display_funcs::SetDisplayParams(&B0, draw_box,
      cxy.c0(), cxy.q3(), cxy.unresolved(), &line_output);

  startGlutLoop(argc, argv);
  return 0;
}//main

unsigned int ConvertGeneration(const double &sz, const Box &b0) {
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

void startGlutLoop(int argc, char **argv) {
	  cout << "-------------Graphic----------------"<<endl;
	  cout << "--------Press ESC to exit-----------"<<endl;
	
	  // Various open GL related things.
	  glutInit(&argc, argv);
	  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	  glutInitWindowSize(600, 600);
	  glutInitWindowPosition(100, 100);
	  glutCreateWindow("CXY       (Press ESC to exit)");
	  display_funcs::ClearBackground();
	  glutReshapeFunc(display_funcs::ReshapeHandler);
	  glutDisplayFunc(display_funcs::DisplayHandler);
	  glutKeyboardFunc(display_funcs::KeyHandler);
	  glutMainLoop();
	}

