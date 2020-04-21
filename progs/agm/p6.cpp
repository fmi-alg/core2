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
    
    cout << "arctan(1 - arctan(1 - arctan(1 - arctan(1/pi))))=" 
        << arctan(1.0 - arctan(1.0 - arctan(1.0 - arctan(NT(1.0).div(pi(bprec+5), bprec+5), bprec+4), bprec+3), bprec+2), bprec+1) << endl;
    
    return 0;
}

