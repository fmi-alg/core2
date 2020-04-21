/* **************************************
 File: tPoly.cpp
 	$Id: tPoly.cpp,v 1.12 2012/04/13 15:24:00 exact Exp $
 Purpose: basic testing of the polynomial class

 USAGE:

	If an optional argument [InteractiveMode] is given,
	the mode is interactive.
	(the user will need to type some inputs on keyboard)
	The value of [InteractiveMode] is not examined.

 NOTE: This program compiled with NT=BigInt as default.
 	But if you define one of the variables
		_NT_INT, _NT_RAT, _NT_EXP
	then you get (respectively)
		NT=int, NT=BigRat, NT=Expr.
 	We would like this to compile with NT=BigFloat as well...

 Author: Chee Yap 
 Date:   May 28, 2002. (Revised Mar 24, 2004)
 
 Since Core Library v1.4.1
 ************************************** */ 

#define CORE_LEVEL 4
#include "CORE/CORE.h"

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
#elif defined _NT_BIGINT
  typedef BigInt NT;
#else
  typedef BigInt NT;
#endif

typedef Polynomial<NT> Poly;

using namespace std;

int testPseudo(Poly A, Poly B, bool verbose=false) {
  NT C;
  Poly Rem = A;  		
  if (verbose) cout << "===============begin testPseudo===========" << endl;
  Poly Quo = Rem.pseudoRemainder(B, C);
  if (verbose) {
  cout << "A = " << A << endl;
  cout << "B = " << B << endl;
  cout << "Quo = " << Quo << endl;
  cout << "Rem = " << Rem << endl;
  cout << "C = " << C << endl;
  }
  A.mulScalar(C);	// A = A * C;
  Poly pp = Quo * B  + Rem;
  if (verbose) {
  cout << "C*A = " << A << endl;
  cout << "Quo*B + Rem = " << pp << endl;
  }
  if (A == pp)
    cout << "CORRECT! (C*A) == (Quo*B + Rem) in pseudoRemainder" << endl;
  else
    cout << "ERROR!   (C*A) != (Quo*B + Rem) in pseudoRemainder" << endl;
  if (verbose) cout << "===============end testPseudo===========" << endl;
  return 1;
}

int main( int argc, char* argv[] ) {

////////////////////////////////////////////////////////////
// ARGUMENT PROCESSING
////////////////////////////////////////////////////////////
  bool InteractiveMode = false;
  if (argc > 1) InteractiveMode = true;

  cout << " ================ START OF TEST POLY ================ " << endl;

cout << "================================================\n";
cout << "CONSTRUCTORS \n";
cout << "================================================\n";

  Poly zP;
  cout << "Zero Poly (zP) :\n>  " << zP << endl;

  if (zP != Poly::Zero()) 
     cout << "ERROR: either Zero() or zP is not the zero polynomial" << endl;
  else
     cout << "CORRECT: Zero() = zP" << endl;

  Poly oneP(0);
  cout << "Unity Poly (oneP) :\n>  " << oneP << endl;

  if (oneP != Poly::Unity()) 
     cout << "ERROR: either Unity() or oneP is not the unity polynomial"
		<< endl;
  else
     cout << "CORRECT: Unity() = oneP" << endl;

  Poly U(5);
  cout << "Another Unit Poly U :\n>  " << U << endl;
  
  NT c[] = {1, 2, 3, 4, 5};
  Poly P4(4, c);
  cout << "P4 :\n>   " << P4 << endl;


  Poly Q(P4);
  cout << "Q, copy of P4 :\n>   " << Q << endl;

cout << "================================================\n";
cout << "DUMPING POLYNOMIALS (normal and Maple) \n";
cout << "================================================\n";

  cerr << "cerr:>  Dumping polynomials to cerr: " << endl;
  cout << "cout:>  Dumping polynomials to cerr: " << endl;
  P4.dump(">This is P4 (output to cerr): ");

  cout << ">  Dumping to cerr : U (unit poly):" << endl;
  U.dump();

  cout << ">  Next is a maple Dump of polynomial P4:" << endl;
  P4.mapleDump(); // no (string) arguments
  cout << "\n Note: mapleDump does not take any arguments..." << endl;

cout << "================================================\n";
cout << "EXPAND, CONTRACT, REVERSE, MakeTailCoeffNonzero, NEGATE \n";
cout << "================================================\n";

  int n = Q.expand(3);		// Q is copy of P4
  cout << "Null extension of Q :\n>  n =" << n << " (should be -2) " << endl;
  if (n != -2) cout << "ERROR!! Q.expand did not return -2" << endl;
  cout << ">  Unchanged  Q :\n>  " << Q << endl;

  n = Q.expand(7);
  cout << "Expansion of Q :\n>  n =" << n << " (should be 7) " << endl;
  cout << ">  New  Q :\n>  " << Q ;
  cout << ">  getTrueDegree(Q) = " << Q.getTrueDegree()
	<< " (should be 4)\n" ;

  n = Q.contract();
  cout << "Reduce Q :\n> n = " << n << " (should be 4) " << endl;
  cout << ">  This should be the original Q:\n>  " << Q << endl;
  n = Q.contract();
  if (n != -2) cout << "ERROR!! Q.contract did not return -2" << endl;
  else cout << "Nothing to reduce in Q :\n> n = " << n
	<< " (CORRECT! n = -2) " << endl;

  cout << ">  Original P4 :\n>  " << P4 ;
  cout << ">  Original Q :\n>  " << Q ;
  Q.reverse();
  cout << ">  Reversed  Q :\n>  " << Q ;
  Q.reverse();		// we want them to be equal, so Q needs to
  cout << ">  2nd Reversed  Q :\n>  " << Q ;
			// be reversed again (it was reversed once above)
  if (Q == P4)   cout << "CORRECT! reverse(reverse)=ident" << endl;
  else		 cout << "ERROR! reverse(reverse) != ident" << endl;

  Q.negate();
  cout << ">  Negated  Q :\n>  " << Q ;
  Q.negate();
  if (Q == P4)   cout << "CORRECT! negate(negate)=ident" << endl;
  else		 cout << "ERROR! negate(negate) != ident" << endl;

  assert(P4.getTailCoeff() != 0);	// needed for the next test
  for (int i = 0; i<25; i += 3) {
  	P4.mulXpower(i);	// multiply by X^17
  	if (P4.makeTailCoeffNonzero() == i)
	  cout << "CORRECT! makeTailCoeff is correct" << endl;
	else	
	  cout << "ERROR! makeTailCoeff has error" << endl;
  }
cout << "================================================\n";
cout << "ASSIGNMENT \n";
cout << "================================================\n";

  Poly P5;
  P5 = P4;
  cout << "Assignment P5=P4 :  Here is P5\n>  " << P5 << endl;

  NT c5[] = {-1, 2, -3, 4, -5, 6};
  P5 = Poly(5, c5);
  cout << "Updated P5 :\n>   " << P5 << endl; 
    
cout << "================================================\n";
cout << "ARITHMETIC \n";
cout << "================================================\n";

  Poly S = P5 + P4;
  cout << "Addition, S = P5 + P4 :\n>   " << S << endl;
  
  S = P5 - P4;
  cout << "Difference, S = P5 - P4 :\n>   " << S << endl;
  
  // Multiplication by Scalar
  NT x = 10;
  S.mulScalar(x);
  cout << "Scalar Multiply of S * 10 :\n>   " << S << endl;

  // Multiplication by X-power
  S.mulXpower(0);
  cout << "S mulXpower(0) :\n>   " << S << endl;
  S.mulXpower(3);
  cout << "S mulXpower(3) :\n>   " << S << endl;
  S.mulXpower(-4);
  cout << "S mulXpower(-4) :\n>   " << S << endl;

  // Unary minus
  cout << "Unary Minus, -S :\n>   " << -S << endl;

  // Product
  Poly P = P5 * P4;
  cout << "Product, P = P5 * P4 :\n>   " << P << endl;

  // POWER
  cout << "Power, P5^0 :\n>   " << power(P5,0) << endl;
  cout << "Power, P5^1 :\n>   " << power(P5,1) << endl;
  cout << "Power, P5^2 :\n>   " << power(P5,2) << endl;
  cout << "Power, P5^3 :\n>   " << power(P5,3) << endl;

cout << "================================================\n";
cout << "REMAINDERS \n";
cout << "================================================\n";

  Poly P7(P4);
  P7.mulXpower(3);	// raise to power x^7
  cout << "P7 :\n>   " << P7 << endl;
  NT c2[] = {2, 0, 3};
  Poly P2(2, c2);
  cout << "P2 :\n>   " << P2 << endl;

  P7.pseudoRemainder(P2);	// Now P7 is the pseudoRemainder
  cout << "P7.pseudoRemainder(P2) :\n>   " << P7 << endl;
  NT c7[] = {8, 22};
  Poly p7(1, c7);	// p7 = 22x + 8
  if (P7 == p7) 
    cout << "CORRECT! 22x+8 is pseudoRemainder" << endl;
  else
    cout << "ERROR! 22x+8 is NOT the pseudoRemainder" << endl;

  ////////////////////////////////////////////
  testPseudo( P4, P5);
  testPseudo( power(P4,3), P2);
  NT c3[] = {1, -3, 0, 2};
  Poly P3 = Poly(3, c3);
  testPseudo( power(P4,3), P3);
  testPseudo( power(P4,3), P4);
  testPseudo( power(P4,6), power(P4,5));
  ////////////////////////////////////////////
  
  Poly P20 = power(P4,5);
  Poly P0 = P20;  
  Poly P26 = P0.pseudoRemainder(P4); // P0 should be zero
  Poly p26 = power(P4,4);
  cout << "P26 = " << P26 << endl;
  cout << "p26 = " << p26 << endl;
  if (p26 == P26)
    cout << "CORRECT! pseudoQuotient of (P4)^5 divided P4" << endl;
  else
    cout << "ERROR! pseudoQuotient of (P4)^5 divided P4" << endl;
  if (P0 == Poly::Zero()) 
    cout << "CORRECT! pseudoRemainder of (P4)^5 divided P4 is 0" << endl;
  else
    cout << "ERROR! pseudoRemainder of (P4)^5 divided P4 is not 0" << endl;

cout << "================================================\n";
cout << "EVALUATION \n";
cout << "================================================\n";

  cout << "Evaluation, of P5 :\n>   P5(BigFloat(0)) = " << eval(P5, BigFloat(0));
  cout << ">   P5(Expr(1)) = " << eval(P5, Expr(1)) << endl; 
  cout << ">   P5(Expr(2)) = " << eval(P5, Expr(2)) << endl; 
  Expr a=eval(P5, Expr("1.2"));
  cout << ">   P5(Expr(\"1.2\")) = " << a << endl; 
  Expr b=eval(P5, Expr(1.2));
  cout << ">   P5(Expr(1.2)) = " << b << endl; 
  cout << ">   Is P5(Expr(\"1.2\")) == P5(Expr(1.2)) ? " ;
  if (a == b) cout << " Yes (ERROR!)" << endl;
  else cout << " No (CORRECT!)" << endl << endl;

cout << "================================================\n";
cout << "DIFFERENTIATION \n";
cout << "================================================\n";

  cout << "Differentiate P5 zero times:\n>  P5 = "
	<< differentiate(P5,0) << endl;
  cout << "Differentiation, of P5 :\n>  P5' = " << differentiate(P5) << endl;
  cout << "Differentiate twice :\n>  P5'' = " << differentiate(P5,2) << endl;
  cout << "Differentiate thrice :\n>  P5''' = " << differentiate(P5,3) << endl;

cout << "================================================\n";
cout << "COMPARISON\n";
cout << "================================================\n";

  cout << "Testing for Zero Polynomial: \n>   " ;
  if (zeroP(zP)) cout << "CORRECT! zP is the zero polynomial" << endl;
  else cout << "ERROR! zP should be the zero polynomial" << endl;

  cout << "Testing for Unit Polynomial: \n>   " ;
  if (unitP(oneP)) cout << "CORRECT! oneP is the unit polynomial" << endl;
  else cout << "ERROR! zP should be the unit polynomial" << endl;

  cout << "Comparing P5 and P4: \n>   " ;
  if (P5 == P4) cout << "ERROR! they are equal " << endl;
  if (P5 != P4) cout << "CORRECT! they are not equal " << endl;
 
cout << "================================================\n";
cout << "Testing parsing of polynomials from string input \n";
cout << "================================================\n";

  Poly P10("1 - 2 X + 3 X^2 - 4 X^3  + 5 X^4");
  cout << "P10 = " << P10 << endl;
  if (P10.getCoeff(0)==1 && P10.getCoeff(1)== -2 &&
  	P10.getCoeff(2)==3 && P10.getCoeff(3)== -4 && P10.getCoeff(4)==5)
      cout << "P10 is CORRECT!" << endl;
  else cout << "P10 is in ERROR!" << endl;
  // This polynomial is rather confusingly 
  // stringified, note that the last term is 5X
  // and this polynomial is actually
  // 1 + 3X + 3X^2 - 4X^3.
  Poly P11("1 - 2 X + 3 X^2 - 4 X^3  + 5 X");
  cout << "P11 = " << P11 << endl;
  Poly P12("(1 - 2 X + 3 X^2)(2 + 3 X)");
  cout << "P12 (coeffs should be 2,-1,0,9) = " << P12 << endl;
  if (P12.getCoeff(0) == 2 && P12.getCoeff(1)== -1 &&
  	P12.getCoeff(2)==0 && P12.getCoeff(3)== 9)
      cout << "P12 is CORRECT!" << endl;
  else cout << "P12 is in ERROR!" << endl;
#if defined _NT_BF
  Poly P13("1.1 - 2.2 X + 3.3 X^2 - 4.4 X^3 + 5 X^4");
  cout << "P13 = " << P13 << endl;
  if ((P13.getCoeff(2) - 3.3) == 0)
      cout << "P13 is CORRECT!" << endl;
  else
    cout << "P13 is in ERROR!" << endl;
#endif
#if defined _NT_RAT
  Poly P14("1/1 - 2/2 X + 3/4 X^2 - 5/6 X^3");
  cout << "P14 = " << P14 << endl;
  if ((P14.getCoeff(2)*4 - 3) == 0)
      cout << "P14 is CORRECT!" << endl;
  else cout << "P14 is in ERROR!" << endl;
#endif

  Poly P15("X");
  cout << "P15 (should be =X) = " << P15 << endl;
  Poly P16("0");
  cout << "P16 (should be 0 polynomial) = " << P16 << endl;

cout << "================================================\n";
cout << "STREAM I/O \n";
cout << "================================================\n";

  if (InteractiveMode) {
    cout << "Stream Input : type degree d, then d+1 coefficients\n>   "
	 << endl;
    Poly Pin;
    cin >> Pin;
    cout << "Is this your input polynomial? \n>   " << Pin << endl; 
  }

cout << "================================================\n";
cout << "COEFFICIENT MANIPULATION \n";
cout << "================================================\n";
  cout << "\nGet coefficient array of P4" << endl;
  const NT * cp = P4.getCoeffs();
  int deg = P4.getDegree();
  for (int i=0; i<=deg; i++)
	cout << "    " << i << "-th Coefficient = " << cp[i] << endl;
	
  cout << "Get each coefficients of P4" << endl;
  for (int i=0; i<=5; i++) {
	if (i <=  deg) {
	  const NT cp = P4.getCoeff(i);
	  cout << "    " << i << "-th Coefficient = " << cp << endl;
	}
  }
  
  cout << "Change the 5th coefficients of P4 to 100" << endl;
  NT hundert = 100;
  if (P4.setCoeff(5, hundert)) 
	cout << "    ERROR!  Should have failed" << endl;
  else
	cout << "    CORRECT!  Failed to change" << endl;

  cout << "Change the 3th coefficients of P4 to 100" << endl;
  if (P4.setCoeff(3, hundert)) 
	cout << "    Here is the new P4:\n>  P4 = " << P4 << endl;
  else
	cout << "    ERROR!  Should have succeeded" << endl;

cout << "================================================\n";
cout << "Testing Resultants (N.B. Correct up to sign only!!) \n";
cout << "================================================\n";


  NT r = res(P10,P11);
  // TODO: Change the test suite for _NT_INT and _NT_LON
  // To calculate a resultant that does not overflow.
#if defined(_NT_INT) || defined(_NT_LON)
  cout << "NOTE : res(P10, P11) as calculated by this test is " << r << " but this is incorrect." << endl
       << "It should have been 13500, but this calculation overflows at this precision level" << endl;
#else
  if (r == 13500) {
    cout << "CORRECT!! res(P10,P11) = 13500" << endl;
  } else {
    if (r == -13500) {
      cout << "CORRECT ONLY UP TO SIGN!! res(P10,P11) = -13500, not 13500" << endl;
    } else {
      cout << "ERROR!! res(P10,P11) != +/-13500" << endl;
      cout << "res(P10,P11) is alleged to be " << r << endl;
    }
  }
#endif

  Poly PP("2X^2 -X+1");
  Poly QQ("3X -2");
  r = res(PP,QQ);
  if (r == 11) cout << "CORRECT!! res(PP,QQ) = 11" << endl;
  else {
   if (r == -11) 
     cout << "CORRECT ONLY UP TO SIGN!! res(PP,QQ) = -11, not 11" << endl;
   else {
   cout << "ERROR!! res(PP,QQ) != +/-11" << endl;
   PP.dump("Dumping PP");
   QQ.dump("Dumping QQ");
   cout << "res(PP,QQ) is alleged to be " << r << endl;
   }
  }

cout << "================================================\n";
cout << "Testing Discriminants \n";
cout << "================================================\n";

  Poly P30("X^2 - X -1");	// disc(P30)= b^2 - 4ac = 5
  cout << "P30 = " << P30 << endl;
  cout << "disc(P30) = " << disc(P30) << endl;
  if (disc(P30) == 5) cout << "CORRECT!! disc(P30) = 5" << endl;
  else cout << "ERROR!! disc(P30) != 5" << endl;

  Poly P31("X^2 - 2X + 1");	// disc(P31)= b^2 - 4ac = 0
  cout << "P31 = " << P31 << endl;
  cout << "disc(P31) = " << disc(P31) << endl;
  if (disc(P31) == 0) cout << "CORRECT!! disc(P31) = 0" << endl;
  else cout << "ERROR!! disc(P31) != 0" << endl;
  
  Poly P32("2X^2 - X -1");	// disc(P32)= 2b^2 - 4ac = 9
  cout << "P32 = " << P32 << endl;
  cout << "disc(P32) = " << disc(P32) << endl;
  if (disc(P32) == 9) cout << "CORRECT!! disc(P32) = 9" << endl;
  else {
    if (disc(P32)== P32.getLeadCoeff()*NT(9))
	cout << "CORRECT up to extraneous leading coefficient, OK" << endl;
    else 
    	cout << "ERROR!! disc(P32) != 9 (even up to extraneous constant factor)" << endl;
  }

cout << "================================================\n";
cout << "Testing Polynomial GCD (see also tGCD.cpp) \n";
cout << "================================================\n";

  NT cs1[] = {-45, 390, -1260, 2040, -2115, 1350};
  Polynomial<NT> G1(5, cs1);
 
  NT cs2[] = {3, -2, 5};
  Polynomial<NT> G2(2, cs2);

  NT cs3[] = {9, -6, 15};
  Polynomial<NT> G3(2, cs3);

  NT cs4[] = {15, -9, 6};
  Polynomial<NT> G4(2, cs4);

  NT cs5[] = {5};
  Polynomial<NT> G5(0, cs5);
  
  Polynomial<NT> G, zero;


  Polynomial<NT> C;
  C = G2*G3;	//  So, gcd(G2,C)=G2;
  if (gcd(C,G2) != G2)
	  cout << "ERROR!!!  gcd(A*B,A) != A" << endl;
  else
	  cout << "CORRECT!  gcd(A*B,A) = A" << endl;
  if (gcd(G3,C) != G3)
	  cout << "ERROR!!!  gcd(B,A*B) != B" << endl;
  else
	  cout << "CORRECT!  gcd(B,A*B) = B" << endl;

  Polynomial<NT> D(G2);
  D.setCoeff(0,4);
  if(gcd(G2,C) == D) 
    cout << "ERROR!!!  D is considered gcd" << endl;
  else
    cout << "CORRECT!  D is not the gcd" << endl;
  


// ==================================================
// ADDITIONAL TESTS 
// ==================================================
/*
#if defined _NT_BF
   Poly BF_R(1);
      BF_R.setCoeff(0, BigFloat(123, 0, -4));  
      BF_R.setCoeff(1, BigFloat(321, 0, 4));  	
      cout << "BF_R: " << BF_R << endl;
   Poly BF_Q(2);
      BF_Q.setCoeff(0, BigFloat(2, 0, -1));
      BF_Q.setCoeff(1, BigFloat(3, 0, 2));
      BF_Q.setCoeff(2, BigFloat(4, 0, -3));
      cout << "BF_Q: " << BF_Q << endl;
   Poly BF_B(3);
      BF_B.setCoeff(0, BigFloat(12, 0, -1));
      BF_B.setCoeff(1, BigFloat(3, 0, 2));
      BF_B.setCoeff(2, BigFloat(4, 0, -3));
      BF_B.setCoeff(3, BigFloat("12345678901234567", 0, -3));
      cout << "BF_B: " << BF_B << endl;

  Poly BF_A = BF_B * BF_Q + BF_R;
     cout << "BF_A: " << BF_A << endl;
  testPseudo(BF_A, BF_B, true);
  // ==================================================
//   NT C;
//   Poly BF_P = BF_A.pseudoRemainder(BF_B, C);
//      cout << "BF_P = pseudoQuotient of BF_A by BF_B: " << BF_P << endl;
//      cout << "PseudoRemainder of BF_A by BF_B: " << BF_A << endl;
//      cout << "Constant of pseudoRemainder is " << C << endl;
//   if (BF_A == BF_R) 
//      cout << "CORRECT! PseudoRemainder is same as BF_R" << endl;
//   else
//      cout << "ERROR!!! PseudoRemainder is different from BF_R" << endl;
//   if (BF_P == BF_Q) 
//      cout << "CORRECT! PseudoQuotient is same as BF_Q" << endl;
//   else
//      cout << "ERROR!!! PseudoQuotient is different from BF_Q" << endl;
//   BF_A = BF_B * BF_Q + BF_R;

  // ==================================================
  BF_A.setCoeff(5, BigFloat(1234, 0, 2));
  testPseudo(BF_A, BF_B, true);

#endif
*/
  cout << " ============ END OF TEST Polynomial<NT> =============== " << endl;

#if defined _NT_INT
  cout << " ================= NT = int ========================= " << endl;
#elif defined _NT_EXP
  cout << " ================= NT = Expr ======================== " << endl;
#elif defined _NT_RAT
  cout << " ================= NT = BigRat ====================== " << endl;
#elif defined _NT_LON
  cout << " ================= NT = long ======================== " << endl;
#elif defined _NT_BF
  cout << " ================= NT = BigFloat ======================== " << endl;
#else
  cout << " ================= NT = BigInt ====================== " << endl;
#endif

  return 0;
}//main

// ==================================================
// End of tPoly
// ==================================================
