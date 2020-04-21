/*************************************************** 
 * file: tHarmonic.cpp
 *
 * Suppose you have a function term(n) that takes integer
 * arguments.  You want to compute the sum, which is
 * the expression
 *
 * 	e = term(s) + term(s+1) + ... + term(t-1) + term(t)
 *
 * 	  = \sum_{i=s}^t term(i)
 *
 * for some s <= t.   
 *
 * In this file, we define a Core library function called sum(...) that
 * will construct this expression in the obvious way:
 *
 *      Expr e = sum(term, s, t);
 *
 * But we also have a builtin operator called "summation" that
 * can be called just like "sum":
 *
 *      Expr e = summation(term, s, t);
 *
 * When you compile this file (tSum.cpp) with the compiler flag "-D_USE_SUM",
 * it will compile the expression e using builtin summation,
 * otherwise, it will compile the expression e using "sum".
 *
 * Our goal is to compare the timing for these two methods of computing e.
 * Timing tests show the improvement of summation
 * over the use of "sum".
 *
 * --Zilin, Jihun, Chee (May 2010)
 * Since Core 2.0
 * 	$Id: tHarmonic.cpp,v 1.2 2010/06/25 10:15:49 exact Exp $
 *************************************************** */
#define CORE_LEVEL_4
#include <vector>
#include <CORE/CORE.h>
#include <CORE/Timer.h>
using namespace std;

#ifdef _USE_SUM

// This is the definition of the builtin operator
// called "summation" in Core 2.0:
//
// template <typename Function, typename T>
// Expr summation(Function fun, T start, T end) {
//   vector<ExprRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>*> c;
//   SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>* rep
//     = new SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>(c);
//   for (T it = start; it <= end; ++it) {
//     rep->insert(fun(it).rep());
//   }
// 
//   return Expr(rep);
// }

#else

// Here we define the simple do-it-yourself version 
// of summation, called "sum":
//
template <typename Function, typename T>
Expr sum(Function fun, T start, T end) {
  Expr sum(0);
  for(T it = start; it <= end; ++it)
    sum += fun(it);
  return sum;
}

#endif

Expr term(int i) {
  return Expr(1)/Expr(i);
}

int main(int argc, char** argv) {
  int num = 100000;
  if (argc >= 2)
    num = atoi(argv[1]);

  if(num < 2)
	cerr << "ERROR!!! Input argument should be greater than 1." << endl;
 
  Timer2 timer;
  //BigFloat r;

  timer.start();

// The two methods to compute the expression e:
//
#ifdef _USE_SUM
  Expr e = summation(term, 1, num);
#else
  Expr e = sum(term, 1, num);
#endif

  set_def_output_and_computing_digits(60);// relative 60 digits

  //r = e.approx(); // same as r=e.approx(200, CORE_INFTY); since 60 digits=200bits
  timer.stop();


  cout << "log(n) = " << log(num) << endl;
  cout << "sum of i=1 to n of 1/i =" << e << endl;
  cout << "it took " << timer.get_mseconds()<< " msec." << endl;

  return 0;
}
