/* **************************************************
 * file: tCutOffBound.cpp
 *
 * 	Testing the CutOff Bound Mechanism.
 *
 * 	USAGE:
 * 		./tCutOffBound 
 * 		./tCutOffBound 1000
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

  int bd=800; // default cut off bound
  if (argc>1) bd= atoi(argv[1]);	// user can set this

  // coreErrorFlag is initially 0, and is incremented each time
  // an error is detected by Core.
  cout << "Testing CutOff bound in Core2:" << endl;
  cout << "===============================================" << endl;
  cout << "coreErrorFlag = " << coreErrorFlag<< endl;
  
  cout << "default Cut Off Bound = " << get_cut_off_bound() << endl;
  
  Expr E = 2;
  int k=13;

  set_cut_off_bound(bd);
  cout << "set Cut Off Bound = " <<  get_cut_off_bound() << endl;
  set_error_message("New cut off bound is 800"); // it returns the old error message

  for(int i=0; i<k; ++i)
	E = E*E;
  E = E-1;		// now, E+1 = exp*(2,k) (a stack of 2's of length k)
  for(int i=0; i<k; ++i)
	E = sqrt(E);
  E -= 2;		// now, E = sqrt[k]( exp*(2,k) - 1) - 2 (very close to 0!)
  if(E == 0) {
	cout << "WRONG, but OK within Cut-off bound!" << endl;
  } else {
	cout << "CORRECT, but you are unlikely to see this!" << endl;
  }

}
