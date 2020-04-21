/* this program is used to test the relative precision of IEEE double.

            x = 1.0000000000...0
   +  epsilon = 0.0000000000...01 (52-bits)
   ------------------------------
              = 1.00000000000000

   The program will print out "52" as result.
 */

#include "CORE.h"
#include <iostream>
#include <cmath>
#include <CORE/BigFloat.h>

using namespace CORE;
using namespace std;

int main(int argc, char** argv) {

  prec_t prec = 10;
  if (argc>1) prec=atoi(argv[1]);

  BigFloat x;
  x.set_prec(prec);
  x.set(1);
  
  BigFloat y;
  y.set_prec(prec);

  BigFloat epsilon;

  for (prec_t i=prec-10; i<prec+10; i++) {
    epsilon.set_prec(i);
    epsilon.set_2exp(1, -i);
    y.add(x, epsilon);
    cout << "i=" << i <<  ":   1+exp2(-i) = " << y << endl;
    if (y.cmp(x) == 0) {
      cout << "(i-1)=" << (i-1) << endl;
      break;
    }
  }
  return 0;
}
