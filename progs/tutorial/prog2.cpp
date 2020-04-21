/* ************************************************
   File: prog2

   Purpose:  An example program showing the comparison
             between algebraically identical expressions
	     sqrt(x) + sqrt(y) and sqrt(x + y + 2*sqrt(xy))
             
	     
	     
   Usage: 

        % prog2

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog2.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

int main(int argc , char ** argv) {

  setDefaultInputDigits(CORE_INFTY);
  double x = "12345/6789";  // rational format
  double y = "1234567890.0987654321";//  approximate format
  double e = sqrt(x) + sqrt(y);
  double f = sqrt(x + y + 2 * sqrt(x*y));
  std::cout << "e == f ? " << ((e == f) ?
			       "yes (CORRECT!)" :
			       "no (ERROR!)"  ) << std::endl;
}					 
