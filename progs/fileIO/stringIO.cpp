/* ************************************************
   File: stringIO.cpp

   Purpose:  To exercise the conversions facilities
      for big numbers from/to strings.  

      Functions provided:

   (1) For the BigInt class
   
       int BigInt::set(const char* s, int b = 0)
       int BigInt::set(const string s, int b = 0)

        -- this reads a string s (in the "base number format") in base b
       into a BigInt.  If b=0, then the base is directly
       determined by the leading characters in string s as follows:
       "0x" or "0X" for hex, "0" for octal, "0b" or "0B" for bin,
       null for decimal 

       string BigInt::get_str(int b = 10)

       -- inverse of the previous function.

   (2) For the BigFloat class

       void set(const char* s, int b = 10)

        -- This reads a string s into a BigFloat in base b

       string get_str(size_t n = 0, int b = 10, rnd_t rnd=MPFR_RND) 

       -- Inverse of the previous function

   Usage:
        % stringIO

   Author: Joaquin Grech (jg568@nyu.edu)

   $Id: stringIO.cpp,v 1.2 2006/11/10 21:08:05 exact Exp $
 ************************************************ */

#include <fstream>
#include "CORE.h"

using namespace std;

int main (int argc, char **argv)
{
  BigInt product, test;
  BigFloat fproduct, ftest;

  // Compute n! for n = 500. 
  int n = 500;
  product = 1;

  cout << "Testing string conversion for BigInt class" << endl;
  cout << "------------------------------------------\n\n";

  cout << "Computing 500! (Factorial)" << endl;
  
  for (int j=1; j<=n; j++)
    product *= j;
  
  // print out the result.
  cout << "The result of the BigInt printed by cout " << product << "\n\n";
  string s = product.get_str();
  cout << "The result of BigInt.get_str() " << s << "\n";
  
  test.set_str(product.get_str().c_str(), 10);
  if (test.get_str().compare(product.get_str()) == 0)
	  cout << "CORRECT! Both strings are equal" << endl;
  else
	  cout << "ERROR!!! Strings are not equal" << endl;

  cout << "Testing base conversion: (Convert from binary to decimal and viceversa)" << endl;
  product.set_str("11111", 2);
  cout << "The result of BigInt.fromString(\"11111\",2) is: " << product << endl;
  cout << "It should print: 31." << endl;
  
  if (product == 31)
	  cout << "CORRECT! base conversion is fine" << endl;
  else
	  cout << "ERROR!! base conversion is wrong!" << endl;

  cout << "The result of BigInt(31).get_str(2) is: " << product.get_str(2) << endl;
  cout << "It should print: 11111." << endl;
  
  if (product.get_str(2).compare("11111")==0)
	  cout << "CORRECT! BigInt get_str is OK" << endl;
  else
	  cout << "ERROR!!! BigInt get_str is wrong" << endl;

  cout << "\nTesting string conversion for BigFloat class" << endl;
  cout << "--------------------------------------------\n\n";
  cout << "Computing 500!/3 " << endl;

  setDefaultPrecision(120, CORE_posInfty);

  fproduct=test/3;

    // print out the result using scientific format
  cout.setf(ios::scientific, ios::floatfield);
  cout << "Result of BigFloat printed by cout " << fproduct << "\n\n";
  cout << "Result of BigFloat.get_str() " << fproduct.get_str() << "\n";
  
  ftest.set(fproduct.get_str().c_str());
  cout << "Result of ftest " << ftest << "\n\n";
  cout << "Result of ftest.get_str()    " << ftest.get_str() << "\n";
  if (ftest.get_str().compare(fproduct.get_str()) == 0)
     cout << "CORRECT! Bigfloat::tostring conversions is OK" << endl;
  else
     cout << "ERROR!!! Bigfloat::tostring conversions is wrong" << endl;

  cout.setf(ios::fixed, ios::floatfield);
  cout << "\nNOT printing in scientist format:\n";
  cout << "The result of the BigFloat printed by cout " << fproduct << "\n\n";
  cout << "The result of BigFloat.get_str() " << fproduct.get_str() << "\n";
  
  ftest.set(fproduct.get_str().c_str());
  cout << "Result of ftest.get_str()        " << ftest.get_str() << "\n";
  if (ftest.get_str().compare(fproduct.get_str()) == 0)
     cout << "CORRECT!  BigFloat toString (non-scientific format) OK" << endl;
  else
     cout << "ERROR!!!  BigFloat toString (non-scientific format) bad" << endl;
 
  return 0;
}
