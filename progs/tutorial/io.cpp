/* ************************************************
   File: io.cpp

   Purpose:  IO examples from Tutorial.  These illustrate
	     the interaction between three kinds of precision parameters:

		(1) the precision for evaluation of expressions,
			controlled by setDefaultPrecision(rel,abs)
		(2) the precision of output,
			controlled by setDefaultOutputDigits(n, os).
			This basically calls the setprecision(digits) of
			ostreams.setprecision(n).
		(3) the precision for reading inputs,
			controlled by setDefaultInputDigits(n)

		In addition, there is the choice between scientific
		and positional format for output. 
		
   Usage:
        % io

   Since Core Library 1.7
   $Id: io.cpp,v 1.3 2010/06/11 17:49:30 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"

using namespace std;

int main( int argc, char *argv[] ) {

  // This is the default setting:
  // 		setDefaultPrecision(60, INFTY);
  //setDefaultInputDigits(CORE_INFTY);

  double X("-1234.567890");
  cout << "  Let X=1234.567890\n";
  cout << "  Default Printout of X : \n    "
	<< X << endl;
  cout << "  Printout of X after setprecision(10) : \n    "
	<< setprecision(10) << X 
	<< "\n     -- so `output width' (=10 here) counts the decimal point\n";
  cout << "  Printout of X after setprecision(6): \n    "
	<< setprecision(6) << X
	<< "\n     -- so the default width is 6" << endl;

  setScientificFormat();

  cout << "  Printout of X after setScientificFormat(): \n    "
	<< X << "\n     -- the width remains set at 6 \n"
  	<< "     -- but decimal point in Scientific width is not counted! \n"
  	<< "     -- Note the rounding of the last digit\n";
  cout << "  Printout of X after setprecision(10) : \n    "
	<< setprecision(10) << X << endl;
  cout << "  Printout of X after setprecision(11) : \n    "
	<< setprecision(11) << X 
	<< "\n     -- an artificial zero digit is printed"  << endl;
  cout << "  Printout of X after setprecision(12) : \n    "
	<< setprecision(12) << X 
	<< "\n     -- another artificial zero digit!"  << endl;
  cout << "  Printout of X after setprecision(100) : \n    "
	<< setprecision(100) << X 
	<< "\n     -- Should print correctly"  << endl;
  cout << "  Printout of X after setprecision(200) : \n    "
	<< setprecision(200) << X 
	<< "\n     -- Should see print out errors!"  << endl;
  cout << "  FIX: set defInputDigits to INFTY before reading X \n";

  defInputDigits = CORE_INFTY;
  setDefaultPrecision(CORE_INFTY,800);

  // X = double("1234.567890");
  X=string("-1234.567890"); // why is this a problem for Core2?

  cout << "  printout X after setprecision(20) : \n    "
	<< setprecision(20) << X  << endl;
  cout << "  Try Again: printout X after setprecision(200) : \n    "
	<< setprecision(200) << X 
	<< "\n     -- Should see no printout errors!"  << endl;

  setPositionalFormat();
  cout << "  Printout of X after setPositionalFormat(): \n    "
	<< X << endl;
  cout << "  Printout of X after setprecision(11): \n    "
	<< setprecision(11) << X
	<< "\n     -- only one extra zero printed, as expected" << endl;
  cout << "  Printout of X after setprecision(5): \n    "
	<< setprecision(5) << X
	<< "\n     -- rounding to a 4-digit integer" << endl;
  cout << "  Printout of X after setprecision(4): \n    "
	<< setprecision(4) << X
	<< "\n     -- we are forced to go to scientific format" << endl;

  cout << "\n To see what happens if X were not exact, we next set Y = 1/3.\n"
	<< "    Here Y is an expression\n"
  	<< "    We evaluate Y relative precision 53 (15.9 digits) \n"
	<< "    by calling setDefaultPrecision(53,CORE_INFTY). \n";
  double one = 1;
  double three = 3;
  double Y = one / three;
  setDefaultPrecision(53, CORE_INFTY);

  cout << "  Output In Positional Format\n";
  cout << "    Cout precision 12:      " << setprecision(12) << Y << endl;
  cout << "    Cout precision 15:      " << setprecision(15) << Y << endl;
  cout << "    Cout precision 16:      " << setprecision(16) << Y << endl;
  cout << "        -- no guarantee that you will see more digits!" << endl;
  cout << "    Cout precision 17:      " << setprecision(17) << Y << endl;
  cout << "        -- you may see more because CORE has exceeded the requirements!" << endl;
  cout << "    Cout precision 18:      " << setprecision(18) << Y << endl;
  cout << "    Cout precision 19:      " << setprecision(19) << Y << endl;
  cout << "    Cout precision 20:      " << setprecision(20) << Y << endl;
  cout << "    Cout precision 21:      " << setprecision(21) << Y << endl;
  cout << "    Cout precision 22:      " << setprecision(22) << Y << endl;
  cout << "    Cout precision 23:      " << setprecision(23) << Y << endl;
  cout << "    Cout precision 24:      " << setprecision(24) << Y << endl;
  cout << "    Cout precision 25:      " << setprecision(25) << Y << endl;
  cout << "    Cout precision 26:      " << setprecision(26) << Y << endl;
  cout << "    Cout precision 27:      " << setprecision(27) << Y << endl;
  cout << "    Cout precision 28:      " << setprecision(28) << Y << endl;
  cout << "    Cout precision 29:      " << setprecision(29) << Y << endl;
  cout << "    Cout precision 30:      " << setprecision(30) << Y << endl;
  cout << "    Cout precision 31:      " << setprecision(31) << Y << endl;

  cout << "THE NUMBER OF OUTPUT DIGITS SHOULD STOP SOMEWHAT BEFORE 31 \n"
	<< "SINCE CORE EXCEEDS THE REQUESTED PRECISION BY A FACTOR of 2\n" ;

  cout << "To ensure that you really see 31 DIGITS, we must compute to \n"
	<< "102 relative bits of precision: by calling Y.approx(103, CORE_INFTY) \n";

  Y.approx(103, CORE_INFTY);
  cout << "    Cout precision 31:      " << setprecision(31) << Y << endl;
  cout << "    Cout precision 36:      " << setprecision(36) << Y << endl;
  cout << "    Cout precision 41:      " << setprecision(41) << Y << endl;
  cout << "    Cout precision 46:      " << setprecision(46) << Y << endl;
  cout << "    Cout precision 51:      " << setprecision(51) << Y << endl;

  cout << "Now, we do Y.approx(123, CORE_INFTY) \n";

  Y.approx(123, CORE_INFTY);
  cout << "    Cout precision 51:      " << setprecision(51) << Y << endl;
}
