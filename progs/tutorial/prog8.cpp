/* ************************************************
   File: prog8

   NOTE: this program is no longer supported in Core2.

   Purpose:  Automatic conversion from RealInt to 
             RealBigInt in case of overflow while
	     operating with RealInts. However the 
	     conversion is not done for int's (signed
	     or unsigned)
   Usage: 

        % prog8

   Author: Chee Yap, Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog8.cpp,v 1.2 2010/05/18 10:34:13 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;

int main(int argc , char ** argv) {

 Real X, Y, Z;
 unsigned int x, y, z;
 int xx, yy, zz;

 X = 1; // X is of type RealInt
 x = 1; // x is unsigned int
 xx = 1; // xx is int

 Y = 4294967295;  // $ 2^{32}-1 $, RealInt
 		  // Compiler may give a warning here, but OK!
 y = 4294967295;  // $ 2^{32}-1 $, uint
 		  // Compiler may give a warning here, but OK!
 yy = 2147483647; // $ 2^{31}-1 $, int

 Z = X + Y;
 z = x + y;
 zz = xx + yy;

 cout.precision(30);
 cout << "RealInt Z = X + Y,           unsigned int z = x + y,         int zz = xx + yy :" << endl;
 cout << "(X, Y, Z) = (" << X << ", " << Y << ", " << Z << ")" << endl;
 cout << "(x, y, z) = (" << x << ", " << y << ", " << z << ")" << endl;
 cout << "                   Overflow in the unsigned int addition!" << endl;
 cout << "(xx, yy, zz) = (" << xx << ", " << yy << ", " << zz << ")" << endl;
 cout << "                   Overflow in the signed int addition!" << endl;

}


