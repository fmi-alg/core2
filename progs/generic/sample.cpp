/************************************************************
  file: sample.ccp
  purpose:
	a simple sample CORE program

  BUGS: In Core2, this demo seems to have different behavior?  CHECK!

  CORE Library 
  $Id: sample.cpp,v 1.5 2007/05/01 16:46:49 exact Exp $
************************************************************/
 
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

// Important: place all standard header files BEFORE "CORE.h"
#include "CORE.h"

using namespace std;

int main(int argc, char* argv[]) {

  double x = 1.44;	// Inexact input; C++ literal input precision

  setDefaultInputDigits(20);  // This is more than usual C++ precision
  double y = "1.44";	// Use of string constructor 

  setDefaultInputDigits(CORE_INFTY); // force exact input 
  double z = "1.44";	// z is represented as a rational number
  
  int p = getDefaultOutputDigits(); // p = defOutputDigits (p=6)
  cout << "    C++'s Default Printout Digits is " << p << endl;
  cout << "    CORE Default Printout Digits is " << getDefaultOutputDigits() 
	<< endl;

  cout << "    x = 1.44;  // standard C++ literal input" << endl;
  cout << "Printout of x: " << x << endl;
  cout << "      sqrt(x): " << sqrt(x) << endl;
  cout << "==================================================" << endl;

  cout << setprecision(6) ;	// setprecision is a C++ function, std::setprecision()
  cout << "    y = \"1.44\". // string literal; defInputDigits = 20\n";
  cout << "    Set CORE Printout Digits to 6 " << endl;
  cout << "Printout of y: " << y << endl;
  cout << "      sqrt(y): " << sqrt(y) << endl;
  cout << "==================================================" << endl;

  cout << setprecision(6) ;
  cout << "    z = \"1.44\". // string literal; defInputDigits = Infinite\n";
  cout << "    Set CORE Printout Digits to 6 " << endl;
  cout << "Printout of z: " << z << endl;
  cout << "      sqrt(z): " << sqrt(z) << endl;
  cout << "==================================================" << endl;
  cout << "    Above, x, y and z look the same."
	<< "\n    To see that they are not the same, we force more"
	<< "\n    precision in evaluation and in output:" << endl;

  cout << " ---Set defRelPrec 133 bits = 40 digits"
	<< "\n ---Set defPrintDigits to 40 " << endl;
  defRelPrec = 133; // this gives 40 digits of precision
  setDefaultOutputDigits(40);

  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  cout << "z = " << z << endl;
  cout << "sqrt(x) = " << sqrt(x) << endl;
  cout << "sqrt(y) = " << sqrt(y) << endl;
  cout << "sqrt(z) = " << sqrt(z) << endl;
  cout << "==================================================" << endl;

  cout << "   Even though sqrt(z) looks like 1.2, the only way to"
	<< "\n   verify this is to do a comparison with 1.2:" << endl;

  setDefaultInputDigits(CORE_INFTY); // force exact input 
  double sqrtz = "1.2";  // exact input

  if (sqrt(z) == sqrtz)
	cout << "CORRECT!  sqrt(z) = 1.2 exactly" << endl;
  else
	cout << "ERROR!  sqrt(z) != 1.2 " << endl;
  cout << "==================================================" << endl;

  return (0);
}
