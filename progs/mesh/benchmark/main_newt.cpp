
//#ifndef CORE_LEVEL
// #define CORE_LEVEL 1
//#endif

#include "tclap/CmdLine.h"
#include <sys/time.h>

#include "../krawczyk/krawczyk-defs.h"

//typedef IntervalT<DoubleWrapper> Interval;
//typedef BoxT<DoubleWrapper> Box;
//#define double DoubleWrapper

#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"
#include "CORE/linearAlgebraT.h"


// Krawczyk
#include "../krawczyk/krawczyk2D-inl.h"
#include "../krawczyk/newton2D-inl.h"
#include "../krawczyk/hs2D-inl.h"
#include "../krawczyk/algorithm-inl.h"

#include "../krawczyk/tmp-display.h"
#include "../krawczyk/step-2.h"
//#include "benchmark.h"

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

TCLAP::ValueArg<string> x_min("x", "x_min", "Minimum x range", false, "-2", "string");
TCLAP::ValueArg<string> x_max("z", "x_max", "Maximum x range", false, "2", "string");
TCLAP::ValueArg<string> y_min("y", "y_min", "Minimum y range", false, "-2", "string");
TCLAP::ValueArg<string> y_max("u", "y_max", "Maximum y range", false, "2", "string");
TCLAP::ValueArg<string> poly("p", "poly", "Input polynomial file name", false, "", "string");
TCLAP::ValueArg<string> min_box_size(
    "m", "min_box_size", "Minimum subdivision size", false, "0.001", "string");
TCLAP::ValueArg<string> max_box_size(
    "l", "max_box_size", "Maximum subdivision size", false, "0.1", "string");
TCLAP::ValueArg<int> cthresh("t", "cr_threshold",
    "Threshold for recalculating the centered form", false, 0, "int");
TCLAP::ValueArg<int> max_gen("g", "max_gen_id",
    "Threshold for recalculating the centered form", false, 20, "int");
TCLAP::ValueArg<int> core_level("c", "core_level", "CORE LEVEL", false, 1, "int");
TCLAP::ValueArg<unsigned int> rand_degree("a", "degree", "Degree of random poly", false, 10, "unsigned int");
TCLAP::ValueArg<unsigned int> rand_seed("f", "seed", "Seed", false, 10, "unsigned int");


TCLAP::SwitchArg use_rb("r", "use_root_bounds", "Use Cauchy Bounds", false);
TCLAP::SwitchArg display("d", "display", "Display subdivision tree", false);
TCLAP::SwitchArg use_newton("n", "newton", "Use newton iteration", false);
TCLAP::SwitchArg use_krawczyk("k", "krawczyk", "Use the krawczyk operator", false);
TCLAP::SwitchArg use_hs("s", "hansen", "Use the hansen sengupta operator", false);
TCLAP::SwitchArg step_2("e", "step_2", "Disambiguate boxes", false);
TCLAP::SwitchArg random_poly("b", "random", "Use a random polynomial", false);

template <typename NT>
void MainT(int argc, char **argv);

int main(int argc, char **argv) {

  try {
     TCLAP::CmdLine cmd("Newton type methods", ' ', "0.1");
     cmd.add(poly);
     cmd.add(x_min);
     cmd.add(x_max);
     cmd.add(y_min);
     cmd.add(y_max);
     cmd.add(use_rb);
     cmd.add(display);
     cmd.add(use_newton);
     cmd.add(use_krawczyk);
     cmd.add(use_hs);
     cmd.add(min_box_size);
     cmd.add(max_box_size);
     cmd.add(cthresh);
     cmd.add(max_gen);
     cmd.add(core_level);
     cmd.add(step_2);
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

   if (core_level.getValue() == 1) {
     MainT<DoubleWrapper>(argc, argv);
   } else if (core_level.getValue() == 2) {
     MainT<BigFloat>(argc, argv);
   } else if (core_level.getValue() == 3) {
     // MainT<Expr>(argc, argv);
   }

   return 0;
}


namespace benchmark {
void GetBiPoly(const char *filename,
               BiPoly<BigFloat> *u,
               BiPoly<BigFloat> *v);
void GetBiPoly(const char *filename,
               BiPoly<DoubleWrapper> *u,
               BiPoly<DoubleWrapper> *v);
void GetBiPoly(const char *filename,
               BiPoly<Expr> *u,
               BiPoly<Expr> *v);
void GenerateRandom(BiPoly<DoubleWrapper> *u,
                    BiPoly<DoubleWrapper> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
void GenerateRandom(BiPoly<BigFloat> *u,
                    BiPoly<BigFloat> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
void GenerateRandom(BiPoly<Expr> *u,
                    BiPoly<Expr> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
void GetRootBounds(const Polynomial<BigFloat> &poly,
                   BigFloat *lb,
                   BigFloat *ub);
void GetRootBounds(const Polynomial<DoubleWrapper> &poly,
                   DoubleWrapper *lb,
                   DoubleWrapper *ub);
void GetRootBounds(const Polynomial<Expr> &poly,
                   Expr *lb,
                   Expr *ub);
void GetPoly(const char *file_name,
             Polynomial<BigFloat> *d);
void GetPoly(const char *file_name,
             Polynomial<DoubleWrapper> *d);
void GetPoly(const char *file_name,
             Polynomial<Expr> *d);
void GenerateRandom(Polynomial<DoubleWrapper> *u,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
void GenerateRandom(Polynomial<BigFloat> *u,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
void GenerateRandom(Polynomial<Expr> *u,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    unsigned int seed);
}

template <typename NT>
void MainT(int argc, char **argv) {
  typedef IntervalT<NT> Interval;
  typedef BoxT<NT> Box;

  NT x_min_d(x_min.getValue()),
      x_max_d(x_max.getValue()), y_min_d(y_min.getValue()),
      y_max_d(y_max.getValue());

  const NT min_size(min_box_size.getValue());
  const NT max_size(max_box_size.getValue());

  BiPoly<NT> fxy;
  BiPoly<NT> gxy;

  if (random_poly.getValue()) {
    benchmark::GenerateRandom(&fxy, &gxy, rand_degree.getValue(), 10, rand_seed.getValue());
    if (use_rb.getValue()) {
      Polynomial<NT> poly;
      benchmark::GenerateRandom(&poly, rand_degree.getValue(), 10, rand_seed.getValue());
      benchmark::GetRootBounds(poly, &x_min_d, &x_max_d);
      y_min_d = x_min_d;
      y_max_d = x_max_d;
    }
  } else {
    benchmark::GetBiPoly(poly.getValue().c_str(), &fxy, &gxy);
    if (use_rb.getValue()) {
      Polynomial<NT> poly_m;
      benchmark::GetPoly(poly.getValue().c_str(), &poly_m);
      benchmark::GetRootBounds(poly_m, &x_min_d, &x_max_d);
      y_min_d = x_min_d;
      y_max_d = x_max_d;
    }
  }


  // Input box
  Interval x_range(x_min_d, x_max_d);
  Interval y_range(y_min_d, y_max_d);

  // start timing code.
  struct timeval start;
  struct timeval end;
  gettimeofday(&start, NULL);
  // end timing code.

  Box *B0 = new Box(0, Interval(x_range), Interval(y_range));

  // Output types.
  std::vector<const Box *> output;
  std::vector<const Box *> ambiguous;
  std::vector<const Box *> exclude;

  std::vector<const Box *> *exclude_ptr = NULL;
  if (display.getValue()) {
    exclude_ptr = &exclude;
  }

  BasePredicate<NT> *pred = NULL;

  step2::operator_type op_type = step2::KRAWCZYK;
  if (use_krawczyk.getValue()) {
    pred = new Krawczyk2D<NT>(fxy, gxy, max_size, min_size, max_gen.getValue(),
        cthresh.getValue());
  } else if (use_newton.getValue()) {
    pred = new Newton2D<NT>(fxy, gxy, max_size, min_size, max_gen.getValue(),
        cthresh.getValue());
    op_type = step2::NEWTON;
  } else if (use_hs.getValue()) {
    pred = new Hs2D<NT>(fxy, gxy, max_size, min_size, max_gen.getValue(),
        cthresh.getValue());
    op_type = step2::HANSEN;
  } else {
    cout << "Please specify one of -n (newton) -s (hansen-sengupta) or -k (krawczyk)" << endl;
    return;
  }

 Algorithm::Run<NT>(
      *pred, B0, &output, exclude_ptr, &ambiguous);

 if (step_2.getValue()) {
   step2::Process(&ambiguous, &output, fxy, gxy,
       min_size, max_size, op_type);
 }

  // start timing code.
  gettimeofday(&end, NULL);
  cout << ",time=" <<
      (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000 << endl;

  if (display.getValue()) {
    cout << endl << "Operating over : " << x_range << "," << y_range << endl;
    cout << "With polynomials : " << endl;
    cout << endl;
    fxy.dump();
    cout << endl;
    gxy.dump();

    cout << endl << "Output regions : " << endl;
    for (unsigned int i = 0 ; i < output.size(); ++i) {
      const Box *interval = output[i];
      cout << "X : " << interval->x_range << ", Y : " << interval->y_range << endl;
    }

    cout << endl << "Unresolved regions : " << endl;

    if (ambiguous.size() > 100) {
      cout << "Omitted due to the large number of unresolved regions" << endl;
    } else {
      for (unsigned int i = 0 ; i < ambiguous.size(); ++i) {
        const Box *interval = ambiguous[i];
        cout << "X : " << interval->x_range << ", Y : " << interval->y_range << endl;
      }
    }

    Box *display = new Box(0, Interval(x_range), Interval(y_range));
    display_funcs::SetDisplayParams(display, &exclude, &output, &ambiguous);
    startGlutLoop(argc, argv);
    delete display;
  } else {
    //cout << "Num roots : " << output.size() << endl;
  }
}
