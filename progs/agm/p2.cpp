#include "agm.h"
#include <CORE/Timer.h>

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

    int bprec = digit2bit(prec);
    
	Timer timer;

	timer.start();
	NT e = exp(NT(1.0), bprec+3).div(pi(bprec+3), bprec+3).sqrt(bprec+1);
	timer.stop();

	cout << "t=" << timer.getSeconds() << endl;
    cout << "sqrt(e/pi)=" << setprecision(10) << e << endl;

	/*
    cout << setprecision(prec);
    cout << "sqrt(e/pi)=" 
        << exp(NT(1.0), bprec+3).div(pi(bprec+3), bprec+3).sqrt(bprec+1) << endl;
	*/
    
    return 0;
}

