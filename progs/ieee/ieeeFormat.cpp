/************************************************************
 * file: ieeeFormat.ccp
 * author: Zilin Du and Chee Yap
 * synopsis:	Various questions about format of IEEE machine numbers
 *
 *		-- checks if your machine is Big Endian or Little Endian 
 *		      E.g., solaris is Little Endian and Pentium is Big Endian 
 *		-- format of special constants (+/- Infinity, NaN, etc)
 *		-- size of various machine number types
 *
 * usage: 
 *		% g++ ieeeFormat.cc -o ieeeFormat
 *		% ./ieeeFormat
 *
 * date: July 26, 2001
 * Since Core Library Version 1.4
 * $Id: ieeeFormat.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

// We must use unions to avoid dirty casts, to avoid aliasing problems.
//  (see option -fstrict-aliasing of GCC)
union float_uint {
  float f;
  unsigned int ui;
};
union double_ulong {
  double d;
  struct {
    unsigned long ul1, ul2;
  } ul;
};

int main() {

  // float

  float_uint f_ui1, f_ui2;
  f_ui1.f = 0.0;
  f_ui2.f = 0.0;

  // double

  double_ulong d_ulx, d_uly, d_ulz;
  d_ulx.d = 0.0;
  d_uly.d = 1;
  d_ulz.d = 0.0;

  // int
	int xi = 1;

  // long
  	long xl = 1;

#ifndef _MSC_VER
  // long long (Visual C++ doesn't support long long)
  	long long xll = 1;
#endif

  // bool
	bool bigEndian;

  // Endian tests
	cout << "*********************** ENDIAN TESTS *******************\n";
	cout << "N.B. Solaris is Little Endian, Pentium is Big Endian.\n";
	char *pi = (char *)&xi;
	if ( (*pi) == 1 ) {
		cout << "This is Big Endian int!" << endl;
		bigEndian = true;
	} else {
		cout << "This is a Little Endian int!" << endl;
		bigEndian = false;
	}

	char *pl = (char *) &xl;
	if ( (*pl) == 1 )
		cout << "This is a Big Endian long!" << endl;
	else
		cout << "This is a Little Endian long!" << endl;

#ifndef _MSC_VER
	char *pll = (char *) &xll;
	if ( (*pll) == 1 )
		cout << "This is a Big Endian long long!" << endl;
	else
		cout << "This is a Little Endian long long!" << endl;
#endif

  // Size tests
	cout << "*********************** SIZE TESTS *********************\n";
	
	cout << "size of an bool = " << sizeof(bool) << endl;
	cout << "size of an char = " << sizeof(char) << endl;
	cout << "size of an int = " << sizeof(int) << endl;
	cout << "size of an unsigned int = " << sizeof(unsigned int) << endl;
	cout << "size of a long = " << sizeof(long) << endl;
	cout << "size of an unsigned long = " << sizeof(unsigned long) << endl;
#ifndef _MSC_VER
	cout << "size of a long long = " << sizeof(long long) << endl;
#endif
	cout << "size of a float = " << sizeof(float) << endl;
	cout << "size of a double = " << sizeof(double) << endl;
	
  // Signed Zero
	cout << "*********************** SIGNED ZERO ********************\n";
	cout << "positive double zero: " << d_ulx.d << endl;
	cout << "positive double zero: " << -d_ulx.d << endl;

	// The following may appear a little surprising at first:
	if (d_ulx.d == (-d_ulx.d)) 
		cout << "CORRECT(!) : positive zero = negative zero ";
	else
		cout << "ERROR(!) : positive zero != negative zero ";
	cout << endl;

  // Infinity
	cout << "*********************** INFINITY ***********************\n";
	f_ui2.f = 0.0; d_ulz.d = 0.0;
	f_ui2.ui = f_ui2.ui | 0xff800000L;	// -infinity float

unsigned long *pzd = &d_ulz.ul.ul1;
	if (bigEndian)    pzd++;
	(*pzd) = (*pzd) | 0xfff00000L;  // -infinity double

	cout << "Negative Infinity: float = " << f_ui2.f << ", double = " << d_ulz.d << endl;

	f_ui2.f = 0.0; d_ulz.d = 0.0;
	f_ui2.ui = f_ui2.ui | 0x7f800000L;	// +infinity float
	(*pzd) = (*pzd) | 0x7ff00000L;	// +infinity double
	cout << "Positive Infinity: float = " << f_ui2.f
		<< ", double = " << d_ulz.d << endl;
	cout << "double 1.0/0.0 = " << d_uly.d/d_ulx.d << endl;
	cout << "double 1.0/(-0.0) = " << d_uly.d/(-d_ulx.d) << endl;

	if (d_ulz.d == (d_uly.d/d_ulx.d))
		cout << "CORRECT! Have constructed positive infinity" << endl;
	else
		cout << "ERROR! Failed to construct positive infinity" << endl;

  // NaN
	cout << "*********************** SIGNED NaN **********************\n";
	cout << "Here are some ways to get NaN" << endl;

	f_ui2.f = 0.0; d_ulz.d = 0.0;
	f_ui2.ui = f_ui2.ui | 0x7f800001L;	// positive NaN 
	(*pzd) = (*pzd) | 0x7ff00001L;	// positive NaN
	cout << "  Positive NaN: float = " << f_ui2.f 
		<< ", double = " << d_ulz.d << endl;

	f_ui2.f = 0.0; d_ulz.d = 0.0;
	f_ui2.ui = f_ui2.ui | 0xfff00000L;	// negative NaN 
	(*pzd) = (*pzd) | 0xfff10000L;	// negative NaN 
	cout << "  Negative NaN: float = " << f_ui2.f
		<< ", double = " << d_ulz.d << endl;

	cout << "N.B. On Solaris, the next 4 divisions only produce +NaN's.\n";
	cout << "N.B. On Pentium, the next 4 divisions only produce -NaN's.\n";
	cout << "  double 0.0/0.0 : " << d_ulx.d/d_ulx.d << endl;
	cout << "  float 0.0/0.0 : " << 0.0/f_ui1.f << endl;
	cout << "  float -0.0/0.0 : " << -0.0/f_ui1.f << endl;
	cout << "  double 0.0/-0.0 : " << d_ulx.d/(-d_ulx.d) << endl;
	return 0;
}
