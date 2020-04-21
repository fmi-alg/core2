#include "agm.h"

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
    
	NT ee = exp(NT(2), bprec+4);
    cout << "arcsin(1/e^2) + arcsin(e^2)=" 
        << arcsin(NT(1.0).div(ee, bprec+4), bprec+1) + arcsin(ee, bprec+1) << endl;
    
    return 0;
}

