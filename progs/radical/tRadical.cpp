/*************************************************************
 * File: tRadical.cpp
 * Purpose:
 * 	Verify that the basic functions of radical(n,m) which
 * 	constructs the m-th root of n is working.
 *
 * Usage:   
 * 		> ./tRadical [n=3] [m=9] [outputPrec=40] [prec=1000]
 *
 * 	where the default values are shown above.
 * 	If n<0, then we do a battery of self-tests and
 * 	verify that the result is correct.
 *
 *
 * Restriction/Bugs:
 * 	n must be an natural number (BigInt or int)
 * 	m must be >= 1
 * 	
 * 	If m is too big, this test will be really slow.
 * 	Currently, 
 * 	        > ./tRadical 123 30
 * 	is reasonably "real time" but
 * 	        > ./tRadical 123 40
 * 	takes about 2 minutes on a Pentium 1.4 GHz laptop.
 * 	Time is somewhat less sensitive to large n.
 *
 * NOTES: for self-validation, we use 250 digits from Maple 12:
 * 	3^(1/3) = 1.44224957030740838232163831078010958839186925349935 (50 digits)
 * 		    05775464161945416875968299973398547554797056452566 (next 50)
 * 		    86835080854489549966425423946110259714868950157185 (next 50)
 * 		    23722709033202384759844506108554002726008814549887 (next 50)
 * 		    27513673553524678660747156884392233189182017038998 (next 50)
 * 	3^(1/9) = 1.12983096390975303261216610428284187448400389383684
 * 		    96298931512015411060336812007954723720002658571205
 * 		    32082673223153887533945197182006223674973675575212
 * 		    14396671746882536797174599367288416929813534350734
 * 		    76582756160743084554714435603463528627387019996833
 *
 * Author: Chee Yap, Vikram Sharma, Jihun Yu
 * Date:  May 22, 2003; Jan 6, 2010 (Core 2)
 *
 * Since Core Library version 1.6
 *************************************************************/

#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

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


//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {
  int outputPrec; prec_t Prec;
  int eps;
  
  NT n = 3;		// default value of n
 
  // set_cut_off_bound(1000);  THIS WAS FOR DEBUGGING
 
  if (argc > 1) {
    n = atoi(argv[1]);
  }

  int m = 3;		// default value of m
  if (argc > 2) {
    m = atoi(argv[2]);
  }

  outputPrec =  40;	// default output precision (in bits)
  if (argc > 3) {
    outputPrec = atoi(argv[3]);
  }

  Prec = 1000;		// default relative precision for evaluation (in bits)
  if (argc > 4) {
    Prec = atoi(argv[4]);
  }

  // cube Root of 3 to 250 digits (from Maple 12):
  string cubeRootOfThree = "1.4422495703074083823216383107801095883918692534993505775464161945416875968299973398547554797056452566868350808544895499664254239461102597148689501571852372270903320238475984450610855400272600881454988727513673553524678660747156884392233189182017038998";

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
 
  setDefaultOutputDigits(outputPrec);

  // SELF-VALIDATION:
  Expr mthrootn = radical(3,3);
  mthrootn.approx(1000);		
  if (isCompatible(mthrootn.toString(),cubeRootOfThree))
	cout << "CORRECT! CubeRoot(3) has been validated to at most 250 digits" << endl;
  else {
    cout << "CORE  : " << mthrootn.toString() << endl;
	cout << "Maple : " << cubeRootOfThree << endl;
	cout << "ERROR! CubeRoot(3) has been invalidated with at most 250 digits" << endl;
  }
  // end SELF-VALIDATION:


  if (n >= 0) {
     Expr mthrootn = radical(n,m);
     mthrootn.approx(Prec);
     cout << "The " << m << "-th root of "<< n << " is "
	     << mthrootn.approx() << endl;
   
     Expr p = power(mthrootn, m);
     cout << "The " << m << "-th power of " << mthrootn << " is " << endl;
     cout << "    P = " << p.approx() << endl;
   
     if(p == Expr(n))
       cout << "CORRECT! P is equal to " << n << endl; 
     else
       cout << "ERROR! P is NOT equal to " << n << endl; 
     
  } else { // if n < 0, do the automatic test
     Expr r, rr;
     if ((radical(NT(1), 1000) == 1) && (radical(NT(1), 1) ==1)) 
	   cout << "CORRECT! for radical(1,k), k=1, 1000" << endl;
	else 
	   cout << "ERROR! for radical(1,k), k=1, 1000" << endl;
     if ((radical(NT(2), 1) == 2) && (radical(NT(12345), 1) == 12345)) 
	   cout << "CORRECT! for radical(m,1), m=2, 12345" << endl;
	else 
	   cout << "ERROR! for radical(m,1), m=2, 12345" << endl;
     for (int i=1; i<11; i++) {
        r = radical(NT(2), i);
	rr = radical(NT(789), i);
	if ((power(r,i) != 2) || (power(rr,i) != 789)){
	   cout << "ERROR! for radical(m,i), (m=2,789), i = " << i << endl;
	} else {
	   cout << "CORRECT! for radical(m,i), (m=2,789), i = " << i << endl;
	}
     }
     int p=7; int q=30;
     for (int i=1000; i<1011; i++) {
        r = radical(NT(i), p);
	rr = radical(NT(i), q);
	if ((power(r,p) != i) || (power(rr,q) != i)){
	   cout << "ERROR! ";
	} else {
	   cout << "CORRECT! ";
	}
	cout << " for radical(i,k), (k=" << p <<
		", " << q << "), i = " << i << endl;
    }
/*
#ifdef _NT_BF
     BigFloat bf(283243346, 0, -4);
     BigFloat bf2(1234, 0, -5);

     for (int i=0; i<100; i++) {
	     bf += bf2;
	     Expr e = radical(bf, 2);
	     Expr p = power(e, 2);
	     cout << "bf=" << bf << endl;
	     cout << "e=" << e << endl;
	     cout << "p=" << p << endl;
	     if (Expr(bf) == p) {
		     cout << "CORRECT!" << endl;
	     } else {
		     cout << "ERROR!" << endl;
	     }
     }
#endif
*/  }
}
