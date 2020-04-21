/* this program is used to test the relative precision of IEEE double.

            x = 1.0000000000...0
   +  epsilon = 0.0000000000...01 (52-bits)
   ------------------------------
              = 1.00000000000000

   The program will print out "52" as result.
 */

#include "CORE/CORE.h"
#include <iostream>
#include <cmath>

using namespace std;

int main() {
  double x = 1;
  for (int i=0; i<100; i++) {
    double epsilon = ::ldexp(1.0, -i);
    if (x + epsilon == x) {
      cout << "(i-1)=" << (i-1) << endl;
      break;
    }
  }
  return 0;
}
