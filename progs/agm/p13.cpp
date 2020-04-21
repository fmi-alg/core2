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
    
    Expr e(0.5);
    Expr c("3.999");
    
    for (int i=0; i<prec; i++) {
        e = c * e * (1.0 - e);
    }

    e.approx(CORE_INFTY, bprec);   
    cout << "=" << e << endl;
    
    return 0;
}

