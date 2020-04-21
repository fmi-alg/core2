/* ************************************************
   File: testIdent.ccp

   Purpose:  To test some basic identities in the Core Library
	     This suite of tests is taken from LiDIA distribution.

   Usage:
        % testIdent

   COMMENT: The tests are currently mostly based on the four arithmetic
		operators and sqrt.  Should extend of
		all of CORE functions.  Should also test
		boundary conditions.

   Since Core Library 1.3
   $Id: testIdent.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */        

#define CORE_LEVEL 3
#include "CORE/CORE.h"

// THESE ARE NOT DEFINED BY CORE:

// already in gmp:  #define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MIN(l,o) ((l) < (o) ? (l) : (o))
#define MAX(h,i) ((h) > (i) ? (h) : (i))

// identitytest(a, b, c):
// 	Added conditional tests, to allow a and b to be zero.
// 	--Chee 8/2003
void identitytest(const double& a, const double& b, const double& c)
{
  double addab = a + b;		// all these definitions help to
  double mulab = a * b;		// speed up the test a little
  double addbc = b + c;
  double mulbc = b * c;
  double subab = a - b;
  double divab = 0;
  if (b != 0)
	  divab = a / b;
  assert( -(-a) ==  a);
  assert( (addab) ==  (b + a));
  assert( (a + (-b)) ==  (subab));
  assert( (a + (-b)) ==  (b - a + 2*a - 2*b));
  assert( (mulab) ==  (b * a));
  assert( (a * (-b)) ==  -(a * b));
  if (b != 0)
  	assert( (a / (-b)) ==  -(divab));
  assert( (subab) ==  -(b - a));
  assert( (a + (addbc)) ==  ((addab) + c));
  assert( (a + (addbc)) ==  ((a + c) + b));
  assert( (a * (mulbc)) ==  ((mulab) * c));
  assert( (a * (mulbc)) ==  ((c * a) * b));
  assert( (a * (addbc)) ==  ((mulab) + (a * c)));
  assert( ((subab) + b) ==  a);
  assert( ((addab) - b) ==  a);
  if (a != 0)
  	assert( ((mulab) / b) ==  a);
  if (a != 0)
  	assert( ((mulab) / a) ==  b);
  if (mulab != 0)
  	assert( ((divab) / a) ==  (1/b));
// sqrts:
  double e = sqrt(a*a - (2*a*b) + b*b) - fabs(subab);
  assert(e == 0);

// THE FOLLOWING HAVE NO ANALOGUE IN CORE LIBRARY:
//  double d;
//    negate(d, a);  assert( -(d) ==  a);
//    add(d, b, a);  assert( (a + b) ==  d);
//    subtract(d, a , b);  assert( (a + (-b)) ==  d);
//    negate(d, b); assert( (a + d) ==  (b - a+2*a - 2*b));
//    d.assign(a); d.multiply_by_2(); assert( (a + a) ==  d);
//    multiply(d, a, b); assert( d ==  (b * a));
//    d.assign(b); d.negate(); multiply(d, a, d); assert( d ==  -(a * b));
//    divide(d, a, b); assert( (a / (-b)) ==  -d);
//    add(d, b, c); multiply(d, a, d);  assert(d ==  ((a * b) + (a * c)));
//    d = b; d.invert();  assert( ((a / b) / a) == d);
}

int main(int argc, char* argv[])
{

   double a, b, c;
   a = BigRat(100001,2000023);
   b = BigRat(7,999999);
   c = BigRat(3999,51145);
   identitytest(a,b,c);

   std::cout << "Finished First Identity Test \n";

   a = rand();
   b = rand();
   c = rand();
   std::cout << "Random test on (a,b,c)=("<< a
		<< ", " << b << ", " << c << ")\n" ;
   identitytest(a,b,c);

   std::cout << "Finished Random Identity Test \n";

   if (argc == 1) {
	std::cout << ">> To further exercise the program, rerun testIdenty\n"
	     << "by giving it some (any) argument e.g. testIdent 1 \n";
	exit(0);  // we do not run the SLOW tests
		  // below unless there is an argument
   }

   std::cout << "Entering loop \n";

// NOTE: if i, j are declared to be doubles, we have problem!

   for (double i=0; i<13 ; i++) 
     for (double j=100; j > 0; j -= 10) {
	std::cout << "(i,j) = (" << i << "," << j << ")\n";
	a = i/j;
	b = sqrt(i) / sqrt(101+j);
	if (i == 0) c = 1999;
	else c = (i + 1999) / (i*j);
	identitytest(a,b,c);
     }
   return 0;
}
