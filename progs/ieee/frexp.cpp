/*
 * File: frexp.cpp
 *
 * Purpose:	To understanding the format of IEEE doubles
 *      via two "standard" library functions frexp and ldexp.
 *
 *      We view a IEEE double d *      as the product,
 *
 *              d = Mantissa(d) * 2^{Exponent(d)}.
 *
 *      Here Mantissa(d) is a 53 bit binary number of the form
 *      (0.1xxx...xxx)_2 where there are 52 x's.  The value of
 *      this mantissa, therefore, is a number in the range [0.5,1.0).
 *
 * 	Let d = double and e = int.  We look at two instructions:
 *
 *		double t = ldexp(d, e);  // (A)
 *	        double m = frexp(d, &e); // (B)
 *
 *      In (A), ldexp(d,e) returns the double value (d * 2^e).
 *      In other words, this performs a left-shift of d by e.
 *      Read "ldexp" as "load exponent".
 *
 *	In (B), frexp(d,&e) will set e to Exponent(d),
 *      and returns the normalized mantissa as m.
 *      Thus, after executing the statement (B), we can
 *      verify the results:
 *
 *              if (d == ldexp(m, e))
 *                    cout << "CORRECT!" << endl;
 *              else
 *                    cout << "ERROR!" << endl;
 *		
 *	For instance, if d=0.5, then e=0, m=0.5;
 *	              if d=1.0, then e=1, m=0.5;
 *	              if d=2.0, then e=2, m=0.5.
 *	              if d=7.0, then e=3, m=0.875;
 *
 *      From the identity d = ldexp(m,e), we also get
 *
 *                    m = ldexp(d, -e).       // (C)
 *
 *      In the BigFloat class, we need to convert a machine
 *      double into BigFloats which uses integer mantissas
 *      (not normalized mantissas).   So, if we define
 *
 *      	IntMantissa(d) := m * 2^{53}
 *
 *      then IntMantissa(d) will be an integer.  Thus,
 *
 *              d = IntMantissa(d) * 2^{-53+Exponent(d)} .
 *
 *      From this equation, it is also natural to define
 *
 *              IntExponent(d) := Exponent(d) - 53
 *
 *      Thus, we have
 *              
 *              d = IntMantissa(d) * 2^IntExponent(d).
 *
 *
 * author: Chee Yap
 * Since Core Library 1.7
 * $Date: 2006/03/07 04:51:24 $
 *************************************************************/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ieeefp.h>

using namespace std;

// implements the integer mantissa function
double IntMantissa(double d) {
	int e;
	return ldexp(frexp(d, &e), 53);
}

// implements the integer exponent function
int IntExponent(double d) {
	int e;
	frexp(d, &e);
	return e-53;
}

int main(int argc, char ** argv){

	cout << setprecision(17);
	double d = 1.0;
	int e = 1;
	if (argc <= 1) {
		cout << "Input a double value: ";
		cin  >> d;
	} else {
		d = atof(argv[1]);  // atof = ascii to float!
		                    // we assume this will convert to double
	}
	if (argc > 2) {
		cin  >> e;
	}
	cout << "###################################################" << endl;
	int n;
	double m = frexp(d, &n); // m is mantissa
	cout << "Exponent of " << d << " is n = " << n << endl;
	cout << "The normalized matissa of " << d << " is = "
		        << ldexp(d, -n) << endl;
	cout << "This should be the same as " << m << endl;
	cout << "The mantissa as an integer is = " 
		        << ldexp(d, 53-n) << endl; 
	cout << "The IntMantissa is = " << IntMantissa(d) << endl;
	cout << "The IntExponent is = " << IntExponent(d) << endl;
	double tmp= IntMantissa(d) * ldexp(1.0, IntExponent(d)); 
	if (tmp == d)
	   cout << "CORRECT! (IntMantissa(d) * 2^IntExponent(d)) == d)" << endl;
	else
	   cout << "ERROR!!! (IntMantissa(d) * 2^IntExponent(d)) != d)" << endl;

	cout << "###################################################" << endl;
	cout << "For comparison, this is 2^{52}   = "
		        << ldexp(1.0, 53-1) << endl;
	cout << "For comparison, this is 2^{53}   = "
		        << ldexp(1.0, 53) << endl;

	d += ldexp(1.0, -52+n);	
	cout << "The next larger number is  d+ 2^{-52+d.exp}  = " << d << endl;

	cout << "This is 2^e    = " << ldexp(1.0, e) << endl;
	cout << "This is 2^(-e) = " << ldexp(1.0, -e) << endl;

	//////////////////////////////////////////////////
	int N = 100000;
	//////////////////////////////////////////////////
	int count = N;
	for (int i=1; i<N; i++) {
	    d = ldexp(double(i),-10); // convert i to value i*2^{-10}
	    m = frexp(d,&e);
            if (d != ldexp(m, e)) {
                cout << "ERROR! at d = " << d << endl;
	    } else count--;
	}
	if (count == 1)
		cout << "CORRECT! " << N << " times" << endl;
		
	//////////////////////////////////////////////////
	// Let us generate the largest integer representable
	// by an IEEE double with exponent 0, i.e.,
	//     D = sum_{i=0}^52 2^i
	//////////////////////////////////////////////////
	double D = 1;
	for (int i=1; i<53; i++)
		D += ldexp(1.0, i);
	cout << "Largest integer with exponent 0 is D = " << D << endl;
	double DD = D + ldexp(1.0, 53);
	cout << "Next larger integer with exponent 0 is DD = " << DD << endl;
	cout << "[Note: DD = D + 2^{53}, but is inexact because of overflow]"
		<< endl;
	// I cannot quite explain the actual error here
	d = ldexp(D,-53);
	double dd = ldexp(DD,-54);
	cout << "Normalized value of D is   d = " << d << endl;
	cout << "Normalized value of DD is dd = " << dd << endl;
	cout << "1.0 - d = " << 1.0-d << endl;
	cout << "1.0 - dd = " << 1.0-dd << endl;

	cout << "Let us add 1 to D: " << (D+1.0) << "(OK!)" << endl;
	cout << "Let us add 2 to D: " << (D+2.0) << "(???)" << endl;
	return 0;

/***************************************************
 * Here is an output from this program:
 
frexp 4
###################################################
Exponent of 4 is n = 3
The normalized matissa of 4 is = 0.5
This should be the same as 0.5
The mantissa as an integer is = 4503599627370496
For comparison, this is 2^{52}   = 4503599627370496
For comparison, this is 2^{53}   = 9007199254740992
The next larger number is  d+ 2^{-52+d.exp}  = 4.0000000000000018
This is 2^n    = 8
This is 2^(-n) = 0.125
CORRECT! 100000 times
Largest integer with exponent 0 is D = 9007199254740991
Next larger integer with exponent 0 is DD = 18014398509481984
[Note: DD = D + 2^{53}, but is inexact because of overflow]
Normalized value of D is   d = 0.99999999999999989
Normalized value of DD is dd = 1
1.0 - d = 1.1102230246251565e-16
1.0 - dd = 0
Let us add 1 to D: 9007199254740992 (OK!)
Let us add 2 to D: 9007199254740992 (???)
***************************************************/

}
