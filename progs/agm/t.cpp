#include "agm.h"

#include <CORE/CORE.h>

using namespace CORE;
using namespace std;

typedef unsigned long ulong;

NT eval_cube(const NT& m, int prec) {
  return m*m*m;	
}

NT comp_cube(const NT& c, int prec) {
  NT m;
  m = newton(eval_cube, c, NT(1.1), prec);
  return m;
}

int main(int args, char** argv) {
	CORE::setFpFilterFlag(false);

	int prec = 100;		// precision in digits
	if (args>1)
		prec = atoi(argv[1]);

	cout << setprecision(prec);

	int bprec = (int)(prec*3.3219281);	// convert to bits

	//cout << "e^{1.25}=" << prim_exp(BigFloat(1.25), bprec) << endl;
	//cout << "log{4}=" << prim_log(BigFloat(4.0), bprec) << endl;
	
	cout << "cube(3)=" << comp_cube(3, bprec) << endl;

	return 0;
}

