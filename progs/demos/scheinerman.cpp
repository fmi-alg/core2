/***************************************************************
 * File: scheinerman.cpp
 * Author: Chee Yap <yap@cs.nyu.edu>
 *
 * Description:  
 * 	Based on a paper by E.R. Scheinerman,
 *		``When Close Enough is Close Enough''
 *	Amer.Math.Monthly, 107(2000)489--499.
 * 	
 * 	There is an imaginary conversation between a student
 * 	who tried to prove an algebraic identity by numerical
 * 	approximation, and teacher who insists that it is the
 * 	wrong way to prove such identities.  Who is right?
 *
 * 	Their conversation has been paraphrased to suit Core Library:
 *
 * Date:   Apr 14, 2004
 * Since Core Library Version 1.7
 * $Id: scheinerman.cpp,v 1.10 2010/05/27 14:35:25 exact Exp $
 **************************************************************/

#include "CORE/CORE.h"

using namespace std;

int main() {

// ****************************************************************************
// Set the output to show 10 digits (like the calculator)
// ****************************************************************************
  cout << "==============================================" << endl;
  cout << "SCHEINERMAN'S TEST" << endl;
  cout << "==============================================" << endl;
  setDefaultOutputDigits(10);
  //cout << "###########   Output Digits = 10 \n" << endl;

// ****************************************************************************
// TEACHER: Show that sqrt(2) + sqrt(5 - 2* sqrt(6))  = sqrt(3)
  cout << "TEACHER: Show that sqrt(2) + sqrt(5 - 2* sqrt(6))  = sqrt(3)\n\n";
// ****************************************************************************

  double X = sqrt(Expr(2)) + sqrt(5 - 2* sqrt(Expr(6)));
  double Y = sqrt(double(3));

  cout << "STUDENT: let me use my calculator...\n\n"
  	<< "           X = sqrt(2) + sqrt(5-2 sqrt(6)) \n"
  	<< "             = " << X.approx(100) << endl;
  cout	<< "           Y = sqrt(3) \n"
  	<< "             = " << Y.approx(100) << endl;
  cout 	<< "        They are the same!\n\n";
  cout	<< "NARRATOR: Your printout should show X and Y to be \n"
	<< "             1.732050808\n";

// ****************************************************************************
// TEACHER: That is an accident!  Your calculator display only 10 digits.
  cout	<< "TEACHER: That is an accident!"
	<< " Your calculator only displays 10 digits.\n" << endl;
  cout	<< "STUDENT: It cannot be coincidence.\n" << endl;
  cout	<< "TEACHER: Of course it can.\n"
	<< "         Here, compute this on your calculator:\n" 
	<< "         A = sqrt(75025) + sqrt(121393) + sqrt(196418) + sqrt(317811)\n"
	<< endl;
// ****************************************************************************
		
  double A = sqrt(Expr(75025))+sqrt(Expr(121393))+sqrt(Expr(196418))+sqrt(Expr(317811));
  A.approx(100);

  cout	<< "STUDENT: OK,...  (after a pause) ... I get\n\n"
  	<< "            A = " << A << endl << endl;
	
// ****************************************************************************
   cout	<< "TEACHER: Good, now compute:\n"
	<< "          B = sqrt(514229) + sqrt(832040)\n" << endl;
// ****************************************************************************

  double B = sqrt(Expr(514229)) + sqrt(Expr(832040));
  B.approx(100);
  cout	<< "STUDENT:    Sure,... (after a pause) ... I get\n\n"
  	<< "            B = " << B << endl;
  cout	<< "         They are both equal to 1629.259889 !! \n\n";

  cout	<< "NARRATOR: Your printout should show A and B to be \n"
	<< "             1629.259889\n";
// ****************************************************************************
  cout	<< "TEACHER:  But they are NOT the same.\n"
	<< "          Let me show you in Core Library...\n" 
	<< "          First, we set the output to display 20 digits (done).\n" << endl;
// ****************************************************************************

// ****************************************************************************
// Set the output to 20 digits first
// ****************************************************************************
  setDefaultOutputDigits(20);

// ****************************************************************************
// Redisplay A and B at 20 digit
// ****************************************************************************
  cout	<< "          Here is A again:   " << A << endl;
  cout	<< "          Here is B again:   " << B << endl <<endl;

  cout	<< "NARRATOR: Your printout should show \n"
	<< "             A=1629.2598886331422998\n"
	<< "             B=1629.2598886301892384.\n"
	<< "             All computations so far has at least 100 bits (about 30 digits)"
	<< " of accuracy\n" << endl;
// May 20, 2010 (Chee):
// While preparing for Bath Talk, I see this discrepancy:
// 	A = sqrt(75025) + sqrt(121393) + sqrt(196418) + sqrt(317811)
// 	B = sqrt(514229) + sqrt(832040)
// Core gives		 A=1629.25988863 31422 818	(20 digits)
// 			 A=1629.25988863 31422 81832988	(25 digits)
// In Maple:		 A=1629.25988863 31422 998     	(20 digits)
// 			 A=1629.25988863 31422 99848838	(25 digits)
// Core gives 		 B=1629.25988863 01892 662	(20 digits)
//			 B=1629.25988863 01892 66223169	(25 digits)
// In Maple:		 B=1629.25988863 01892 384     	(20 digits)
// 			 B=1629.25988863 01892 38404283	(25 digits)
// 			 
// ****************************************************************************
// Lesson Continues
// ****************************************************************************
  cout	<< "TEACHER: You see, these two numbers differ at the 9th decimal place. \n"
	<< "          This shows that it is impossible to numerically determine\n"
	<< "          if two expressions are the same, no matter how many\n"
	<< "          digits you compute.  They will agree except for the last digit\n"
	<< "	     Going back to our previous example, let us display\n"
	<< "               X = sqrt(2) + sqrt(5 - 2* sqrt(6))\n"
	<< "               Y = sqrt(3)\n"
	<< "          to 70 digits:\n" << endl;

// ****************************************************************************
  setDefaultOutputDigits(70);
  cout	<< "NARRATOR: The output digits is now set to 70.\n" << endl;
// ****************************************************************************
  cout  << "\n X = sqrt(2) + sqrt(5-2 sqrt(6)) \n"
  	<< "     = " << X << endl;
  // Prints: X=1.7320508075688772935274463415058723669428 (41 digits)
  cout  << "\n Y = sqrt(3) \n"
  	<< "     = " << Y << endl << endl;
  // Prints: Y=1.73205080756887729352744634150587236694 (39 digits)
  //
  // TODO: the two outputs should agree for at least 60 digits,
  // but Core2 has a bug that continues to print before the error bound.
  // In other words, it is not truncating garbage digits in the output.
  // This fix should be easy to do.

// ****************************************************************************
  cout	<< "STUDENT: Hey, they seem to agree up to about 30 digits,\n"
	<< "          but neither one printed 70 digits!\n" << endl;
//
  cout	<< "TEACHER: Oh, I know what is going on...\n"
	<< "          I read in the Tutorial that Core Library never prints\n"
        << "          more digits than it CURRENTLY knows about.\n"
	<< "          Apparently, the current approximations do not have\n"
        << "          70 digits of accuracy.\n"
	<< "          We must first ask Core Library to approximate X and Y \n"
	<< "          to at least 70 digits.  Let's see: 70 digits is less\n"
        << "          than 4*70 = 280 bits.  So we must ask CORE to evaluate\n"
        << "          to 280 relative bits of precision:\n" << endl;
// ****************************************************************************

  cout << "CORE LIBRARY: X and Y are now approximated to 280 bits of relative precision\n"
	  << endl;
  X.approx(280,CORE_INFTY);
  Y.approx(280,CORE_INFTY);

  cout << "X = " << X << endl;
  cout << "Y = " << Y << endl << endl;
  cout	<< "NARRATOR: Your printout should show X and Y to be \n"
 << "    1.732050807568877293527446341505872366942805253810380628055806979451933\n";

// ****************************************************************************
  cout	<< "STUDENT: Great, both prints exactly 70 digits.  But why don't\n"
	<< "         we ask Core Library whether X and Y are equal?\n\n";
// ****************************************************************************

  if (X == Y)
  cout << "CORE LIBRARY:  I declare X and Y equal (CORRECT!)" << endl;
  else 	
  cout << "CORE LIBRARY:  I declare X and Y unequal (ERROR!)" << endl<<endl;

// ****************************************************************************
  cout	<< "STUDENT: Hey, Core Library got it right!\n" << endl;

  cout  << "TEACHER: That must be a fluke. Let us run a few more tests.\n"
	<< "         (After a few more identities and non-identities...)\n"
	<< "         Amazing!  Core Library got it right every time!\n"
	<< "         I am convinced that Core Library does some kind of\n"
        << "         theorem proving.  After all, we know that that\n"
        << "         it is impossible to decide equality by approximation.\n\n";

  cout	<< "STUDENT: Thanks, teach!  That was a great object lesson.\n\n";
// ****************************************************************************
 
  return 0;
}


