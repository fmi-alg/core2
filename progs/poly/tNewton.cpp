/* **************************************
   File: tNewton.cpp

	   Uses the Newton Class to compute square roots and cube roots
	   of any int N.  Result is stored in a file, and then read out again
	   at any desired precisions.  This is self-tested for accuracy.

   Usage:
		% tNewton [N = 2] [prec = 2000] [readprec = 100]

	where
		[N] is value whose square-root and cube-root is to be taken
		[prec] is the precision (in number of bits) of the computation.
		[readprec] is the precision to read back from file

	The number read from the file is squared (cubed) and compared to
	the argument [N].  Their difference should be no more than 
	2^{4-[readprec]} (otherwise an error will be reported).

	ASSERT: [prec] >= [readprec]

	The result is stored in the files named
		output/sqrt[N].[prec]
		output/cube[N].[prec]

	CORRECTNESS CHECK:
	Suppose we want to verify that |x - sqrt(N)| < 2^{-p} (p = prec).
	Let sqrt(N) = x+e.  Then |N-x^2| = |(x+e)^2 - x^2| > 2x|e|.  I.e.,
	diff2 := |N/x  - x|/2 > |e|.  
	If we ASSUME x>1, then 2|e|>diff2>|e|.
	So if we make
		|e|<2^{-p-1}    (***)
	then
		-p > diff2.uMSB().
	We test this inequality in our code (see below). 

	To ensure (***), we compute x (or read x from file)
	to (p+1) absolute bits of precision.
	[Important Note: when reading x from file, the accuracy
	argument is relative, so you need to first convert p to the
	corresponding relative accuracy].

	Similarly, suppose we want |x - cuberoot(N)| < 2^{-p}.
	Let cuberoot(N) = x+e where x>0.  Then |N-x^3| > 3x|e|(x-|e|) > 3x|e|,
	assuming x-|e| >= 1.  Hence it is enough to verify
		-p >= diff3.uMSB()
	where diff3 = |(N/x*x) - x|/3 if e<2^(-p-2)

	Then, similar considerations apply that are similar to (***).

	REMARK:  There are more direct methods to test the case of sqrt
	but these do not extend to cuberoot (since CORE does not directly
	support cuberoot currently).

   Author:  Chee Yap and Zilin
   Date:    Aug 2, 2002 (updated, Oct 10, 2004).

   Since Core Library  v1.4
   $Id: tNewton.cpp,v 1.4 2007/04/09 02:55:28 exact Exp $
 ************************************** */

#include <fstream>
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

    // Set Absolute Precision prec
    int prec;
    if (argc >= 3) {
  	prec = atoi(argv[2]);	// If user specifies [prec]
    } else { 			// default absolute precision 
	prec = 2000;
    }
  
    // Set bit precision for reading from file
    int readprec;
    if (argc >= 4) {
  	readprec = atoi(argv[3]);
    } else {
	readprec = 100;		// default value
    } 
    // Make sure that [prec] >= [readprec]-4
    if (readprec-4 > prec) 
	prec = readprec;

    char prec_str[200];
    sprintf(prec_str, "%d", prec);
    fname1 += prec_str; fname2 += prec_str;

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);
				// choose scientific notation
    cout.precision(11); 	// default Print Digits = 11
    
    cout << "--------------------------------------------------------\n";
    cout << "Computing Square Root and Cube Root of N=" << N
    		 << " to " << prec << " absolute bits" << endl;
    cout << "--------------------------------------------------------\n";
  
    // Open Output files
    ofstream ofs1, ofs2;	// output streams for sqrt and cuberoot
    ofs1.open(fname1.data());
    if (!ofs1)
	cout << "error opening output file  " << fname1 << endl;
    ofs2.open(fname2.data());
    if (!ofs2)
	cout << "error opening output file " << fname2 << endl;

    //============================================================
    // Step 1: compute sqrt(N) to prec absolute bits:
    //============================================================

    NT cs2[] = {-2, 0, 1};	// coefficients of Polynomial
    cs2[0] = -N;
    Sturm<NT> NP (Polynomial<NT>(2, cs2));	// Sturm object

    BigFloat del;
    BigFloat val;

    val = NP.newtonIterE(prec, N, del);
    		// compute to sqrt(N) to prec absolute bits
    cout << "   ---------------------------------------------------\n";

    ofs1 << "# Filename: poly/" << fname1 << endl; 
    ofs1 << "# This stores sqrt(" << N << ") to " << prec
		<< " absolute bits, in base 10" << endl; 
    int base = 10;
    writeToFile(val, ofs1, base);
    ofs1 << "# END of FILE " << fname1 << endl; 
    ofs1.close();

    // CHECKING
    cout << "   ---------------------------------------------------\n";
    cout << "   SELF-CHECK for sqrt: reading from output file\n";
    cout << "   ---------------------------------------------------\n";
    ifstream ifs1(fname1.data());	// input stream for sqrt 
    BigFloat rootN;
    readFromFile(rootN, ifs1);
    	// read [readprec] many absolute bits of precision
	// REMARK: we need to add "1" to get (***) above
	//         we need to add (1+N.uMSB())/2 to convert the relative precision
	//         of reading to an absolute precision
    cout << "  Sqrt(" << N << ") = " << rootN << endl;

    cout << "  >>> Reading [readprec]=" << readprec << " bits \n";
    Expr diff2 = (Expr(N)/Expr(rootN) - Expr(rootN))/2;
    diff2.approx(CORE_INFTY, readprec);
    cout << "uMSB=" << diff2.BigFloatValue().uMSB() << endl;
    cout << "readprec=" << -readprec << endl;
    if (diff2.BigFloatValue().uMSB() > -readprec)
	cout << "  >>> ERROR!  Value read from file is not accurate \n";
    else
	cout << "  >>> CORRECT!  Value read from file is accurate \n";
    ifs1.close();   ifs1.open(fname1.data());	// close and reopen file

    //============================================================
    cout << "  >>> AGAIN, reading all " << prec << " bits \n";
    readFromFile(rootN, ifs1);	// read all digits in file
    diff2 = (Expr(N)/Expr(rootN) - Expr(rootN))/2;
    diff2.approx(CORE_INFTY, prec);
    cout << "uMSB=" << diff2.BigFloatValue().uMSB() << endl;
    cout << "prec=" << -prec << endl;
    if (diff2.BigFloatValue().uMSB() > -prec)
	cout << "  >>> ERROR!  Value read from file is not accurate \n";
    else
	cout << "  >>> CORRECT!  Value read from file is accurate \n";
    ifs1.close();

    //============================================================
    // Step 2: compute cuberoot(N) to prec absolute bits:
    //============================================================

    NT cs3[] = {-2, 0, 0, 1};	// coefficients of Polynomial
    Sturm<NT> NP2 (Polynomial<NT>(3, cs3));	// Newton object
    NP2.seq[0].setCoeff(0, -N);	// make -N the constant coefficient

    val = NP2.newtonIterE(prec, N, del);
    		// compute to cuberoot(N) to prec abs. bits

    ofs2 << "# Filename: poly/" << fname2 << endl; 
    ofs2 << "# This stores cuberoot(" << N << ") to " << prec
		<< " absolute bits, in base 10" << endl; 
    writeToFile(val, ofs2, base);
    ofs2 << "# END of FILE " << fname2 << endl; 
    ofs2.close();
 
    //CHECKING:
    cout << "   ---------------------------------------------------\n";
    cout << "   SELF-CHECK for cube root: reading from output file\n";
    cout << "   ---------------------------------------------------\n";
    ifstream ifs2(fname2.data());	// input stream for cuberoot
    readFromFile(rootN, ifs2); 
    	// read [readprec] bits 
    cout << "  CubeRoot(" << N << ") = " << rootN << endl;

    Expr ExprrootN=Expr(rootN);
    Expr diff3 = (Expr(N)/(ExprrootN*ExprrootN) - ExprrootN)/3;
    diff3.approx(CORE_INFTY, readprec);
    cout << "uMSB=" << diff3.BigFloatValue().uMSB() << endl;
    cout << "readprec=" << -readprec << endl;
    if (diff3.BigFloatValue().uMSB() > -readprec)
	cout << "  >>> ERROR!  Value read from file is not accurate \n";
    else
	cout << "  >>> CORRECT!  Value read from file is accurate \n";
    cout << "----------------------END-------------------------------\n";

    return 0;
  }// main


