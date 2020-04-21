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
    
    /*
    BigInt p;
    power(p, BigInt(6), (int)pow(6.0, 6));

    cout << "6^{6^6}=" << p << endl;
    cout << "sin(6^{6^6})=" << sin(NT(p), bprec) << endl;
    */
    cout << "sin(6^{6^6})=" 
        << sin(pow(NT(6.0), (int)pow(6.0, 6)), bprec) << endl;
    
    return 0;
}

