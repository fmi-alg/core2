/*************************************************************
 * bigRand.cpp
 *
 * purpose:
 *  	Since rand() gives you up to 2^{32} bits.
 *  	we want to generate 53 bit random integers
 *  	which can be stored in a machine double.
 *
 *  	So we want a function "bigRand()" which computes
 *
 *  	         (rand()*2^{21} + (rand()>>11)).
 *
 * usage:
 *
 * 	> random3 [N=1000] [xtraBits=2] [make_pseudo_random]
 *
 * 	All arguments are optional.  If there is a third argument,
 * 	then the random seed will be a fixed one, not random.
 *
 * 	The program will generate N bigRand's, and verify that
 * 	they all fit inside a machine double!
 *
 * author: Chee Yap, August 2004
 * Since Core Library 1.7
*************************************************************/

#ifndef CORE_LEVEL
#define CORE_LEVEL 4
#endif

#include <iostream>
#include <CORE/CORE.h>


using namespace std;

// Creating a 53 bit random integer
//
//    THIS FUNCTION IS ALSO AVAILABLE IN CORE as "bigrand()"
//    BUT THE RETURNED VALUE IS machine double
BigInt myBigRand(){
	BigInt r = rand();
	BigInt s = rand();
	// Shift r to make space for 21 more bits:
	//   r *= (BigInt(1) << 21);
	// Add 21 more bits from s
	//   r += (s>> 11); 
	return((r<<21) + (s>>11));
}

// Creating a 53+xtraBits random integer
// 	This should NOT fit into double, even with xtraBits=1.
// 	For some reason, there is no problem with 1 extra bit?
BigInt badRand(int xtraBits=2){
	if (xtraBits >11) xtraBits=11;  // negative shifts cause problems
	BigInt r = rand();
	BigInt s = rand();
	return((r << (21 + xtraBits)) + (s >> (11 - xtraBits)));
}


int main(int argc, char** argv)
{

  // PROCESSING ARGUMENTS:
  int N = 1000;  //default argument
  if (argc > 1) N = atoi(argv[1]);

  int xtra = 2;
  if (argc > 2) xtra = atoi(argv[2]); 

  if (argc > 3) srand(123);  // pseudo random only...
  else srand(time(0)); 	     // default is true random

  // MAIN LOOP:
  int err=0, baderr=0;
  double r;
  BigInt M, badM, R;
  M=0; // keep track of the largest random number
  badM=0;

  for (int i = 0; i<N ; i++){
	R = myBigRand();
	r = R.doubleValue();
	if (M<R) M = R; //update
	if (R != BigInt(r)) {
		err++;
		cout << "  ERROR! r = " << r << ", R = " << R << endl;
	}
	R = badRand(xtra);
	if (M<R) badM = R; //update
	r = R.doubleValue();
	if (R != BigInt(r)) {
		baderr++;
	}
  }

  cout << "*** Number of tests, N = " << N << endl;
  cout << "*** Number of real errors = " << err << endl;
  cout << "*** Number of fake errors = " << baderr << endl;
  cout << "*** Largest random number generated is      = " << M << endl;
  cout << "*** Largest fake random number generated is = " << badM << endl;

  return 0;
}

