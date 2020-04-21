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
    
    Expr a0("14/3");
    Expr a1("5");
    Expr a2("184/35");

    Expr e;
    
    for (int i=0; i<prec; i++) {
        e = Expr(114) - (Expr(1463) - (Expr(6390) - Expr(9000) / a0) / a1) / a2;
        a0 = a1;
        a1 = a2;
        a2 = e; 
    }

    e.approx(CORE_INFTY, bprec);   
    cout << "=" << e << endl;
    
    return 0;
}

