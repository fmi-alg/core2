/***************************************************************
 * File: scheinermanx.cpp
 * 	-- interactive version of scheinerman.cpp
 * 	-- it will pause at each step waiting for any character to be typed
 * 	-- must be compiled with "-lcurses"
 * 	-- but scrolling windows still need to be implemented....
 *
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
 * 	wrong way to prove such identities.  
 *
 * 	Their conversation has been paraphrased to suit Core Library:
 *
 * Date:   Apr 14, 2004
 * Since Core Library Version 1.7
 * $Id: scheinermanx.cpp,v 1.1 2009/10/19 17:01:56 exact Exp $
 **************************************************************/


#include <curses.h>
#include "CORE/CORE.h"
using namespace std;

int main() {

// ****************************************************************************
// Setting up for interactivity (from Pradeep Padala's NCURSES Programming HOWTO)
// ****************************************************************************
int ch;
initscr();	/* start curses mode */
raw();		/* line buffering disabled -- else you need to type ENTER to read */
noecho();	/* the typed char is not important to see */
printw("When program pauses, type any char to continue.\n");
refresh(); ch = getch();

// ****************************************************************************
// Set the output to show 10 digits (like the calculator)
// ****************************************************************************
  printw("==============================================\n");
  printw("SCHEINERMAN'S TEST\n");
  printw("==============================================\n");
refresh();
  setDefaultOutputDigits(10);
  //cout << "###########   Output Digits = 10 \n" << endl;

// ****************************************************************************
// TEACHER: Show that sqrt(2) + sqrt(5 - 2* sqrt(6))  = sqrt(3)
  printw("TEACHER: Show that sqrt(2) + sqrt(5 - 2* sqrt(6))  = sqrt(3)\n\n");
  refresh(); ch = getch();
// ****************************************************************************

  double X = sqrt(Expr(2)) + sqrt(5 - 2* sqrt(Expr(6)));
  double Y = sqrt(double(3));

  printw("STUDENT: let me use my calculator...\n\n");
  	printw("           X = sqrt(2) + sqrt(5-2 sqrt(6)) \n");
  	printw("             = "); refresh();
	cout << X ; printw("\n");
  refresh(); ch = getch();
  printw("           Y = sqrt(3) \n");
  	printw("             = "); refresh();
	cout << Y; printw("\n"); refresh();
  printw("        So they are the same!\n");
	printw("        My calculator that they are equal to 1.732050808\n\n");
  refresh(); ch = getch();
// ****************************************************************************
// TEACHER: That is an accident!  Your calculator display only 10 digits.
  printw( "TEACHER: That is an accident!");
	printw( " Your calculator only displays 10 digits.\n\n");
  refresh(); ch = getch();
  printw( "STUDENT: It cannot be coincidence.\n\n" );
  refresh(); ch = getch();
  printw( "TEACHER: Of course it can.\n");
	printw( "         Here, compute this on your calculator:\n" );
	printw( "         A = sqrt(75025) + sqrt(121393) + sqrt(196418) + sqrt(317811)\n\n");
// ****************************************************************************
		
  double A = sqrt(Expr(75025))+sqrt(Expr(121393))+sqrt(Expr(196418))+sqrt(Expr(317811));

refresh(); ch = getch();
  printw( "STUDENT: OK,...  (after a pause) ... I get\n\n");
  	printw( "            A = "); refresh();
	cout << A; refresh(); printw("\n\n");
refresh(); ch = getch();
	
// ****************************************************************************
   printw( "TEACHER: Good, now compute:\n");
	printw( "          B = sqrt(514229) + sqrt(832040)\n\n");
// ****************************************************************************

refresh(); ch = getch();
  double B = sqrt(Expr(514229)) + sqrt(Expr(832040));
  printw( "STUDENT:    Sure,... (after a pause) ... I get\n\n");
  	printw( "            B = "); refresh();
	cout << B; refresh(); printw("\n\n");
  printw( "         Wow, they are both equal to 1629.259889 !! \n\n");
refresh(); ch = getch();

// ****************************************************************************
  printw( "TEACHER:  But they are NOT the same.\n");
	printw( "          Let me show you in Core Library...\n");
	printw( "          First, we must display the output to 20 digits\n\n");
// ****************************************************************************

initscr();	/* start curses mode */

// ****************************************************************************
// Set the output to 20 digits first
// ****************************************************************************
  setDefaultOutputDigits(20);

// ****************************************************************************
// Redisplay A and B at 20 digit
// ****************************************************************************
refresh(); ch = getch();
  printw( "          Here is A again:   " ); refresh();
	cout << A; refresh(); printw("\n");
refresh(); ch = getch();
  printw( "          Here is B again:   " ); refresh();
	cout << B; refresh(); printw("\n\n");

//  printw( "NARRATOR: Your printout should show \n");
//	printw( "             A=1629.25888633142\n");
//	printw( "             B=1629.25888630189.\n\n");

// ****************************************************************************
// Lesson Continues
// ****************************************************************************
refresh(); ch = getch();
  printw( "TEACHER: These two numbers differ at the 9th decimal place. \n");
refresh(); 
	printw( "          So you see, it is impossible to numerically determine\n");
refresh(); 
	printw( "          if two expressions are the same, no matter how many\n");
refresh(); 
	printw( "          digits you compute.  They will agree except for the last digit\n");
	printw( "	   Go back to our previous example; let us compute\n");
refresh(); 
	printw( "               X = sqrt(2) + sqrt(5 - 2* sqrt(6))\n");
	printw( "               Y = sqrt(3)\n");
refresh(); 
	printw( "          to 70 digits:\n\n");
refresh(); ch = getch();

// ****************************************************************************
  setDefaultOutputDigits(70);
  printw("NARRATOR: The output digits is now set to 70.\n\n" );
// ****************************************************************************
  printw( "\n X = sqrt(2) + sqrt(5-2 sqrt(6)) \n");
  	printw( "     = "); refresh();
	cout << X; refresh(); printw("\n");
refresh(); ch = getch();
  // Prints: X=1.7320508075688772935274463415058723669428 (41 digits)
  // REMARK: In Core2, it prints only X=1.732050807568877 !!!
  printw( "\n Y = sqrt(3) \n");
  	printw( "     = "); refresh();
	cout << Y; refresh(); printw("\n\n");
refresh(); ch = getch();
  // Prints: Y=1.73205080756887729352744634150587236694 (39 digits)
  // REMARK: In Core2, it prints only Y=1.73205080756888 !!!

// ****************************************************************************
  printw( "STUDENT: Hey, they seem to agree,\n");
	printw( "          but neither one printed 70 digits!\n\n");
refresh(); ch = getch();
//
  printw( "TEACHER: Oh, I know what is going on...\n");
	printw( "          I read in the Tutorial that Core Library never prints\n");
        printw( "          more digits than it CURRENTLY knows about.\n");
	printw( "          Apparently, the current approximations do not have\n");
refresh(); 
        printw( "          70 digits of accuracy.\n");
	printw( "          We must first ask Core Library to approximate X and Y \n");
	printw( "          to at least 70 digits.  Let's see: 70 digits is less\n");
refresh(); 
        printw( "          than 4*70 = 280 bits.  So we force CORE to evaluate\n");
        printw( "          to 280 relative bits of precision:\n\n");
// ****************************************************************************

refresh(); ch = getch();
  printw( "CORE LIBRARY: X and Y are now approximated to 280 bits of relative precision\n\n");
  X.approx(280,CORE_INFTY);
  Y.approx(280,CORE_INFTY);

  printw( "X = "); 
  refresh(); cout << X; refresh(); printw("\n");
refresh(); ch = getch();
// X = 1.732050807568877293527446341505872366942805253810380628055806979451933
  printw( "Y = "); 
  refresh(); cout << Y; refresh(); printw("\n\n");
refresh(); ch = getch();
// Y = 1.732050807568877293527446341505872366942805253810380628055806979451933

// ****************************************************************************
  printw( "STUDENT: Great, both prints exactly 70 digits.  But why don't\n");
	printw( "         we ask Core Library whether X and Y are equal?\n\n");
// ****************************************************************************

refresh(); ch = getch();
  if (X == Y)
  printw( "CORE LIBRARY:  I declare X and Y equal (CORRECT!)\n\n");
  else 	
  printw( "CORE LIBRARY:  I declare X and Y unequal (ERROR!)\n\n");

// ****************************************************************************
  printw( "STUDENT: Hey, Core Library got it right!\n\n");

refresh(); ch = getch();
  printw( "TEACHER: That must be a fluke. Let us run a few more tests.\n");
	printw( "         (After a few more identities and non-identities...)\n");
	printw( "         Amazing!  Core Library got it right every time!\n");
refresh(); 
	printw( "         I am convinced that Core Library does some kind of\n");
        printw( "         theorem proving.  After all, we know that that\n");
        printw( "         it is impossible to decide equality by approximation.\n\n");

refresh(); ch = getch();
  printw( "STUDENT: Thanks, Teach!  That was a great object lesson.\n\n");
// ****************************************************************************
// Cleanup:
// ****************************************************************************
refresh(); ch = getch();
endwin();
 
  return 0;
}


