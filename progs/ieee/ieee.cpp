#include <iostream>
#include <cmath>
#include <ieeefp.h>

using namespace std;

int main(void) {

#ifdef ___IEEE_BIG_ENDIAN
	cout << "Big Endian System!" << endl;
#endif

#ifdef ___IEEE_LITTLE_ENDIAN
	cout << "Little Endian System!" << endl;
#endif

  double db1 = 1.0;
  double db2 = 0.0;
  fpsetsticky(FP_X_DZ);
  double db3 = db1 / db2;
  cout << "db3=" << db3 << endl;
  cout << "exception: " << fpgetmask() << endl;
  cout << "FP_X_DX=" << FP_X_DZ << endl;

  return 0;
}

