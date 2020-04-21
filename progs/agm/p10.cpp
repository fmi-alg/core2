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
    
    Expr e;
    e = sqrt(Expr(7) + radical(Expr(2), 5) - 5*radical(Expr(8), 5)) + radical(Expr(4), 5) - radical(Expr(2), 4);
    e.approx(CORE_INFTY, bprec);
    
    cout << "=" << e << endl;
    
    return 0;
}

