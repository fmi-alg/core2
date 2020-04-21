#include "agm.h"
#include <CORE/Expr.h>

using namespace CORE;
using namespace std;

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
   cout << "bprec=" << bprec << endl;
    
    Expr e(0);
    BigInt x;
    
    // 1/(2n+1)^2 < 2^{-(bprec+1)}
    // 2n+1 > 2^{(bprec+1)/2}
    // n > 2^{bprec/2}
    BigInt n = BigInt(1) << (bprec >> 1);
    cout << "n=" << n << endl;

    for (BigInt i=0; i<=n; i++) {
        x = BigInt(2*i+1);
        
        if (i % 2 == 0) 
            e += Expr(1) / (x*x);
        else
            e -= Expr(1) / (x*x);
    }

    e.approx(CORE_INFTY, bprec);   
    cout << "e=" << e << endl;
    
    return 0;
}

