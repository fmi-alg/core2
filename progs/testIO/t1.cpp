//====================================== 
// 
//
#define CORE_LEVEL 4
#include "CORE.h"
using namespace std;

int main(){
cout << "Example from \
  `Precision Arithmetic: A New Floating-Point Arithmetic' by \
  Chengpu Wang" << endl;
cout << "Let ab - cd = 64919121 x 205117922 – 159018721 x 83739041 \n"
	<< " = 13316075197586562 - 13316075197586561 \n"
	<< " = 1" << endl;
BigInt a(64919121), b(205117922);
double aa(64919121), bb(205117922);
BigInt c(159018721), d(83739041);
double cc(159018721), dd(83739041);
if ((a*b == BigInt("13316075197586562")) && 
// NOTE: 133...62" is too large for machine constant
    (c*d == BigInt("13316075197586561")) &&
    ((a*b - c*d) == BigInt(1)))
	cout << "CORRECT!!! The formula ab-cd=1 is verified by BigInt" << endl;
else
	cout << "ERROR!!! The formula ab-cd=1 is not verified by BigInt" << endl;
cout << "Now we compute the same expression ab-cd in machine float:" << endl;
cout << "ab-cd = " << aa*bb - cc*dd << endl;
cout << "You should see the result as 2." << endl;

  return 0;
}
//====================================== 

