/* **************************************
   File: pisot.cpp

   Description: 
	Some tests of Pisot numbers:

	Golden ratio 

	((1+\sqrt{5})/2)^{100} =
	  792,070,839,848,372,253,126.999,999,999,999,999,999,998,737,486,..

	NOTE: Core Library confirms this
	value, which is taken from a book review on the 
	"Pisot and Salem Numbers" by Bertin, et al,
	Birkhaeuser, Basel 1992.

   Author:  Chee Yap 
   Date:    July 20, 2002

   Since Core Library  Version 1.5
   $Id: pisot.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#ifndef Level
#  define Level 3
#endif

#include "CORE.h"

using namespace std;


// ==================================================
// main 
// ==================================================

int main()
{

	setDefaultAbsPrecision(30);
	cout.precision(50);

	double phi = (1 + sqrt(double(5)))/2;

	double phic = pow(phi,100);

	phic.approx(200);
	cout << "phi^{100} = " << phic << endl;

}

