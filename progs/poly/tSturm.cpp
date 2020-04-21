/* **************************************
   File: tSturm.cpp

   Description: 
	Testing the Sturm Class, which is in turn 
	    based on the Polynomial class.
	    The current version of tSturm using intervals
	    that are pairs of BigFloat's, not Expr's.  This
	    is much more efficient.  

	The number type for coefficients of polynomials is NT.
   	This program can be compiled with NT equal to any of the
	following: 
	
		BigInt (default), BigFloat, Expr, BigRat, int.

   Usage:
	% tSturm [opt-prec=100] [if-more-test-desired]

	where [opt-prec] is the optional precision bounding
	the bit size of the isolating intervals.
	It there is another argument (if-more-test-desired), then
	some slow polynomial evaluations will be invoked.

   Note:
	Some examples may fail when NT = int.
	This is to be expected, not a bug.  Below, we
	try to avoid doing such examples when NT = int.

   Author:  Chee Yap, Sylvain Pion, Vikram Sharma
   Date:    July 20, 2002

   Since Core Library  v1.4
   $Id: tSturm.cpp,v 1.6 2007/04/08 01:10:48 exact Exp $
 ************************************** */

#  define CORE_LEVEL 4

#include "CORE/CORE.h"
using namespace std;

// ==================================================
// Setup
// ==================================================
// Default NT is BigInt

#if defined _NT_INT
  typedef int NT;
#elif defined _NT_LONG
  typedef long NT;
#elif defined _NT_RAT
   typedef BigRat NT;
#elif defined _NT_EXP
  typedef Expr NT;
#elif defined _NT_BF
  typedef BigFloat NT;
#else
  typedef BigInt NT;	//Default 
#endif

typedef Polynomial<NT> PolyNT;
typedef Sturm<NT> SturmNT;


// ==================================================
// Test function
// ==================================================
int testPseudo(PolyNT A, PolyNT B, bool verbose=false) {
  NT C;
  PolyNT Rem = A;  		
  if (verbose) cout << "===============begin testPseudo===========" << endl;
  PolyNT Quo = Rem.pseudoRemainder(B, C);
  if (verbose) {
  cout << "A = " << A << endl;
  cout << "B = " << B << endl;
  cout << "Quo = " << Quo << endl;
  cout << "Rem = " << Rem << endl;
  cout << "C = " << C << endl;
  }
  A.mulScalar(C);	// A = A * C;
  PolyNT pp = Quo * B  - Rem;
  if (verbose) {
  cout << "C*A = " << A << endl;
  cout << "Quo*B - Rem = " << pp << endl;
  }
  if (A == pp)
    cout << "CORRECT! (C*A) == (Quo*B - Rem) in pseudoRemainder" << endl;
  else
    cout << "ERROR!   (C*A) != (Quo*B + Rem) in pseudoRemainder" << endl;
  if (verbose) cout << "===============end testPseudo===========" << endl;
  return 1;
}

int testingSturm(PolyNT A) {
    // constructor from polynomial
    A.makeTailCoeffNonzero();	// No reason to have zero tail coefficients
    int ell = A.getTrueDegree();
    assert(ell > 0);	// No reason to form Sturm object for degree <=0
    // (Too many special cases otherwise)
    PolyNT * seq = new Polynomial<NT> [ell+1];
    seq[0] = A;
cout << "testingSturm: seq[0] = ";  seq[0].mapleDump();
    seq[1] = differentiate(A);
cout << "testingSturm: seq[1] = ";  seq[1].mapleDump();
    int i;
    for (i=2; i <= ell; i++) {
      seq[i] = seq[i-2];
      seq[i].pseudoRemainder(seq[i-1]);
cout << "testingSturm: seq[" << i << "] = ";  seq[i].mapleDump();
      if (zeroP(seq[i]))
        break;
      seq[i].primPart(); // Primitive part is important to speed
      // up large polynomials! However, for first 2 polymials,
      // we DO NOT take primitive part, because we
      // want to use them in Newton Iteration
    }
    int len = i;
    return len;
  }

// ==================================================
// Main
// ==================================================

int main(int argc, char** argv){
  
    // SET INTERNAL PRECISION
    //setDefaultRelPrecision(CORE_posInfty); // This puts defAbsPrec in control
    int prec = 60;			// Do Sturm refinement till delta
					// is at most 2^{-prec}
    if (argc >= 2)
         prec = atoi(argv[1]);		// if user specifies

    setDefaultAbsPrecision(prec);

    // bool slowTest = false;		// slowTest is not used!
    // if (argc >= 3) 
    //	slowTest = true;		// if user specifies precision
  
    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits
    
    cout << "------------------------------------------------------------\n";
    cout << "Relative Precision is " << defRelPrec << endl;
    cout << "Print Digits is " << cout.precision() << endl;
    cout << "Iteration Stops when delta < " << BigFloat::exp2(-prec) << endl;
    //  cout << setprecision(defPrtDgt.asLong()); // already set above.
    cout << "------------------------------------------------------------\n";



    ////////////////////////////////////////////////////////////
    //  Some extra polynomials
    ////////////////////////////////////////////////////////////
    // NT cs1[] = {-2, 0, 1};  	// sqrt of 2
    // PolyNT P0(2, cs1);
    // NT cs[] = {-23, 17, 0, -4, 0, 13};
    // PolyNT P(5, cs);
    ////////////////////////////////////////////////////////////

    
    cout <<"=============================================================\n"
      << "TEST 1:  Polynomial of degree 16 with 6 real roots\n" <<
      "=============================================================" << endl;
    NT cs[] = {-113, 0, 224, 0, 200, 0, -184, 0, -98, 0, 64, 0, 8, 0, -8, 0, 1};
    PolyNT P(16, cs);
    // Show Polynomial:
    cout << "Polynomial is ";  P.mapleDump();
    // Show its root bound:
    cout << " Separation bound = " << sepBound(P) << endl;
    // Test One Root we know:
    Expr root = sqrt(1+sqrt(5-3*sqrt(1+sqrt(Expr(2)))));
    cout << " sqrt(1 + sqrt(5 - 3*sqrt(1 + sqrt(2)))) = " << root << endl;
    if (eval(P, root) == 0)
      cout << "The preceding is a root (CORRECT!)" << endl;
    else
      cout << "The preceding is not a root (ERROR!)" << endl;
    
    // Test all roots
    testNewtonSturm(P, prec, 6);
    cout <<"=============================================================\n"
      << "TEST 2 :  Polynomial X^2 + 1, no roots! \n" <<
      "=============================================================" << endl;
    NT cs1[] = {1, 0, 1};
    PolyNT P1(2, cs1);	// P1 = X^2 + 1, so has no roots!
    testNewtonSturm(P1, prec, 0);

    cout <<"=============================================================\n"
      << "TEST 3:   Legendre Polynomial 35 X^4 - 20 X^2 + 3 \n"
      "=============================================================" << endl;
    // Legendre : 4 real roots between [-1;1]
    NT cs2[] = {3, 0, -30, 0, 35};
    PolyNT P2(4, cs2);
    testNewtonSturm(P2, prec, 4);
    
// The next test will fail in NT=int or NT=long, so we omit it.
#ifndef _NT_LONG
#ifndef _NT_INT
#ifndef _NT_EXP
    cout <<"=============================================================\n"
      << "TEST 4:   Mignotte Polynomial,  P(X) = X^20 - 2(100X-1)^2 \n" <<
      "=============================================================" << endl;
    // Mignotte's family of polynomials are P(X) = X^n - 2(aX-1)^2
    // where n>2 and a>2 are integers.
    //    The polynomial is irreducible, and has two real roots which
    //    are very close to 1/a.  In fact, they
    //    they are at most 2a^{-(n+2)/2} apart.
    // See [Yap's Book, page 173, Exercise 6.7.3]
    // For our example here, n=20, a=100, we see that the two roots
    //	  are very close to 1/100 = 0.01.  In fact, the 2 real roots
    //	  are within the interval 0.01 +/- 10^{-22}.  This can be
    //	  seen in the output.
    PolyNT P3(20);
    P3.setCoeff(20, 1); 
    P3.setCoeff(0, -2); 
    P3.setCoeff(1, 400); 
    P3.setCoeff(2, -20000); // P3 = X^20 - 2(100 X-1)^2
    testNewtonSturm(P3, prec, 4);
    // REMARK: there is another variation used in the Frisco Suite.
    // They define A(X)= X^n+(aX+1)^m. They used a complex value for a,
    // e.g.,  a=100i.   If we use a real value a=100
    // then a polynomial like X^20 + (100 X+1)^2 has no real roots.


#endif
#endif
#endif

    cout <<"=============================================================\n"
      << "TEST 5:   Roots of Unity,  P(X) = X^99 - 1 \n" <<
      "=============================================================" << endl;
    PolyNT P4(99);
    P4.setCoeff(99, 1);
    P4.setCoeff(0, -1);
    testNewtonSturm(P4, prec, 1);

    cout <<"=============================================================\n"
      << "TEST 6:   Wilkinson Polynomial of degree 6 (roots are 1,..,6)\n" <<
      "=============================================================" << endl;
    NT cs5[] = {720, -1764, 1624, -735, 175, -21, 1};
    PolyNT P5(6, cs5);
    testNewtonSturm(P5, prec, 6);

    cout <<"=============================================================\n"
      << "TEST 6a:   Perturbed Wilkinson6 (add 1 to constant coeff)\n" 
      << "   NOTE:   Roots are more sensitive to perturbations in higher degree coeffs\n" <<
      "=============================================================" << endl;
    NT cs6[] = {721, -1764, 1624, -735, 175, -21, 1};
    PolyNT P6(6, cs6);
    testNewtonSturm(P6, prec, 6);

// The next test will fail in NT=int or NT=long, so we omit it.
#ifndef _NT_LONG
#ifndef _NT_INT
    cout <<"=============================================================\n"
      << "TEST 6b:   Perturbed Wilkinson6 (add 1 to coeff of X^2)\n" <<
      "=============================================================" << endl;
    NT cs7[] = {720, -1764, 1625, -735, 175, -21, 1};
    PolyNT P7(6, cs7);
    testNewtonSturm(P7, prec, 2);

    cout <<"=============================================================\n"
      << "TEST 6c:   Perturbed Wilkinson6 (add 1 to coeff of X^5)\n" <<
      "=============================================================" << endl;
    NT cs8[] = {720, -1764, 1624, -735, 175, -20, 1};
    PolyNT P8(6, cs8);
    testNewtonSturm(P8, prec, 2);
#endif
#endif

    cout <<"=============================================================\n"
      << "TEST 7:   Wilkinson Polynomial of degree 10 (roots are 1,..,10)\n" 
      << "       	(THIS USED TO BE SLOW, BUT NO LONGER)\n" <<
      "=============================================================" << endl;
    NT cs9[] = {3628800, -10628640, 12753576, -8409500, 3416930, -902055,
	    		157773, -18150, 1320, -55, 1};
    PolyNT P9(10, cs9);
    SturmNT SS9(P9);
    BFInterval I = SS9.isolateRoot(1);
    cout << " The smallest root lies in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;
    I = SS9.isolateRoot(3);
    cout << " The 3rd smallest root lies in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;
    I = SS9.isolateRoot(-1);
    cout << " The largest root lies in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;
    I = SS9.isolateRoot(-3);
    cout << " The 3rd largest root lies in the interval"
	    	<< I.first << ", " << I.second << "]" << endl;
    I = SS9.isolateRoot(11);
    cout << " The 11th smallest root lies in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;
    cout << " 		It is undefined, and so prints as [1,0]" << endl;
    I = SS9.isolateRoot(-11);
    cout << " The 11th largest root lies in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;
    cout << " 		It is undefined, and so prints as [1,0]" << endl;

   
    ////////////////////////////////////////////////////////////
    //  Special Cases
    ////////////////////////////////////////////////////////////
    // multiple roots
    cout <<"=============================================================\n"
      << "SPECIAL TESTS: Polynomials with multiple roots " << endl;
    // to be done!!!
    
    // linear polynomial
    cout <<"=============================================================\n"
      << "SPECIAL TESTS: Linear Polynomial " << endl;
    NT cs11[] = {-2, 1};  
    PolyNT P11(1, cs11);
    SturmNT SS11(P11);
    I = SS11.isolateRoot(1);
    cout << " The first root of X-2 is in the interval ["
	    	<< I.first << ", " << I.second << "]" << endl;

#ifndef _NT_LONG
#ifndef _NT_INT
    // Polynomial with zero tail coefficient
    cout <<"=============================================================\n"
      << "SPECIAL TESTS: Polynomial with Zero Tail Coefficients" << endl ;
    PolyNT P12 = P6;		// P6 is perturbed Wilkinson with 6 roots
    P12.mulXpower(7);		// now tail coeff is zero, i.e., 0 is a root
    assert(P12.getTrueDegree() == 7+P6.getTrueDegree());
    				// P12 has been transformed
    testNewtonSturm(P12, prec, 7); // This should have ONE MORE ROOT than P6
#endif
#endif
    // Polynomial with roots of multiplicity greater than one
    cout <<"=============================================================\n"
      << "SPECIAL TESTS: Polynomial with  roots of multiplicity greater than one " << endl ;
    // p(x) = x^4 - 8*x^3 + 8*x^2 + 32*x + 16 
    //Roots are roughly -0.843 and 4.828 each of multiplicity 2
    NT cs14[]={16, 32, 8, -8, 1};
    PolyNT P14(4, cs14);
    testNewtonSturm(P14, prec, 2);


   ////////////////////////////////////////////////////////////
    // ANOTHER SPECIAL TEST: A(X)= X^3 + 3X^2 = (X+3)X^2
NT cs13[] = {0, 0, 3, 1};
PolyNT P13(3,cs13);
SturmNT SS13(P13);

    ////////////////////////////////////////////////////////////

    cout <<"=============================================================\n"
      << "                END TEST \n" << endl;
#if defined _NT_INT
   cout << "Number type NT = int" << endl;
#elif defined _NT_LONG
   cout << "Number type NT = long" << endl;
#elif defined _NT_RAT
   cout << "Number type NT = BigRat" << endl;
#elif defined _NT_EXP
   cout << "Number type NT = Expr" << endl;
#elif defined _NT_BF
   cout << "Number type NT = BigFloat" << endl;
#else
   cout << "Number type NT = BigInt" << endl;
#endif
    cout <<"=============================================================" << endl;
    
    return 0;
  }// main
