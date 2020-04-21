#include "agm.h"
#include <CORE/Timer.h>

using namespace CORE;
using namespace std;

int main(int args, char** argv) {
	int N = 2;		// precision in digits
	int prec = 100;   // precision in digits
    
    if (args > 1) {
		N = atoi(argv[1]);
        prec = (int)pow(double(10.0), N);
    }
    
    if (args > 2)
        prec = atoi(argv[2]);

    int bprec = digit2bit(prec);

	Timer timer;

	timer.start();
	NT e = sin(tan(cos(NT(1.0), bprec+4), bprec+2), bprec+1);
	timer.stop();
	
	cout << "t=" << timer.getSeconds() << endl;
    cout << "sin(tan(cos(1.0)))=" << setprecision(10) << e << endl;

	/*
    cout << setprecision(prec);
    cout << "sin(tan(cos(1.0)))="
    	 << sin(tan(cos(NT(1.0), bprec+4), bprec+2), bprec+1) << endl;
    */
	
    return 0;
}

