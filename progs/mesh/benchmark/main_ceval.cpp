/* **************************************
   File: progs/mesh/benchmark/main_ceval.cpp

   Description: 
		This program is a GLUT based visualization of the CEVAL Algorithm
		of Sagralloff and Yap.

		The actual implementation of CEVAL is found in
		        progs/mesh/ceval/ceval.cpp

		Arguments to main_ceval are all optional:

		E.g., To find the roots of the polynomial in input file "nroots20.pol"

		> ./main_ceval --poly ./data/nroots20.pol --display --use_root_bounds

		or equivalently

		> ./main_ceval --d --r --p ./data/nroots20.pol

		The flag --poly gives a file name containing a polynomial in frisco format
		The flag --display is a switch to turn on the display of subdivision boxes
		The flag --use_root_bounds is a switch to automatically
			determining a bounding box containing all complex roots.
			Otherwise, you may give explicit specification of the box using

				--x_min, --y_min, --x_max, --y_max.
			or
				--x, --y, --c, --u
		
		We use the TCLAP package to process command line arguments.
		Each of these flags have default values.
		To learn more, type:

		> ./main_ceval --help

	TCLAP::ValueArg<string> poly("p", "poly", "Input polynomial file name",
		false, "", "string");
	TCLAP::ValueArg<string> x_min("x", "x_min", "Minimum x range (default -2)",
		false, "-2", "string");
	TCLAP::ValueArg<string> x_max("c", "x_max", "Maximum x range (default 2)",
		false, "2", "string");
	TCLAP::ValueArg<string> y_min("y", "y_min", "Minimum y range (default -0.05)",
		false, "-0.05", "string");
	TCLAP::ValueArg<string> y_max("u", "y_max", "Maximum y range (default 0.05)",
		false, "0.05", "string");
	TCLAP::ValueArg<string> min_box_size("m", "min_box_size",
		"Minimum box size (default 0.00001)",
		false, "0.00001", "string");
	TCLAP::ValueArg<string> max_box_size("M", "max_box_size",
		"Maximum box size (default 0.1)",
		false, "0.1", "string");
	TCLAP::ValueArg<unsigned int> rand_degree("g", "degree",
		"Degree of random poly (default 10)",
		false, 10, "unsigned int");
	TCLAP::ValueArg<unsigned int> rand_seed("f", "seed", "Seed (default 10)",
		false, 10, "unsigned int");
	TCLAP::SwitchArg use_rb("r", "use_root_bounds",
		"Use Cauchy Bounds", false);
	TCLAP::SwitchArg display("d", "display",
		"Display subdivision tree", false);
	TCLAP::SwitchArg no_use_inclusion("i", "no_use_inclusion",
		"Do not use the inclusion pred", false);
	TCLAP::SwitchArg random_poly("a", "random",
		"Use a random polynomial", false);


   Author:  Narayan Kamath
   Date:    2010

   Since Core Library  Version 2.1
   $Id: CXY.cpp,v 1.3 2010/06/10 16:51:46 exact Exp $
 ************************************** */

#include <list>

#include "tclap/CmdLine.h"
#include <sys/time.h>

// Note that this does not include "benchmark.h"
#include "../ceval/ceval-defs.h"
#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"

#include "../ceval/algorithm.h"
#include "../ceval/predicates.h"
#include "../ceval/tmp-display.h"

#include "benchmark.h"

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

// Arg processing:
	TCLAP::ValueArg<string> x_min("x", "x_min", "Minimum x range (default -2)",
		false, "-2", "string");
	TCLAP::ValueArg<string> x_max("c", "x_max", "Maximum x range (default 2)",
		false, "2", "string");
	TCLAP::ValueArg<string> y_min("y", "y_min", "Minimum y range (default -0.05)",
		false, "-0.05", "string");
	TCLAP::ValueArg<string> y_max("u", "y_max", "Maximum y range (default 0.05)",
		false, "0.05", "string");
	TCLAP::ValueArg<string> poly("p", "poly", "Input polynomial file name",
		false, "", "string");
	TCLAP::ValueArg<string> min_box_size("m", "min_box_size",
		"Minimum box size (default 0.00001)", false, "0.00001", "string");
	TCLAP::ValueArg<string> max_box_size("M", "max_box_size",
		"Maximum box size (default 0.1)", false, "0.1", "string");
	TCLAP::ValueArg<unsigned int> rand_degree("g", "degree",
		"Degree of random poly (default 10)", false, 10, "unsigned int");
	TCLAP::ValueArg<unsigned int> rand_seed("f", "seed", "Seed (default 10)",
		false, 10, "unsigned int");
	
	TCLAP::SwitchArg use_rb("r", "use_root_bounds",
		"Use Cauchy Bounds", false);
	TCLAP::SwitchArg display("d", "display",
		"Display subdivision tree", false);
	TCLAP::SwitchArg no_use_inclusion("i", "no_use_inclusion",
		"Do not use the inclusion pred", false);
	TCLAP::SwitchArg random_poly("a", "random",
		"Use a random polynomial", false);

int main(int argc, char **argv) {

  try {
	    TCLAP::CmdLine cmd("CEVAL Algorithm", ' ', "0.1");
	    cmd.add(poly);
	    cmd.add(x_min);
	    cmd.add(x_max);
	    cmd.add(y_min);
	    cmd.add(y_max);
	    cmd.add(use_rb);
	    cmd.add(display);
	    cmd.add(no_use_inclusion);
	    cmd.add(min_box_size);
	    cmd.add(max_box_size);
	    cmd.add(random_poly);
	    cmd.add(rand_degree);
	    cmd.add(rand_seed);
	
	    // Parse the args.
	    cmd.parse(argc, argv);
	
	    // Get the value parsed by each arg.
	    //string name = nameArg.getValue();
  } catch (TCLAP::ArgException &e) {
	    cerr << "Error : " << e.error() << endl;
	    cerr << "Processing arg : " << e.argId() << endl;
	    return -1;
  }

  // Get input polynomial a:
	  Polynomial<PolyType> a;
	  if (random_poly.getValue()) {
	    benchmark::GenerateRandom(&a, rand_degree.getValue(), 10 ,rand_seed.getValue());
	  } else {
	    benchmark::GetPoly(poly.getValue().c_str(), &a);
	  }

  double max_box_size_d(max_box_size.getValue());
  double min_box_size_d(min_box_size.getValue());

  // Get dimension of initial box
	  Complex min, max;
	  if (use_rb.getValue()) {
	    double min_r, max_r;
	    benchmark::GetRootBounds(a, &min_r, &max_r);
	    min = Complex(min_r, min_r);
	    max = Complex(max_r, max_r);
	    if (display.getValue()) {
	      cout << "Min : " << min << ",Max : " << max << endl;
	    }
	  } else {
	    double x_min_d(x_min.getValue()), x_max_d(x_max.getValue());
	    double y_min_d(y_min.getValue()), y_max_d(y_max.getValue());
	    min = Complex(x_min_d, y_min_d);
	    max = Complex(x_max_d, y_max_d);
	  }
	
	  Box *b = new Box(min, max);
	  Box *b_copy = new Box(min, max);

  // start timing code.
	  struct timeval start;
	  struct timeval end;
	  gettimeofday(&start, NULL);
	  // end timing code.

  Predicates p(a);
  Algorithm algo(p, b, min_box_size_d, !no_use_inclusion.getValue(), display.getValue());
  algo.Run();

  if (no_use_inclusion.getValue()) {
    //    cerr << endl << endl << "HERE1" << endl << endl;
    algo.AttemptIsolation();
  }

  // start timing code.
  gettimeofday(&end, NULL);
  cout << ",time=" <<
      (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);


  if (display.getValue()) {
    cout << "Operated on Bounding box : " << min << "," << max << endl;
    cout << "With polynomial : " << a.toString() << endl; 

    //a.dump(); 	// a.toString() is much better than a.dump().
    cout << endl;
    cout << "--------------------------" << endl;
    cout << "Number of roots:" << algo.output()->size() << endl;
    list<const Disk *>::const_iterator it = algo.output()->begin();
    while (it != algo.output()->end()) {
      cout << "m= " << (*it)->centre << ", r= " << (*it)->radius << endl;
      ++it;
    }

    display_funcs::SetDisplayParams(b_copy, algo.reject(), algo.output_boxes(),
        algo.ambiguous());

    startGlutLoop(argc, argv);
  } else {
    cout << ",output=" << algo.output()->size() << endl;
  }
}
