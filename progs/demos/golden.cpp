/* ************************************************
   File: golden.cpp

   Purpose:  Simple Demo Program 
		
   Usage:
        % simple

   Since Core Library
   $Id: golden.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"  // this must appear after the standard headers
using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> Poly;

int main() {

  //////////////////////////////////////////////////////////////
  // Golden Ratio Test
  //////////////////////////////////////////////////////////////
  NT c[] = {-1, -1, 1};
  Poly P(2,c);
  double phi = rootOf(P,2); 

  cout << "==============================================" << endl;
  cout << "GOLDEN RATIO TESTS" << endl;
  cout << "==============================================" << endl;
  cout << "Golden ratio, phi = " << phi << endl;

  if (phi*phi - phi - 1 == 0)
	  cout << "CORRECT!  phi^2 = 1+ phi" << endl;
  else
	  cout << "ERROR!  phi^2 != 1+ phi" << endl;

  //////////////////////////////////////////////////////////////
  // A random polynomial test 
  //////////////////////////////////////////////////////////////
  NT cc[] = {123, 23, 3, -2};
  Poly Q(3,cc);
  double theta = rootOf(Q,0); 
  cout << "Polynomial Q(X) = "; Q.mapleDump();
  cout << "r = rootOf(Q,0) = " << theta << endl;

  double theta2 = theta*theta;
  if (2*theta*theta2 == 3*theta2 + 23*theta  + NT(123) )
	  cout << "CORRECT!  2*rrr == 3rr  + 23r + 123" << endl;
  else
	  cout << "ERROR!  2*rrr != 3rr  + 23r + 123" << endl;

  return 0;
}

