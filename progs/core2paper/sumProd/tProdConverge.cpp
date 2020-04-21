/*************************************************** 
 * file: tProdConverge.cpp
 *
 * In the program "tProd.cpp" we compute arbitrary prefixes of
 * an infinite product formula for pi/2.
 *
 * The current program does a self-validation to indicate that
 * the formula is indeed converging towards pi/2.
 * 
 * Again, if you use the compiler flag -D_USE_PROD,
 * then we will compile the program using the built-in anary
 * operator "product", resulting is a more efficient program.
 *
 * --Jihun (June 2010)
 * Since Core 2.0
 * 	$Id: tProdConverge.cpp,v 1.3 2010/06/25 10:15:49 exact Exp $
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
  int num = 10000;
  if (argc >= 2)
    num = atoi(argv[1]);

  if(num < 500)
	cerr << "ERROR!!! Input argument should be greater than 500." << endl;

  Expr lastErr = 1;
  Expr CurrErr;
  Expr halfPi = pi() / 2;

  for(int i = 100; i < num; i *=2) {
 #ifdef _USE_PROD
    Expr e = product(term, 1, i);
#else
    Expr e = prod(term, 1, i);
#endif
    CurrErr = abs(e-halfPi);

    if(CurrErr > lastErr)
      cout << "ERROR!!! tProd does not converge to PI /2 for n = " << i << endl;
  
    lastErr = CurrErr;

    cout << "CORRECT!!! tProd converges to PI / 2 for n = " << i << endl;
  }

  return 0;
}
