/* **************************************
   **************************************/
#define CORE_LEVEL 3 
#include "CORE/CORE.h"
using namespace CORE;
using namespace std;

//typedef BigInt  NT;
//typedef BigFloat  NT;
typedef BigRat  NT;
typedef Expr    CoNT;
//typedef BigInt CoNT;
int main(int argc , char ** argv) {

  // Set the polynomial:
  // p(x) = x3 + 1
  // Compute its roots.
  Polynomial<NT> p("2/3 x^3 + 1.1");
  Sturm<NT> s(p);
  BFInterval I(-2, 2);

  cout << "poly=" << p << endl;

  /*
s.dump("Sturm sequence");

  const int n_roots = s.numberOfRoots();
  std::vector<CoNT>          roots (n_roots);
  int                        i;

  cout << "Number of roots are " << n_roots << endl;
  std::cout << "The roots are:" << std::endl;
  for (i = 0; i < n_roots; i++)
  {
    roots[i] = rootOf(p, i + 1);
    std::cout << "    " << i + 1 << ")  " << roots[i] << std::endl;
  }

  BFInterval J = s.newtonRefine(I,200);
  cout << setprecision(600);
  cout << "J = " << J.first << ", " << J.second << endl;
  if (Expr(J.first) == Expr(J.second)) cout << "Equal!" << endl;
  else cout << "Not equal!" << endl;
  */
}	


// ==================================================
// End of tPoly
// ==================================================
