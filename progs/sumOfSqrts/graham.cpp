/* **************************************
   File: graham.cc

   Purpose: To illustrate Graham's example, where
	two sums of squareroots are equal for 37 decimal
	digits after the decimal point.  Each sum has
	9 square-roots.

   Usage:
	% graham

   CORE Library Version 1.2
   $Id: graham.cpp,v 1.4 2010/01/08 17:02:02 exact Exp $
 ************************************** */


#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

int main(int argc, char** argv) {

  int relprec;
  if (argc > 1) relprec = atoi(argv[1]);
  else relprec = 100;
 
  setDefaultRelPrecision(relprec);	// relative precision in bits
  cout.precision(bits2digits(relprec));  	// print precision in digits

  long e1 = 1000001;
  long e2 = 1000025;
  long e3 = 1000031;
  long e4 = 1000084;
  long e5 = 1000087;
  long e6 = 1000134;
  long e7 = 1000158;
  long e8 = 1000182;
  long e9 = 1000198;
  
  double e
    = sqrt(e1) + sqrt(e2) + (sqrt(e3) + sqrt(e4))
      + ((sqrt(e5) + sqrt(e6)) + (sqrt(e7) + sqrt(e8)))
	+ sqrt(e9);
  
  long f1 = 1000002;
  long f2 = 1000018;
  long f3 = 1000042;
  long f4 = 1000066;
  long f5 = 1000113;
  long f6 = 1000116;
  long f7 = 1000169;
  long f8 = 1000175;
  long f9 = 1000199;
  
  double f
    = sqrt(f1) + sqrt(f2) + (sqrt(f3) + sqrt(f4))
      + ((sqrt(f5) + sqrt(f6)) + (sqrt(f7) + sqrt(f8)))
	+ sqrt(f9);

  
  cout << "Graham's Example (comparing 2 sums of 10 square roots\n";
  cout << " e : " << e << endl;
  cout << " f : " << f << endl;
 
  double diff = e - f;

  //cout << diff.approx() << endl;
  //cout << " Root bound of e - f = "
  //          << diff.rep()->get_constructive_bound() << endl;
  cout << " e == f ? " 
            << (diff == 0 ? " yes (ERROR!)" : " no (CORRECT)") << endl;
  cout << " e - f = " << diff << endl;
  cout << flush;
  
  return 0;
}

