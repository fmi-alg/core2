#include "agm.h"
#include <CORE/Expr.h>

using namespace CORE;
using namespace std;

double lg2(double x) {
    return log(x)/log(2.0);
}

int main(int args, char** argv) {
	int N = 3;		// precision in digits
	int prec = 1000;   // precision in digits
    
    if (args > 1) {
		N = atoi(argv[1]);
        prec = (int)pow(double(10.0), N);
    }
    
    if (args > 2)
        prec = atoi(argv[2]);

    cout << setprecision(prec);

    int bprec = digit2bit(prec);

    int n = 10*prec;
    int nn = n * n;
    cout << "n=" << n << endl;

    /* exact version
    Expr e(0);
    for (int k=n; k<=nn; k++) {
        e += 1 / Expr(k); 
    }

    e.approx(CORE_INFTY, bprec); 
    */
    BigFloat e(0);
    int p = (int)lg2(nn - n);
    
    for (int k=n; k<=nn; k++) {
        e += BigFloat(1).div(k, bprec+p).makeExact(); 
    }

    cout << "e=" << e << endl;
    
    return 0;
    
}

