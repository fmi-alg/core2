/**
 * File: mesh/arrangements/main.cpp
 *
 * Description:
 * 	Driver program for the "modified-miranda" algorithm.
 *	The modified-miranda test is from Moore-Kioustelides ("mk").
 *
 * 	This algorithm uses subdivision of boxes to strongly
 * 	isolate all the intersections of two real polynomial curves
 * 	within some given box B.
 *
 * 	PRECONDITION:
 * 	the intersections inside B must be simple (transversal)
 * 
 * Usage:
 *      This program takes the following arguments (processed via tclap):
 *      All arguments are optional (default values shown):
 *
 *         -f or --fxy [first_curve = "y-x^2"]	
 *         -g or --gxy [second_curve = "x^2+y^2-1"]
 *         -p or --poly [input_file_containing_complex_polynomial]
 *         	        (this produces 2 real curves, in lieu of fxy and gxy)
 *         -x or --x_min [xval = -2]
 *         -X or --x_max [xval2 = 2]
 *         -y or --y_min [yval = -2]
 *         -Y or --y_max [yval2 = 2]
 *         -m or --minsize [min_size_of_boxes = 0.0001]
 *         -M or --maxsize [max_size_of_boxes = 0.1]
 *         -r or --maxgen [maximum_generation = 15]
 *
 *  It is compiled into "mk" by our Makefile.  
 *  See the Makefile for examples of usage.  E.g.,  
 *
 *  		> make		-- to compile
 *  		> make test	-- basic test
 *  		> make eg1
 *  		> make eg2
 *  		> ... many more
 *
 *  NOTES on Number Types:
 *  There are two types of dyadic numbers, 
 *       DT = is the number type for defining subdivision boxes
 *       NT = is the number type for for endpoints of intervals used for evaluation.
 *       We require that DT <= NT (subtype relation).
 *       Typically, DT = NT = DoubleWrapper.
 *       But for increased precision, we may use NT = BigFloat.
 *
 * Author:  Vikram Sharma
 *
 * Date:    June, 2012
 *
 * Since Core Library Version 2.1
 *  $Id: $
 */

#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <iostream>
#include <tclap/CmdLine.h>

#include "algorithm.h"
#include "tmp-display.h"
#include "benchmark.h"
#include "rootbox.h"

using namespace std;

//typedef DoubleWrapper DT;
//typedef DoubleWrapper NT;
typedef BigFloat NT;
typedef BigFloat DT;


void startGlutLoop(int argc, char **argv);

// statistical variable
unsigned int largest_gen = 0;
unsigned int c0_excluded = 0;
unsigned int jacobian_excluded = 0;
unsigned int mk_excluded = 0;

// command line arguments (tclap install required)
// ValueArg parameters:
// 1:flag     2: name     3: description  4: required or not  5: default  6: optional visitor
TCLAP::ValueArg<string> f_xy ("f", "fxy", "first curve of the system", false, "y-x^2", "string");
TCLAP::ValueArg<string> g_xy ("g", "gxy", "second curve of the system", false, "x^2+y^2-1", "string");
TCLAP::ValueArg<string> x_minimum("x", "x_min", "Lower bound of x range", false, "-2", "string");
TCLAP::ValueArg<string> x_maximum("X", "x_max", "Upper bound of x range", false, " 2", "string");
TCLAP::ValueArg<string> y_minimum("y", "y_min", "Lower bound of y range", false, "-2", "string");
TCLAP::ValueArg<string> y_maximum("Y", "y_max", "Upper bound of y range", false, " 2", "string");
TCLAP::ValueArg<string> Poly ("p", "poly",  "file name of the polynomial", false, "", "string");
TCLAP::ValueArg<string> min_box_size("m", "minsize", "minimum size of box", false, "0.0001", "string");
TCLAP::ValueArg<string> max_box_size("M", "maxsize", "maximum size of box", false, "0.1",    "string");
TCLAP::ValueArg<string> max_generation("r", "maxgen", "maximum generation", false, "15", "string");


int main(int argc, char **argv) {

  // use DT for arithmetics
  typedef IntervalT<DT> IntervalDT;
  typedef IntervalT<NT> IntervalNT;
  typedef BoxT<DT> Box;
  typedef BiPoly<DT> poly;

  // queues that used for display
  std::vector< RootBoxT<DT,NT> *> output;
  std::vector<const Box *> ambiguous;
  std::vector<const Box *> exclude;
  std::vector<const Box *> outer_output;
  std::vector<const Box *> inner_output;

  // default values for arguments, all arguments can be 
  // specified in Makefile

  // This is the maximum number of ambiguous boxes to be printed
  unsigned int max_ambiguous_box = 20;

  DT x_min = -3, x_max = 3, y_min = -3, y_max = 3;
  DT min_size = 0.0001;
  DT max_size = 0.01;
  int max_gen = 15;
  string fxy_str = "y - (x-1)^2 - 1";	// default fxy
  // string gxy_str = "x - 1";		// default gxy
  string gxy_str = "x^2 + y^2 - 4";	// default gxy
  poly fxy;
  poly gxy;

  try {
    TCLAP::CmdLine cmd("MK test", ' ', "1.0");
    cmd.add(Poly);
    cmd.add(x_minimum);
    cmd.add(x_maximum);
    cmd.add(y_minimum);
    cmd.add(y_maximum);
    cmd.add(min_box_size);
    cmd.add(max_box_size);
    cmd.add(f_xy);
    cmd.add(g_xy);
    cmd.add(max_generation);

    // parse the arguments
    cmd.parse(argc, argv);
  }
  catch (TCLAP::ArgException &e) {
    cerr << "Error : " << e.error() << endl;
    cerr << "Processing arg : " << e.argId() << endl;
    return -1;
  }
  
	std::cout << "Reading poly " << Poly.getValue()<< endl;

  if(!(f_xy.getValue()).empty() && !(g_xy.getValue()).empty() && (Poly.getValue()).empty()) {
	fxy_str = f_xy.getValue();
    gxy_str = g_xy.getValue();
	fxy = fxy.getbipoly(fxy_str);
    gxy = gxy.getbipoly(gxy_str);
  }
  else if(!(Poly.getValue()).empty()) {
    benchmark::GetBiPoly(Poly.getValue().c_str(), &fxy, &gxy);
  }
  else {
    cout << "you should either specify a file name or provide 2 bipolynomials!" << endl;
    return -1;
  }

	
  x_min = x_minimum.getValue();
  x_max = x_maximum.getValue();
  y_min = y_minimum.getValue();
  y_max = y_maximum.getValue();
  min_size = min_box_size.getValue();
  max_size = max_box_size.getValue();
  max_gen = atoi(max_generation.getValue().c_str());
  // initial box construction
  IntervalDT x_range(x_min, x_max);
  IntervalDT y_range(y_min, y_max);
  Box *const box = new Box(0, x_range, y_range);


  struct timeval start;
  struct timeval end;
  // start time
  gettimeofday(&start, NULL);
  
  cout << "Starting Algorithm"<<endl;
  // get the algorithm running
  MKPredicates<DT,NT> *const pred = 
      new MKPredicates<DT,NT>(fxy, gxy, min_size, max_size, max_gen);

  Algorithm::Run<DT,NT>(*pred, box, 
      &output, &ambiguous, &exclude);

  // end time
  gettimeofday(&end, NULL);

  cout << "------------- Statistics ----------------"<<endl;
  cout << "Polynomial fxy = " << fxy.toString() << endl;
  cout << "Polynomial gxy = " << gxy.toString() << endl;

  unsigned int num_includes = output.size();      // statistic collections

  cout << endl << "Output regions: " << endl;
  for(unsigned int i = 0; i < output.size(); i++) {
    RootBoxT<DT,NT> *b = output[i];
    cout << "X: " << b->innerBox_->x_range << " , Y: " << b->innerBox_->y_range << endl;
    // we make full use of this loop, while going though each element in output,
    // also put outer and inner into 2 separate queues for display
    outer_output.push_back(b->outerBox_);
    inner_output.push_back(b->innerBox_);
  }

  // DISPLAY OF AMBIGUOUS BOXES:
  unsigned int num_ambiguous = ambiguous.size();   // statistic collections
  cout << "Ambiguous regions: " << endl;
  int ratio=1;
  if (ambiguous.size() > max_ambiguous_box) {
      ratio = 1+(num_ambiguous/max_ambiguous_box);
      cout << " Too many ambiguous boxes -- show only a sampled subset of "
		<< max_ambiguous_box << " boxes" << endl;
      cout << " print every " << ratio << "-th boxes" << endl;
  }
  for(unsigned int i = 0; i < num_ambiguous; i++) {
    if((i % ratio) == 0 ) {
       const Box *b = ambiguous[i];
       cout << "X: " << b->x_range << " , Y: " << b->y_range << endl;
    }
  }
  

  unsigned int num_excludes = exclude.size();  // statistic collections

  cout << endl;
  cerr << "The total time taken was = " <<
      (end.tv_sec - start.tv_sec)*1000000 + 
      (end.tv_usec - start.tv_usec) << " micro seconds" << endl;

  cout << endl << "Statistics: " << endl;
  cout << " -- output regions: " << num_includes << endl;
  cout << " -- ambiguous regions: " << num_ambiguous << endl; 
  cout << " -- exclusion regions: " << num_excludes << endl; 
  cout << " -- maximum subdivision depth: " << largest_gen << endl;
  cout << endl;

  // display preparation
  Box *b_display = new Box(0, x_range, y_range);
  display_funcs::SetDisplayParams(b_display, &exclude, &outer_output,
                                  &inner_output, &ambiguous);
  // draw plot
  startGlutLoop(argc, argv);
  delete b_display;
  delete pred;


  return 0;
}

void startGlutLoop(int argc, char **argv) {
  cout << "-------------Graphic----------------"<<endl;
  cout << "--------Press ESC to exit-----------"<<endl;

  // Various open GL related things.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MK       (Press ESC to exit)");
  display_funcs::ClearBackground();
  glutReshapeFunc(display_funcs::ReshapeHandler);
  glutDisplayFunc(display_funcs::DisplayHandler);
  glutKeyboardFunc(display_funcs::KeyHandler);
  glutMainLoop();
}

