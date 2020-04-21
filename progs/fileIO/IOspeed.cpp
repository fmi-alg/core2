/* ************************************************
   File: IOspeed.cc

   Purpose:  To test the speed of reading/writing in decimal and
       hex format for big numbers.  

       For more information about the file format, see the file
       "bigNumFileFormat" in this directory.

       [June, 2001: Only the big integer  facilities in this format
       has been implemented.]

   Usage:
        % IOspeed

   Core Library
   Author: Zilin Du (zilin@cs.nyu.edu)

   $Id: IOspeed.cpp,v 1.2 2006/11/20 19:47:59 exact Exp $
 ************************************************ */

#include <fstream>
#include <ctime>
#include "CORE.h"
using namespace std;

int main (int argc, char **argv)
{
  ifstream inFile;
  ofstream outFile;
  BigInt product, test;

  // compute n! for n = 20000. 
  // Note: if you change the value of n, you should change the 
  //	size of char array of str to hold the result.
  int n = 20000;
  product = 1;
  
  cout << "computing " << n << "!...\n";
  for (int j=1; j<=n; j++)
    product *= j;
  
  // print out the result.  
  // string s = product.toString();
  // cout << "The result of " << n << "! = " << s << '\n';
        
  // read/write BigInt in 4 different bases.
  int bases[] = {2, 8, 10, 16};
  const char* filename[] = {"bin.big", "octal.big", "dec.big", "hex.big"};
  
  machine_long t1=0, t2=0;
       
  for (int i=2; i < 4; i++ ) {
    cout << "\nWrite the result to the file " 
	<< filename[i] << " in base = " << bases[i] <<"...\n";
    outFile.open(filename[i]);
    t1 = clock();

    writeToFile(product, outFile, bases[i]);
    t2 = clock();
    outFile.close();
    outFile.clear();
    cout << "Done (Time spent: "
	    << (float)(t2-t1)/CLOCKS_PER_SEC << " seconds)\n\n";
    
    cout << "Read the result from the file " << filename[i] << "...\n";
    inFile.open(filename[i]);
    t1 = clock();
    readFromFile(test, inFile);	// maxLength = 0 means the length is
					// determined by length field in file.
    t2 = clock();
    inFile.close();
    inFile.clear();
    cout << "Done (Time spent: "
	    << (float)(t2-t1)/CLOCKS_PER_SEC << " seconds)\n\n";
    
    if ( test == product )
      cout << "---<<CORRECT!>>---\n";
    else 
      cout << "---<<ERROR!>>---\n";
  }
  return 0;
}
