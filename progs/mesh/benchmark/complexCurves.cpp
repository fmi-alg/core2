/* **************************************
   File: complexCurves.cpp

   Description: 
	Given an input polynomial p(z) with real coefficients,
	we construct the real and complex bivariate polynomials,
	u(x,y) and v(x,y) where p(x+iy)=u(x,y)+i v(x,y).
	We then compute the real and complex curves u=0 and v=0
	in the complex plane, using the cxy algorithm.

	The arguments are rather similar to those for cxy,
	but combined with elements from ceval (for complex roots).

   Usage:
        > ./complexCurves  ["curveDef"]   [Xmin] [Ymin] [Xmax] [Ymax] \
                       [max-size] [min-size] [offset] [drawbox] [display]

   Author: Chee Yap
   Since Nov, 2010, Core Library  Version 2.1
   $Id: complexCurves.cpp,v 1.3 2010/11/08 21:14:16 exact Exp $
 ************************************** */

#define CORE_LEVEL 4

#include <list>

#include "tclap/CmdLine.h"
#include <sys/time.h>

#include "../cxy/display.h"
#include "../cxy/cxy-defs.h"
#include "../cxy/box.h"
#include "../cxy/predicates-inl.h"
#include "../cxy/point-inl.h"
#include "../cxy/algorithm-inl.h"
#include "../cxy/display-list-inl.h"
//#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"
#include "CORE/ComplexT.h"

//#include "../ceval/ceval-defs.h"
//#include "../ceval/algorithm.h"
//#include "../ceval/predicates.h"
//#include "../ceval/tmp-display.h"


// Note that this does not include "benchmark.h" (nope)
#include "benchmark.h"


using namespace std;

// ##################################################
// Eventually to be moved out:
// ##################################################
	// this is from ceval/ceval-defs.h
	typedef machine_double NT;
	typedef ComplexT<double> Complex;

// ##################################################
// Argument processing
	// Value Arguments
	TCLAP::ValueArg<string> poly("p", "poly", "Input polynomial file name",
		false, "", "string");
	TCLAP::ValueArg<unsigned int> rand_degree("g", "degree",
		"Degree of random poly (default 10)", false, 10, "unsigned int");
	TCLAP::ValueArg<unsigned int> rand_seed("f", "seed",
		"Seed (default 10)", false, 10, "unsigned int");
	TCLAP::ValueArg<double> offset_value("o", "offset_value",
		"Offset to be added to polynomial", false, 0.0, "double");
	TCLAP::ValueArg<double> min_size("m", "min_size",
		"Smallest size of subdivision boxes", false, 0.001, "double");
	TCLAP::ValueArg<double> max_size("M", "max_size",
		"Largest size of subdivision boxes", false, 10000.0, "double");
	TCLAP::ValueArg<double> x_min("x", "x_min",
		"Min x value of initial box", false, -2.0, "double");
	TCLAP::ValueArg<double> y_min("y", "y_min",
		"Min y value of initial box", false, -2.0, "double");
	TCLAP::ValueArg<double> x_max("X", "x_max",
		"Max x value of initial box", false, 2.0, "double");
	TCLAP::ValueArg<double> y_max("Y", "y_max",
		"Max y value of initial box", false, 2.0, "double");

	// Switch Arguments
	TCLAP::SwitchArg random_poly("a", "random",
		"Use a random polynomial", false);
	TCLAP::SwitchArg verbose("v", "verbose",
		"Use verbose mode: show polynomials", true);
	TCLAP::SwitchArg interactive("i", "interactive_mode",
		"Do interactive display", true);
	TCLAP::SwitchArg draw_box("d", "draw_box",
		"Do draw subdivision boxes (if interactive mode)", true);

// ##################################################
// Routines to be defined:
	// Glut Loop:
	//void startGlutLoop(int, char**);
	// Converting box dimensions into generational ID (integer):
	unsigned int ConvertGeneration(const double &, const Box &);

// ##################################################
// MAIN
int main(int argc, char **argv) {

  try { // Argument Processing
	    TCLAP::CmdLine cmd("CEVAL Algorithm", ' ', "0.1");
	    cmd.add(poly);
	    cmd.add(rand_degree);
	    cmd.add(rand_seed);
	    cmd.add(offset_value);
	    cmd.add(min_size);
	    cmd.add(max_size);
	    cmd.add(x_min);
	    cmd.add(y_min);
	    cmd.add(x_max);
	    cmd.add(y_max);
	// Switch args:
	    cmd.add(random_poly);
	    cmd.add(verbose);
	    cmd.add(interactive);
	    cmd.add(draw_box);
	
	    // Parse the args.
	    cmd.parse(argc, argv);
	
  } catch (TCLAP::ArgException &e) {
	    cerr << "Error : " << e.error() << endl;
	    cerr << "Processing arg : " << e.argId() << endl;
	    return -1;
	  }

  // Get the value parsed by each arg.
	    double xmin=x_min.getValue();
	    double ymin=y_min.getValue();
	    double xmax=x_max.getValue();
	    double ymax=y_max.getValue();

  // Get the input polynomial a (from file or randomly generated)
	  Polynomial<NT> a;
	  if (random_poly.getValue()) {
	    benchmark::GenerateRandom(&a, rand_degree.getValue(), 10 ,rand_seed.getValue());
	  } else {
	    benchmark::GetPoly(poly.getValue().c_str(), &a);
	  }
	  if (verbose.getValue())
	    cout << "Input Polynomial is: " << a.toString() << endl;

  // Convert input polynomial to the real and complex parts, u, v (and uv):
	BiPoly<NT> u, v;
  	benchmark::ConvertPoly(a,&u,&v); 
	if (verbose.getValue()) {
	    cout << "\n --- Real Part Polynomial is u(x,y): "
		    << u.toString() << endl;
	    cout << "\n --- Complex Part Polynomial is v(x,y): "
		    << v.toString() << endl;
	}
	Curve<NT> uv = u*v;

  // The bounding box for the algorithm to run on.
	  Point P1(xmin, ymin);
	  Point P2(xmax, ymax);
	  Box B0(P1, P2);

/*
  // These parameters determine the maximum and minimum extent of the subdivision.
	  //const unsigned int min_gen = ConvertGeneration(max_size.getValue(), B0);
	  const unsigned int min_gen = 4;
	  //const unsigned int max_gen = ConvertGeneration(min_size.getValue(), B0);
	  const unsigned int max_gen = 100;

  // Setup the predicate object, "preds"
	 CxyPredicates<NT> preds(uv, min_gen, max_gen);

  // Setup the algorithm object, "cxy"
	 Algorithm2D<CxyPredicates<double> > cxy(&preds, &B0, max_gen, min_gen);

  // Get start time 
	  struct timeval start;
	  struct timeval end;
	  gettimeofday(&start, NULL);
	  // end timing code.

  // Run the Cxy algorithm:
	cxy.run(draw_box.getValue()); 
  // Construct Curve:
	

  // Get finish time; hence total time
	gettimeofday(&end, NULL);
	cout << "\n Total Time="
	  << (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec)
	  << endl;
*/
}//main

// ##################################################
// Routines needed

unsigned int ConvertGeneration(const double &sz, const Box &b0) {
	  const double x_div = b0.getX()->width() / sz;
	  const double y_div = b0.getY()->width() / sz;
	
	  double choice = ceil(log2(std::max(x_div, y_div).doubleValue()));
	
	  // Should we warn on this condition ? This means
	  // the max box size is greater than the input area.
	  if (choice < 0) {
	    choice = 0;
	  }
	  return 10;//hack
	//  return static_cast<unsigned int>(choice.doubleValue());
	}

/*
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
*/

// ##################################################
// END
// ##################################################
