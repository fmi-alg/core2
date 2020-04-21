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
    
    cout << "e^{e^{sqrt(e)}}=" 
        << exp(exp(exp(NT(1.0), bprec+8).sqrt(bprec+8).makeExact(), bprec+5), bprec+1) << endl;
    
    return 0;
}

