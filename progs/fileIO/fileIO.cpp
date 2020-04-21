/* ************************************************
   File: fileIO.cpp

   Purpose:  To exercise the file input and output facilities
      for big numbers.  

      For more information about the file format, see the file
      "bigNumFileFormat" in this directory.
      June, 2001 Note: Only the big integer  facilities in this format
      has been implemented.

      Functions provided:

   (1) void (const BigInt& z, istream is) 

       -- this constructs a BigInt z from file, reading at most
       maxlen number of digits (of the appropriate base).
       If maxLen=0, then read the entire file.

   (2) void writeToFile(const BigInt& z, std::ostream os, int b = 10)

       -- this writes the BigInt value to file os, in base b.  Each
       output line is 80 characters.

   (3) int BigInt::set_str(const char* s, int b = 0)

        -- this reads a string s (in the "base number format") in base b
       into a BigInt.  If b=0, then the base is directly
       determined by the leading characters in string s as follows:
       "0x" or "0X" for hex, "0" for octal, "0b" or "0B" for bin,
       null for decimal 

   (4) std::string BigInt::get_str(int b = 10)

       -- inverse of the previous function.
       -- Remark: formerly, this was getString(int b=10)

   Usage:
        % fileIO

   Author: Zilin Du (zilin@cs.nyu.edu), Jihun Yu(jihun@cs.nyu.edu)

   $Id: fileIO.cpp,v 1.2 2006/11/16 17:41:03 exact Exp $
 ************************************************ */

#include <fstream>
#include "CORE.h"

using namespace std;

  ////////////////////////////////////////////////////////////
  // This method is NOT tested in this file???  Please fix it!
  //
  // 	BigInt::fromString(const char* s, int b = 0)
  // 	
  ////////////////////////////////////////////////////////////
 
  
int main (int argc, char **argv)
{
  ifstream inFile;
  ofstream outFile;
  BigInt product, test;

  // Compute n! for n = 500. 
  // Note: if you change the value of n, you should change the 
  //	size of char array of str to hold the result.
  int n = 500;
  product = 1;
  
  for (int j=1; j<=n; j++)
    product *= j;
  
  ////////////////////////////////////////////////////////////
  // print out the result.  
  ////////////////////////////////////////////////////////////
  string s = product.get_str();
  cout << "The result of " << n << "! = " << s << "\n";
        
  ////////////////////////////////////////////////////////////
  // read/write BigInt in 4 different bases.
  ////////////////////////////////////////////////////////////
  int bases[] = {2, 8, 10, 16};
  const char* filename[] = {"bin.big", "octal.big", "dec.big", "hex.big"};
  
  for (int i=0; i < 4; i++ ) {
    cout << "\nWrite the result to the file "
		<< filename[i] << " in base = " << bases[i] <<"...\n";
    outFile.open(filename[i]);
    writeToFile(product, outFile, bases[i]);
    outFile.close();
    outFile.clear();
    
    cout << "Read the result from the file " << filename[i] << "...\n";
    inFile.open(filename[i]);
    if (inFile.good()) {
    readFromFile(test, inFile);
    } else {
      cout << "ERROR" << endl;
    }
    inFile.close();
    inFile.clear();
    
    if ( test == product )
      cout << "CORRECT! write and read from file" << endl;
    else 
      cout << "ERROR! write and read from file" << endl;
  }

  return 0;
}
