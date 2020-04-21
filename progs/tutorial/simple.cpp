/* ************************************************
   File: simple.cpp

   Purpose:  Simple Program from Tutorial
		
   Usage:
        % simple

   Since Core Library 1.7
   $Id: simple.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

// this must appear after the standard headers:
#include "CORE/CORE.h"
using namespace std;

int main() {
  setDefaultInputDigits(CORE_INFTY);	// precision for reading inputs

  double x = "12345/6789"; 		// rational format
  double y = "1234567890.0987654321"; 	//  approximate format
  double e = sqrt(x) + sqrt(y);
  double f = sqrt(x + y + 2 * sqrt(x*y));

  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  cout << "e = sqrt(x) + sqrt(y)" << endl;
  cout << "f = sqrt(x + y + 2*sqrt(x*y))" << endl;
  cout << "e == f ? " << ((e == f) ? 
           "yes (CORRECT!)" :
           "no (ERROR!)"  ) << endl;
}

