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
    
    cout << "tan(sqrt(2)) + arctan(sin(1))=" 
        << tan(NT(2).sqrt(bprec+4).makeExact(), bprec+2) + arctan(sin(NT(1), bprec+4), bprec+2) << endl;
    
    return 0;
}

