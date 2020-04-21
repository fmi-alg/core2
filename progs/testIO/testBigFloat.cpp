/* ************************************************
   File: testBigFloat.cpp

   Purpose:  Tests I/O for BigFloat and BigFloat2

   		Strings can be in scientific or positional notation

   Usage:
        % testBigFloat		-- non-verbose output
        % testBigFloat 1	-- verbose output

   Author: Jihun and Chee Yap

   Since Core Library 2.0
   $Id: testBigFloat.cpp,v 1.3 2010/06/25 09:04:03 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

using namespace CORE;

//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

  // Global unit test variables:
  coretest_error=false;
  coretest_verbose=false;

  if (argc>1) coretest_verbose = true;
  
  // get exponent:
  BigFloat f("0"); 		// f=0
  core_test(0, f.get_exp(), "f.get_exp() is wrong");
  			// REMARK: this is really only a convention.
  f = BigFloat("1"); 		// f=1
  core_test(1, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("0.5"); 		// f=1/2
  core_test(0, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("0.125"); 	// f=1/8
  core_test(-2, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("1.125");	 // f=9/8
  core_test(1, f.get_exp(), "f.get_exp() is wrong");
  
  // get mantissa:
  f = BigFloat("31459");
  BigInt z; int exp;
  exp = f.get_z_exp(z);
  
  if(exp >= 0)
    z *= power(BigInt(2), exp);
  else
    z /= power(BigInt(2), -exp);

  core_test("31459", z.get_str(), "f.get_z_exp() is wrong");
  //
  
  // BigFloat2 string input:
  BigFloat2 f2("0.0007891", 10); 
  core_test(".0007891", f2.get_str(7), "BigFloat string input is wrong");
  //
  
  f = "0.0625";
  f2 = BigFloat2(f);
  core_test(".0625", f2.get_str(), "BigFloat2.get_str is wrong");

  if (coretest_error == false)
    cout << "CORRECT!!! all test are passed" << endl;

  return 0;
}//main
