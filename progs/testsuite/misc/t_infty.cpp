// construct IEEE double infty

#include <iostream>
#include <cmath>
#include <cfloat>

const double DBL_INFTY= ::ldexp(DBL_MAX, 1);

int main() {
  double x = 2.2;
  double y = ldexp(x, 20);
  std::cout << "y = " << y << std::endl;
  std::cout << "DBL_INFTY = " << DBL_INFTY<< std::endl;
  return 0;
}

