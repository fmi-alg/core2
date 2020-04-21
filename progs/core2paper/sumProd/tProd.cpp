/*************************************************** 
 * file: tProd.cpp
 *
 * Suppose you have a function term(n) that takes integer
 * arguments.  You want to compute the product, which is
 * the expression
 *
 * 	e = term(s) * term(s+1) * ... * term(t-1) * term(t)
 *
 * for some s <= t.   
 *
 * We define here a function called prod(...) that
 * will construct this expression in the obvious way:
 *
 *      Expr e = prod(term, s, t);
 *
 * But we have a builtin operator calles "product" that
 * can be called just like "prod":
 *
 *      Expr e = product(term, s, t);
 *
 * When you compile this program with the flag -D_USE_PROD,
 * we will use the builtin operator "product" for product.
 * Otherwise, we use the straight forward construction of
 * the product.   The built-in operator is more efficient.
 *
 * In this test, we show the advantages of using the builtin
 * product over the straightforward "prod".  In the Makefile,
 * we compile this program using builtin (tProd1)
 *
 * The example in this test:
 * ========================
 *
 * Let us we define term(i) to be
 *
 * 	             4*i*i
 * 	term(i) =  --------------
 * 	            4*i*i - 1
 * 
 *
 * Then the infinite product
 *
 * 	product(term, 1, \infty )
 *
 * is an approximation to Pi/2.    
 *
 * This program will compute any prefix of this infinite
 * product, giving approximations to Pi/2.
 *
 *
 * --Jihun, Chee (May 2010)
 * Since Core 2.0
 * 	$Id: tProd.cpp,v 1.3 2010/06/25 10:15:49 exact Exp $
 *************************************************** */
#define CORE_LEVEL_4
#include <vector>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

using namespace std;

#ifdef _USE_PROD

// Use the builtin operator called "product" in Core 2.0:

#else

// Here we define the simple do-it-yourself version 
// of product, called "prod":
//
template <typename Function, typename T>
Expr prod(Function fun, T start, T end) {
  Expr p(1);
  for(T it = start; it <= end; ++it)
    p *= fun(it);
  return p;
}

#endif

Expr term(int i) {
  int num = 4*i*i;
  int den = num - 1;
  return Expr(num)/Expr(den);
}

int main(int argc, char** argv) {
  int num = 100000;
  if (argc >= 2)
    num = atoi(argv[1]);

  if(num < 2)
	cerr << "ERROR!!! Input argument should be greater than 1." << endl;

  cout << setprecision(60);
	
  Timer2 timer;
  BigFloat r;

  timer.start();

#ifdef _USE_PROD
  Expr e = product(term, 1, num);
#else
  Expr e = prod(term, 1, num);
#endif

  r = e.approx(60, CORE_INFTY);
  timer.stop();

  cout << "r=" << r << endl;
  cout << "it took " << timer.get_mseconds()<< " mseconds." << endl;

  return 0;
}
