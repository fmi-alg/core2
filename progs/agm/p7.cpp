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
    
    cout << "pi^{1000}=" 
    //    << pow(pi(bprec+1660), 1000) << endl;
        << exp(1000*log(pi(bprec+2000), bprec+2000), bprec+1) << endl;

    
    return 0;
}

