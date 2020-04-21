/* **************************************
   File: heron.cc

   Purpose: To compute the area of a triangle using 
	Heron's formula.  
	We use a series of 12 test inputs from a paper
	of W. Kahan (see README file for details).
   Usage:
	% heron [opt-prec] [opt-printDgt]

	where [opt-prec] is the optional guaranteed precision 
	and [opt-printDgt] is the optional output precision.
	Default is opt-prec = 54 (as in machine double)
	and opt-printDgt = 11 (comparable to Kahan).

   Core Library 
   $Id: heron.cpp,v 1.2 2006/11/27 23:30:35 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE.h"

// ==================================================
// To compute area of triangle using Heron's formula
// NOTE: if bad input, return -1.
// ==================================================

double heron(const char* aa, const char* bb, const char* cc) {
  double a = BigFloat(aa);
  double b = BigFloat(bb);
  double c = BigFloat(cc);
  double s = (a+b+c)/2;

  if ((s<a) || (s<b) || (s<c))
	return (-1); // error input
  else
	return sqrt(s*(s-a)*(s-b)*(s-c));
}

// ==================================================
// Test Routine
// ==================================================
void test(int n, const char* a, const char* b, const char* c, const char* k) {
  double A = heron(a,b,c);
  std::cout << "------------------- Problem " << n << " --------------------------\n" ;
  std::cout << "(a,b,c) = (" << a << ", " << b << ", " << c << ")\n" ;
  std::cout << "   Kahan's Area = " << k << std::endl;
  std::cout << "   Core's Area  = ";
  if (A == -1) 
	std::cout << "Invalid Input Detected\n";
  else
	std::cout << heron(a,b,c) << "\n" ;
}

int main( int argc, char *argv[] ) {

  if (argc > 3) {
    std::cerr << "Usage: heron [optional_precisionBits] [optional_printDigits]"
	<< "\n          Default values: heron 54 11\n";
    exit(1);
  }

// --------------------------------------------------------------------------------------
// Initialize precision for computing and display 
// --------------------------------------------------------------------------------------

  if (argc >= 2) {
	setDefaultRelPrecision(atoi(argv[1]));	// if user specifies precision
  } else
  	setDefaultRelPrecision(54); 	// otherwise, defRelPrec = 54
					// i.e., as in machine arithmetic 

  if (argc >= 3) {
	std::cout.precision(atoi(argv[2]));	// if user specifies output precision
  } else
  	std::cout.precision(11); 	// otherwise, defPrtDgt = 11
					// to compare against Kahan's values


  double a, b, c;
 
// ======================================================================================
// TABLE FROM KAHAN
// No	a		b		c		Naive		Kahan
// ======================================================================================
// 1 	10  		10  		10 		43.30127019 	43.30127020 
// --------------------------------------------------------------------------------------
// 2 	-3		5  		2  		2.905		Error 
// --------------------------------------------------------------------------------------
// 3	100000		99999.99979 	0.00029 	17.6		9.999999990
// --------------------------------------------------------------------------------------
// 4	100000		100000		1.00005		50010.0		50002.50003 
// --------------------------------------------------------------------------------------
// 5	99999.99996	99999.99994	0.00003		Error		1.118033988 
// --------------------------------------------------------------------------------------
// 6	99999.99996	0.00003		99999.99994	Error		1.118033988 
// --------------------------------------------------------------------------------------
// 7	10000		5000.000001	15000		0		612.3724358 
// --------------------------------------------------------------------------------------
// 8	99999.99999	99999.99999	200000		0		Error 
// --------------------------------------------------------------------------------------
// 9	5278.64055	94721.35941	99999.99996	Error		0 
// --------------------------------------------------------------------------------------
// 10	100002		100002		200004		0		0 
// --------------------------------------------------------------------------------------
// 11	31622.77662	0.000023	31622.77661	0.447		0.327490458 
// --------------------------------------------------------------------------------------
// 12	31622.77662	0.0155555	31622.77661	246.18		245.9540000 
// ======================================================================================


  std::cout << "----------------------------------------------------------------------\n";
  std::cout << "Relative Precision is " << defRelPrec << std::endl;
  std::cout << "Print Digits is " << std::cout.precision() << std::endl;
//  std::cout << setprecision(defPrtDgt.asLong()); // already set above.
  std::cout << "----------------------------------------------------------------------\n";

  test(1, "10",		"10",		"10",		"43.30127020");
  test(2, "-3",		"5",		"2",		"Invalid (but expected)");
  test(3, "100000",	"99999.99979",	"0.00029",	"9.999999990");
  test(4, "100000",	"100000",	"1.00005",	"50002.50003");
  test(5, "99999.99996", "99999.99994", "0.00003",	"1.118033988");
  test(6, "99999.99996","0.00003",	"99999.99994",	"1.118033988");
  test(7, "10000",	"5000.000001",	"15000",	"612.3724358");
  test(8, "99999.99999", "99999.99999", "200000",	"Invalid (but expected)");
  test(9, "5278.64055",	"94721.35941",	"99999.99996",	"0");
  test(10, "100002",	"100002",	"200004",	"0");
  test(11, "31622.77662", "0.000023",	"31622.77661",	"0.327490458");
  test(12, "31622.77662", "0.0155555",	"31622.77661",	"245.9540000");

  std::cout << std::flush;
  
  return 0;
}

