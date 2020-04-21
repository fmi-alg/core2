/*****************************************************************************
 *
 *  Discrete Newton Iteration
 *
 *      verify-digits   [prec]  [loops]
 *
 ****************************************************************************/

#include "agm.h"

using namespace CORE;
using namespace std;

// check routine
void check(int prec, eval_fun f) {
	int bprec = digit2bit(prec);

    NT h;
    NT f1, f2, diff;
    NT old_ff, new_ff;
    
    cout << setprecision(prec);
    for (NT x = NT(0.1); x < NT(0.8); x += NT(0.1)) {
        cout << "x=" << x << endl;
        old_ff = 0.1;
        for (int n=2; n<bprec; n++) {
            h = NT::exp2(-n);
            cout << "h=" << h << ",n=" << n << endl;
            f2 = f(x+h, bprec);
            //cout << "f(x+h)=" << f2 << endl;
            f1 = f(x, bprec);
            //cout << "f(x)=" << f1 << endl;
            diff = f2 - f1;
            //cout << "f(x+h)-f(x)=" << diff << endl;
            
            cout << "f = " << f1 << endl;
            cout << "fh = " << f2 << endl;
            cout << "ff = " << old_ff << endl;
            
            cout << "f/f' = " << f1.div(old_ff, bprec) << endl;
            
            new_ff = diff.div(h, bprec);
            cout << "msb = " << (new_ff - old_ff).uMSB() << endl;
            old_ff = new_ff;            
        }
    }
}

int main(int args, char** argv) {
	int prec = 100;		// precision in digits
	int loops = 5;     // loops for self-check
    
    if (args > 1)
		prec = atoi(argv[1]);
    if (args > 2)
        loops = atoi(argv[2]);

    // verify pi()
    check(prec, eval_T);

	return 0;
}
