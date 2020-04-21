/* **************************************
   File: timeNewton.cpp

	Investigates the effects of relative error in computing Newton iterates
	on speed of convergence.   More precisely, when we compute the
	Newton iterate

		N(x) = x - f(x)/f'(x),
	
	to what relative precision should f(x)/f'(x) be computed?  
	
	This precision, if unspecified, is given by the global variable

    			defBFdivRelPrec = 30;  	// default value
	
	Call this the (relative) ``precision for computing Newton iterates''.
	

   Usage:
		% timeNewton [N = 2] [rprec = 30] [acc = 2000]

	where
		[N] is value whose square-root is to be taken
		[rprec] is the precision for computing Newton iterates
		[acc] is the precision (in number of bits) of the computation.

	E.g.,
		% timeNewton 7 100 2000


   Author:  Chee, Vikram, Zilin
   Date:    Sep 28, 2004

   Since Core Library  v1.7
   $Id: timeNewton.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#include <fstream>
#include "CORE/Timer.h"
#include "CORE/CORE.h"
using namespace std;

// ==================================================
// Main
// ==================================================

// Default number type is NT=BigInt
#if defined _NT_INT
  typedef int NT;
#elif defined _NT_LON
  typedef long NT;
#elif defined _NT_EXP
  typedef Expr NT;
#elif defined _NT_RAT
  typedef BigRat NT;
#elif defined _NT_BF
  typedef BigFloat NT;
#elif defined _NT_BINT
  typedef BigInt NT;
#else
  typedef BigInt NT;
#endif

  int main(int argc, char** argv){

    //============================================================
    // Process Command Line Arguments
    //============================================================
    // Initialize file names for storing results

    string fname1 = "outputs/sqrt";
    string fname2 = "outputs/cuberoot";

    // Initialize N
    int N;
    if (argc >= 2) {
  	N = atoi(argv[1]);  fname1 += argv[1];  fname2 += argv[1];
	fname1 += ".";  fname2 += ".";			// assigned value
    } else {
	N = 2; fname1 += "2.";  fname2 += "2.";		// default value
    } 

    // Initialize the relative precision for Newton iterates
    defBFdivRelPrec = 30;  	// 30 is system default in Core Library
    if (argc >= 3)
	defBFdivRelPrec = atoi(argv[2]);
  
    // Set Absolute Precision prec
    int prec;
    if (argc >= 4) {
  	prec = atoi(argv[3]);	// If user specifies [prec]
    } else { 			// default absolute precision 
	prec = 2000;
    }
  
    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);
				// choose scientific notation
    cout.precision(11); 	// default Print Digits = 11
    
    cout << "--------------------------------------------------------\n";
    cout << "Timing Test using relative iterate precision of " << defBFdivRelPrec <<
	    "\n Compute Cube Root of N=" << N
    		<< " to " << prec << " absolute bits" << endl;
    cout << "--------------------------------------------------------\n";
  
    //============================================================
    // Step 1: compute cuberoot(N) to prec absolute bits:
    //============================================================

    NT cs2[] = {-2, 0, 0, 1};	// coefficients of Polynomial
    Sturm<NT> NP (Polynomial<NT>(3, cs2));	// Sturm object
    NP.seq[0].setCoeff(0, -N);	  // make -N the constant coefficient

    BigFloat del;
    BigFloat val;

    Timer t;
    t.start();
    val = NP.newtonIterE(prec, N, del); 
    		// compute to sqrt(N) to prec absolute bits
    t.stop();
    cout << "timing: " << t.getSeconds() << endl;


    return 0;
  }// main


