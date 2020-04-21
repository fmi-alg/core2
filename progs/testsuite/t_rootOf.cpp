/***************************************************
 * file: t_rootOf.cpp
 *
 * Input Polynomial is
 * 				poly =  x^50 - 50 x^2 + 20x - 2
 * Its four real roots are 
 *	 			-1.09254, 0.2, 0.2, 1.07565
 *
 * We test the rootOf(..) constructs:
 *
 * 		Expr x = rootOf(poly, n=0)
 * 					-- the n-th real root
 * 		Expr x = rootOf(poly, BFinterval bfi)
 * 					-- the unique real root in bfi
 * 		Expr x = rootOf(poly, BigFloat left, BigFloat right)
 * 					-- the unique real root in [left,right]
 *
 * using 3 versions of poly:
 * 		Polynomial<BigRat>
 * 		Polynomial<BigInt>
 * 		Polynomial<BigFloat>
 * 
 *
 * Between g++4.9.3 and g++5.4.0,
 * 	rootOf(...) gave "rootOf was not declared" error.
 * 	It was because rootOf is a friend, and it did
 *
 * 		
 * Chee Yap (2010, 2017)
 * Core Library
 *
 ***************************************************/
#define CORE_NO_AUTOMATIC_NAMESPACE
#ifdef CORE_LEVEL
#undef CORE_LEVEL
#define CORE_LEVEL 4
#endif

#include <CORE/CORE.h>
#include <CORE/poly/Poly.h>
#include <iostream>

using namespace CORE;
using namespace std;


// diag(e, cor, err):
//	prints either CORRECT! or ERROR! diagnostic message
bool diag(Expr e, string correctMsg, string errorMsg){
    if (e == 0) {
	cout << correctMsg << endl;
	return true;
    }
    cout << errorMsg << endl;
    return false;
}


int main()
{

   // ======================================
   // Experiment 1, for Polynomial<BigRat>:
   // ======================================
   BigRat c[51];
   c[0]= BigRat(-2);
   c[1]= BigRat(20);
   c[2]= BigRat(-50);
   c[50]= BigRat(1);
   for (int k=3;k<50;++k) c[k]=0;

   Polynomial<BigRat> bigRat_poly(50, c);

   cout << "Polynomial is    x^{50} - 50 x^2 + 20 x - 2" << endl;
   cout << "It has 4 real roots: " << endl;
   cout << "       -1.09254,   0.2,    0.2,    1.07565 " << endl;


   cout << "<BigRat> Sturm Sequence produces these intervals: " << endl;
   Sturm<BigRat> const bigRat_sturm(bigRat_poly);

   int nb_roots=bigRat_sturm.numberOfRoots(-100, 100);
   diag (Expr(nb_roots - 4),
       "CORRECT! Sturm<bigRat> found 4 real roots",
       "ERROR! Sturm<bigRat> found wrong number of real roots");

   Expr result[nb_roots];	// vector of all roots

   for (int k=1;k<=nb_roots;++k){
     const BFInterval bfi_sturm = bigRat_sturm.isolateRoot(k, -100, 100);
     cout << "  -- Interval [" << bfi_sturm.first << ", "
	 		<< bfi_sturm.second << "]" << endl;
     result[k-1] = rootOf(bigRat_poly, bfi_sturm);
     cout <<  "          has " << k << "th Root = "<< result[k-1] << endl;
   }

   // ======================================
   // Repeat the above for Polynomial<BigInt>:
   // ======================================
   BigInt cc[51]; 	// cc cannot be const
   cc[0]= BigInt(-2);
   cc[1]= BigInt(20);
   cc[2]= BigInt(-50);
   cc[50]= BigInt(1);
   for (int k=3;k<50;++k) cc[k]=0;

   // Next, we need a "const" here polynomial:
   //
   Polynomial<BigInt> const bigInt_poly(50, cc);

   Sturm<BigFloat> new_sturm(bigInt_poly);

   cout << "<BigInt> Sturm Sequence produces these intervals: " << endl;
   Sturm<BigInt> const bigInt_sturm(bigInt_poly);

   nb_roots = bigInt_sturm.numberOfRoots(-100, 100);
   diag (Expr(nb_roots - 4),
       "CORRECT! Sturm<bigInt> found 4 real roots",
       "ERROR! Sturm<bigInt> found wrong number of real roots");
   
   Expr bigInt_res;
   string cor = "CORRECT! Sturm<BigInt> and Sturm<BigRat> agree on ";
   string err = "ERROR! Sturm<BigInt> and Sturm<BigRat> disagree on ";

   for (int k=1;k<=nb_roots;++k){
	bigInt_res = rootOf(bigInt_poly, k);
	diag(bigInt_res - result[k-1], 
        	cor + numberToString(k) + "-th root",
		err + numberToString(k) + "-th root");
   }

   BFInterval const bfi_1 = new_sturm.isolateRoot(1, -100, 100);
   Expr res1 = rootOf(bigInt_poly, bfi_1);
   cout <<  "1st Root = " << res1 << endl;


   return 0;
}

