#include "agm.h"

#include <CORE/Expr.h>

using namespace CORE;
using namespace std;

double cuberoot(double x) {
    return exp(log(x)/3.0);
}

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
    
    // (1/7)^{n^3} < 2^{-(bprec+1)}
    // n^3 > (bprec+1)/log_2(7)
    int n = (int)cuberoot((bprec+1)/lg2(7.0));
    cout << "n=" << n << endl;

    Expr e(0);
    Expr x("1/7");

    for (int i=0; i<=n; i++) {
        e += pow(x, i*i*i+1);
    }

    e.approx(CORE_INFTY, bprec+1);   

    /* BF version
    BigFloat e(0);
    int p = (int)lg2(n);
    
    for (int i=n; i<=n; i++) {
	int x = i*i*i + 1;
        e += pow(BigFloat(1).div(BigFloat(7), bprec+(int)lg2(x)).makeExact(), x+p);
    }
    */

    cout << "e=" << e << endl;
    
    return 0;
}

