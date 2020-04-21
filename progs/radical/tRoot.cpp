#include <iostream>

#define CORE_LEVEL 3
#include "CORE.h"
using namespace std;


int main(int argc, char** argv)
{
  cout << "test begins" << endl;
  int N=10;
  if (argc>1) N = atoi(argv[1]);

  int X=2;
  if (argc>2) X = atoi(argv[2]);

  cout.precision(40);
  for (int k=1; k<N; k++) {
    int expo = k;
    Expr r = root(pow(Expr(X),expo) + 1, expo);
    Expr rr = root(pow(Expr(X),expo) - 1, expo);
    Expr rrr = root(pow(Expr(X),expo), expo);
    if (r <= X) {
      cout << "ERROR!!! (2^k + 1) ^ (1/k) is not greater than 2" << endl;
    }
    if (rr >= X) {
      cout << "ERROR!!! (2^k - 1) ^ (1/k) is not less than 2" << endl;
    }
    if (rrr !=X) {
      cout << "ERROR!!! (2^k - 1) ^ (1/k) is not equal to 2" << endl;
    }
  }
  cout << "test ends" << endl;
  return 0;
}
