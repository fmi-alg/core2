#include <iostream>
#include <string>
#include <sys/time.h>

#include "krawczyk-defs.h"

// Krawczyk
#include "krawczyk2D-inl.h"
// Newton
#include "newton2D-inl.h"
// Gauss - Siedel
#include "hs2D-inl.h"

#include "algorithm-inl.h"

#include "tmp-display.h"

using namespace std;

void startGlutLoop(int argc, char **argv) ;

typedef DoubleWrapper RunNT;
typedef IntervalT<RunNT> Interval;
typedef BoxT<RunNT> Box;

int main(int argc, char* argv[]) {
  RunNT x_min = 0.7, x_max = 0.9, y_min = 0.5, y_max = 0.7;

  string fxy_str = "x^2 + y^2 - 1";  // default f(x, y)
  string gxy_str = "x^2 - y";

  RunNT min_size = 0.0001;
  RunNT max_size = 0.0001;

  BiPoly<RunNT> fxy;
  BiPoly<RunNT> gxy;

  char algorithm = 'H';

  // Process command line arguments.
  if (argc == 3) {
    fxy_str = argv[1];
    gxy_str = argv[2];
  } else if (argc >= 7) {
    fxy_str = argv[1];
    gxy_str = argv[2];
    x_min = argv[3];
    x_max = argv[4];
    y_min = argv[5];
    y_max = argv[6];
    // If there are 8 arguments, then process the min_size
    // as well, else use a default.
    if (argc > 7) {
      min_size = argv[7];
    }
    // If there are 9 arguments, process the max_size as well.
    if (argc > 8) {
      max_size = argv[8];
    }

    if (argc > 9) {
      algorithm = argv[9][0];
    }
  } else if (argc != 1) {
    cout << "Synopsis: to find the zeroes of f(x + iy) = f(x, y) + ig(x, y)" << endl;
    cout << "./krawczyk \"f(x, y)\" \"g(x, y)\" [Xmin] [Xmax] [Ymin] [Ymax] [min-size] [max_size]"<< endl;
    return 1;
  }

  // Parse the bivariate polynomials for each of the
  // functions.
  fxy = fxy.getbipoly(fxy_str);
  gxy = gxy.getbipoly(gxy_str);

  Interval x_range(x_min, x_max);
  Interval y_range(y_min, y_max);


  fxy.dump("f(x, y) :");
  cout << endl;
  gxy.dump("g(x, y) :");
  cout << endl;

  /*
  cout << "Coords : " << x_min << ","
       << x_max << "," << y_min << "," << y_max << endl;
  cout << "Min size : " << min_size << endl;
  */

  // start timing code.
  struct timeval start;
  struct timeval end;
  gettimeofday(&start, NULL);
  // end timing code.

  Box *B0 = new Box(0, x_range, y_range);

  std::vector<const Box *> output;
  std::vector<const Box *> ambiguous;
  std::vector<const Box *> exclude;


  Krawczyk2D<RunNT> k_pred(fxy, gxy, max_size, min_size, 15);
  Newton2D<RunNT> n_pred(fxy, gxy, max_size, min_size, 15);
  Hs2D<RunNT> h_pred(fxy, gxy, max_size, min_size, 15);

  BasePredicate<RunNT> *pred = NULL;
  if (algorithm == 'H') {
    pred = new Hs2D<RunNT>(fxy, gxy, max_size, min_size, 15);
  } else if (algorithm == 'N') {
    pred = new Newton2D<RunNT>(fxy, gxy, max_size, min_size, 15);
  } else if (algorithm == 'K') {
    pred = new Krawczyk2D<RunNT>(fxy, gxy, max_size, min_size, 15);
  } else {
    cout << "Unknown algorithm type : " << algorithm << " (options are N/H/K)" << endl;
    exit(1);
  }

  Algorithm::Run<RunNT>(
      *pred, B0, &output, &exclude, &ambiguous);

  // start timing code.
  gettimeofday(&end, NULL);
  cerr << "The total time taken was = " <<
       (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec) << " micro seconds" << endl;
  // end timing code.

  cout << endl << "Output regions : " << endl;
  for (unsigned int i = 0 ; i < output.size(); ++i) {
    const Box *interval = output[i];
    cout << "X : " << interval->x_range << ", Y : " << interval->y_range << endl;
  }

  cout << endl << "Ambiguous regions size : " << ambiguous.size() <<  endl;
  Box *B_display= new Box(0, x_range, y_range);

  display_funcs::SetDisplayParams(B_display, &exclude, &output, &ambiguous);

  for (unsigned int i = 0 ; i < ambiguous.size(); ++i) {
    const Box *interval = ambiguous[i];
    cout << "X : " << interval->x_range << ", Y : " << interval->y_range << endl;
  }


  startGlutLoop(argc, argv);
  delete B_display;

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
  glutCreateWindow("CXY       (Press ESC to exit)");
  display_funcs::ClearBackground();
  glutReshapeFunc(display_funcs::ReshapeHandler);
  glutDisplayFunc(display_funcs::DisplayHandler);
  glutKeyboardFunc(display_funcs::KeyHandler);
  glutMainLoop();
}
