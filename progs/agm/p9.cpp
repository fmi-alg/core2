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
    
    cout << "sin(10*arctan(tan(pi*sqrt(2011)/3)))=" 
        << sin(10*arctan(tan(pi(bprec+16)*NT(2011).sqrt(bprec+12).div(3.0, bprec+9), bprec+9), bprec+6), bprec+1) << endl;
    
    return 0;
}

