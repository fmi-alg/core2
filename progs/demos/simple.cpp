/* ************************************************
   File: simple.cpp

   Purpose:  Simple Program from Tutorial
   	This program will compile in Core Levels 1 and 3.
	In Level 3, there is no error.
	In Level 1, it is mostly error.
		
   Usage:
        % simple

   Core Library
   $Id: simple.cpp,v 1.2 2010/05/20 14:28:34 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"  // this must appear after the standard headers
using namespace std;

bool test(double x, double y, bool verbose=false) {
  double e = sqrt(x) + sqrt(y);
  double f = sqrt(x + y + 2 * sqrt(x*y));
  bool correct = ((e==f)? true : false);
  if (verbose) {
  cout << "==============================================" << endl;
  cout << ">>  x = " << x << endl;
  cout << ">>  y = " << y << endl;
  cout << ">>  e = sqrt(x) + sqrt(y)" << endl;
  cout << ">>  f = sqrt(x + y + 2*sqrt(x*y))" << endl;
  cout << ">>  e == f ? " << ((correct) ? 
           "  yes (CORRECT!)" :
           "  no (ERROR!)"  ) << endl;
  cout << "==============================================" << endl;
  }
  return correct;
}
  
int main() {

  double x = 1234567890; 
  double y = 1234567890.987654321; 

  test(x,y,true);

  cout << "=========== Testing Examples in a Loop ===========" << endl;
  int corr = 0;
  int incorr = 0;
  for (int i= 100; i<120; i++) {
	  for (int j = 117; j< 137; j++) {
		  if (test(i, j/7, false)) corr++;
		  else incorr++;
	  }
  }
  if (incorr>0)
	  cout << "ERROR! There are " << incorr << " wrong answers out of"
		<< corr+incorr << endl;
  else
	  cout << "CORRECT! All " << corr+incorr 
		<< " answers are right." << endl;
  return 0;
}

