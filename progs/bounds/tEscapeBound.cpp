/* **************************************************
 * file: tEscapeBound.cpp
 *
 * 	Tests the Escape Bound Mechanism.
 *
 * 	Usage:
 *
 * 		time ./tEscapeBound
 * 		time ./tEscapeBound 1000
 *
 * 	to set the EscapeBound to some default or to 1000, and see
 * 	the difference in timing
 *
 *
 * 	The Escape Bound is an arbitrary user-settable value 
 * 	(currently defaults to 10,000 bits) which is used
 * 	as the root bound for transcendental expressions.
 *
 * 	If the value of a transcendental expression is smaller than
 * 	2^{-Escape_Bound}, then we accept the value as 
 * 	a "conditional zero".  We also print a
 * 	"Zero Assertion" certificate in the Core_Diagnostics File.   
 *
 * 	We use two well-known formulas for Pi from Machin:
 *
 * 		Machin2: Pi = 16*atan(1/5) - 4*atan(1/239) 
 *		Machin3: Pi = 4*(atan(1/2)+atan(1/5)+atan(1/8))
 *
 * 	In this program, we use the builtin constants Pi()
 * 	and function atan(x) to create transcendental expressions
 * 	that are identically 0, based on Machin's formulas.
 *	We then test if these expressions are zero.
 * 	The Escape Bound should kick in, to give us conditional zeros.
 *
 * 	NOTE: Should explicitly set the Escape Bound in this example.
 *
 * Jihun and Chee, May 2010.
 * Since Core 2.0.
 *
 * ************************************************* */
#define CORE_LEVEL 3

using namespace std;

#include "CORE.h"

int main (int argc, char* argv[]) {

  int bd = 5000;  // default escape bound
  if (argc>1) bd = atoi(argv[1]);  // user sets the bound

  // coreErrorFlag is initially 0, and is incremented each time
  // an error is detected by Core.
  cout << "Testing escape bound in Core2:" << endl;
  cout << "===============================================" << endl;
  cout << "coreErrorFlag = " << coreErrorFlag<< endl;
  Expr machin2 = 16*atan(Expr("1/5")) - 4*atan(Expr("1/239"));
  Expr machin3 = 4*(atan(Expr("1/2"))+atan(Expr("1/5"))+atan(Expr("1/8")));
  // NOTE: we use atan(Expr("1/5")) instead of atan("1/5") because
  // some of the newer g++ compilers (g++-4.1.1 in particular) could
  // not do automatic type conversion for this case.
  
  cout << "default Escape Bound = " << get_escape_bound() << endl;
  set_escape_bound(bd);
  cout << "set Escape Bound = " <<  get_escape_bound() << endl;
  set_error_message("New Escape Bound is 5000"); // it returns old message.
  
  // Printout values for Pi from formulas:
  cout<< std::setprecision(60);
  cout<< "First machin formula for Pi: 16*atan(1/5) + 4*atan(1/239)"
	<< endl << " = " << machin2.approx(200)<< endl;
  cout<<
    "Second machin formula for Pi: 4*(atan(1/2)+atan(1/5)+atan(1/8))"
	<< endl << " = " << machin3.approx(200)<< endl;

  // Test the operation of Escape Bounds:
  cout << "===============================================" << endl;
  if (pi() == machin2) {
    cout << "CORRECT!!! First machin formula is (conditionally) zero" << endl;
  } else {
    cout << "ERROR!!! First machine formula is not zero" << endl;
    cout << machin2 << endl;
  }

  if((pi() - machin3).sign() == 0) {
    cout << "CORRECT!!! Second machin formula is (conditionally) zero" << endl;
  } else {
    cout << "ERROR!!! Second machine formula is not zero" << endl;
    cout << machin3 << endl;
  }
  if (coreErrorFlag>0) cout << "ERROR!!! "<<endl;
  else cout << "No Error detected" << endl;
  cout << "===============================================" << endl;
  return 0;
}
