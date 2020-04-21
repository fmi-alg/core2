/* ***************************************************************
   File: pi1.cc
	 This is different from pi.cc only in that the validated values
	 for Pi are stored in the program, instead of read from file.
	 This turns out to be slower??

   Purpose: To compute Pi and sqrt(Pi) to any desired precision using
        Machin's series.

   Usage:
	  % pi [optPrec] [DOsqrt] [DOvalidate]

	where the arguments (all optional) are

	-- [optPrec] is the absolute precision in number of bits.
		Default is [optPrec] = 54 (as in machine double)

	-- [DOsqrt] is a flag (default to 1).
		A non-zero value will cause the program to compute sqrt{Pi),
		validated to 100 digits.

	-- [DOvalidate] is a flag (default to 1) 
		[DOvalidate]=0 means no validation of Pi.  
		[DOvalidate]=1 means validate Pi up to 250 digits.
		[DOvalidate]=2 means validate Pi up to 2000 Digits.  

	EXAMPLE:  To compute Pi to 2000 digits, set [optPrec] to
	6644 (= 2000 * log_2(10)).  If you also want to check the 2000
	computed digits, but to skip the sqrt(Pi) computation, you may type:

	  % pi 6644 0 2

	WARNING: there are 2 sources for slowness in running this program.
	(1) [DOvalidate]=2 is slow just because of the need to read 2000
		digits from a file and converting them into internal format.
		We cannot do much about this.  The actual time to compute
		Pi to 2000 digits is no problem at all (you will see the
		output in a flash)
	(2) [DOsqrt]=1 and [optPrec] is large.

   Notes:
	The implementation is basically straightforward, and illustrates
	the power of the Core Library's features.  It is clear that most
	of the common functions of analysis can be similarly implemented
	to achieve the guaranteed accuracy feature (see below).

	GUARANTEED OUTPUT:  The specified number of bits is translated into
	an equivalent number of digits for output.  All but the last printed
	digits are guaranteed to be correct.  The last digit can be off 
	by +/-1.  Thus, if an output that terminates with ...1999, the
	actual digits might actually be ...2000 or ...1998.
	
	SQUARE ROOT of Pi: This constant is useful, e.g., in computing
	the error function (erf).  We compute sqrt(Pi) to ([optPrec] - 2) bits
	of absolute precision.  But this computation can be turned off
	if the optional second argument is 0.

	SELF-VALIDATION:
	This program checks that the INTERNAL VALUE of computed Pi
	and the OUTPUT of the computed Pi are each correct up to
	N digits.  Here, N = 0, 250, 2000 (respectively) when the flag
	[DOvalidate] = 0, 1, 2.

	It also checks that the internal value of computed sqrt(Pi),
	when computed, is correct up to 100 digits.

   Author: Chee Yap and Chen Li (June 2000)
   Since CORE Library Version 1.2
   	$Id: pi1.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 *************************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#  define CORE_LEVEL 3

#include "CORE.h"

double arctan(int x, int a) {
  // compute arctan(1/x) to absolute precision 2^{-a}
  // x is a natural number and x > 1.

  int N = (int) (a / ( 2 * log(x)/log(2))) + 1;
  // std::cout << " Number of terms needed in arctan, N = " << N << std::endl;

  double at = 1;
  double x2 = x * x;
  for (int i=1; i<=N; i++) {
    if (i % 2 == 1) 
      at = x2 * at - 1 / double(2 * i + 1);
    else
      at = x2 * at + 1 / double(2 * i + 1);
  }
  
  at /= pow (x2, N) * x;
  
  at.approx(extLong::CORE_posInftyLong, a);
  return at;
}

int main( int argc, char *argv[] ) {

  /* ***************************************************************************
  COMMAND LINE ARGUMENTS
  *************************************************************************** */
  int eps = 54; 	// Number of bits of absolute precision desired
			// default to 54 bits (= machine double precision)
  int DOsqrt = 1;		// a flag: defaults to 1
			// (i.e., compute sqrt)
  int DOvalidate = 1;	// a flag to validate Pi: defaults to 1
			// (i.e., validate to 250 digits)

  if (argc > 1) eps = atoi(argv[1]);	
  if (argc > 2) DOsqrt = atoi(argv[2]);
  if (argc > 3) {
     DOvalidate = atoi(argv[3]);	
     if ((DOvalidate < 0) || (DOvalidate >2)) {
	std::cout << " !! Third argument in error, defaults to 1\n";
	DOvalidate = 1;
     };
  };

std::cout << "DOsqrt = " << DOsqrt << ";  DOvalidate = " << DOvalidate << std::endl;

  /* ***************************************************************************
  COMPUTING Pi
  *************************************************************************** */
  // Translates eps (in bits) to outputPrec (in digits)

  int outputPrec;	// desired precision in digits
  outputPrec = (int) (eps * log(2)/log(10));
  std::cout << " Output precision is " << outputPrec << " digits \n";

  // Computing Auxilliary Values
  double t1 = arctan(5, eps + 6);
  // debugging output:
  // std::cout << std::setprecision(outputPrec+1) << "arctan(1/5)   = " << t1 << std::endl;
  double t2 = arctan(239, eps + 4);
  // debugging output:
  // std::cout << std::setprecision(outputPrec+1) << "arctan(1/239) = " << t2 << std::endl;

  double pi = (4 * t1 - t2) * 4;
  pi.approx(extLong::CORE_posInftyLong, eps);

  // Output of Pi
  std::cout << " Pi = " << std::setprecision(outputPrec+1) << pi << std::endl;
  //   Note: setprecision in C++ is actually "width" (=number of characters
  //   in output.  So we use "outputPrec + 1" to account for the decimal point.

  /* ***************************************************************************
  AUTOMATIC CHECK that
	(1) our internal value of Pi
  	(2) our output value of Pi
  are both correct to 250 (or 2000) digits
  *************************************************************************** */

/*
  // Reading in digits of Pi from Files
  int prec;				// bit precision for the comparison
  std::ifstream from;			// input stream from file
  if (DOvalidate == 0) {		// no validation 
    prec = 1;
  }
  if (DOvalidate == 1) {
    prec = core_min(eps, 830);	// 830 bits = 250 digits.
    from.open("PI250", std::ios::in);	// read 250 digits of Pi from file
  }
  if (DOvalidate == 2) {
    prec = core_min(eps, 6644);	// 6644 bits = 2000 digits
    from.open("PI2000", std::ios::in);	// read 2000 digits of Pi from file
  }

  if (DOvalidate > 0) {		// validation needed
    string piStr; 
    char c;
    while (from.get(c))
	if ((c >= '0' && c <= '9') || (c=='.')) 
		piStr += c;
    if (!from.eof()) std::cout << "!! Error in reading from PI250 \n";
    from.close();
    double bigPi(piStr.c_str());	// bigPi is the value of Pi from file

    // debug:
    //  std::cout << " bigPi = " << std::setprecision(outputPrec + 1) << bigPi << std::endl;

    // CHECKING OUTPUT VALUE OF Pi:
    std::ostringstream ost;
    ost << std::setprecision(outputPrec+1) << pi << std::endl;
    piStr = ost.str();

    // Need to take the min of outputPrec and the number of digits in bigPi
    int minPrec;
    if (DOvalidate == 1)
	minPrec = core_min(outputPrec, 250);
    if (DOvalidate == 2) 
	minPrec = core_min(outputPrec, 2000);

    if ( fabs(Expr(piStr.c_str()) - bigPi) <= pow(Expr(10), -minPrec+1))
       std::cout << " >> Output value of Pi verified to "<< minPrec << " digits\n";
    else
       std::cout << " !! Output value of Pi INCORRECT to " << minPrec << " digits! \n";

    // CHECKING INTERNAL VALUE of Pi:
    if ( fabs(pi - bigPi) <= BigFloat::exp2(- prec +1))
       std::cout << " >> Internal value of Pi verified up to " << prec << " bits\n";
    else
       std::cout << " !! Internal value of Pi INCORRECT to " << prec << " bits! \n";

  };

 */


// ALTERNATIVE METHOD FOR READING VALIDATEDED DIGITS OF PI

  int prec;
  double bigPi;		// validated digits of Pi
  if (DOvalidate == 1) { 	// 250 digits
  bigPi = double("\
3.14159265358979323846264338327950288419716939937510582097494459\
230781640628620899862803482534211706798214808651328230664709384\
460955058223172535940812848111745028410270193852110555964462294\
8954930381964428810975665933446128475648233786783165271201909\
");
  }
  if (DOvalidate == 2) { 	// 2000 digits
  bigPi = double("3\
.14159265358979323846264338327950288419716939937510582097494459\
230781640628620899862803482534211706798214808651328230664709384\
460955058223172535940812848111745028410270193852110555964462294\
895493038196442881097566593344612847564823378678316527120190914\
564856692346034861045432664821339360726024914127372458700660631\
558817488152092096282925409171536436789259036001133053054882046\
652138414695194151160943305727036575959195309218611738193261179\
310511854807446237996274956735188575272489122793818301194912983\
367336244065664308602139494639522473719070217986094370277053921\
717629317675238467481846766940513200056812714526356082778577134\
275778960917363717872146844090122495343014654958537105079227968\
925892354201995611212902196086403441815981362977477130996051870\
72113499999983729780499510597317328160963185950244594553469083\
026425223082533446850352619311881710100031378387528865875332083\
814206171776691473035982534904287554687311595628638823537875937\
519577818577805321712268066130019278766111959092164201989380952\
572010654858632788659361533818279682303019520353018529689957736\
225994138912497217752834791315155748572424541506959508295331168\
617278558890750983817546374649393192550604009277016711390098488\
240128583616035637076601047101819429555961989467678374494482553\
797747268471040475346462080466842590694912933136770289891521047\
521620569660240580381501935112533824300355876402474964732639141\
992726042699227967823547816360093417216412199245863150302861829\
745557067498385054945885869269956909272107975093029553211653449\
872027559602364806654991198818347977535663698074265425278625518\
184175746728909777727938000816470600161452491921732172147723501\
414419735685481613611573525521334757418494684385233239073941433\
345477624168625189835694855620992192221842725502542568876717904\
946016534668049886272327917860857843838279679766814541009538837\
863609506800642251252051173929848960841284886269456042419652850\
222106611863067442786220391949450471237137869609563643719172874\
677646575739624138908658326459958133904780275901\
"); };

  if (DOvalidate > 0) {		// validation needed

    //  std::cout << " bigPi = " << std::setprecision(outputPrec + 1) << bigPi << std::endl;

    // CHECKING OUTPUT VALUE OF Pi:
    std::ostringstream ost;
    ost << std::setprecision(outputPrec+1) << pi << std::endl;
		std::string piStr = ost.str();

    // Need to take the min of outputPrec and the number of digits in bigPi
    int minPrec;
    if (DOvalidate == 1)
	minPrec = core_min(outputPrec, 250);
    if (DOvalidate == 2) 
	minPrec = core_min(outputPrec, 2000);

    if ( fabs(Expr(piStr.c_str()) - bigPi) <= pow(Expr(10), -minPrec+1))
       std::cout << " >> Output value of Pi verified to "<< minPrec << " digits\n";
    else
       std::cout << " !! Output value of Pi INCORRECT to " << minPrec << " digits! \n";

    // CHECKING INTERNAL VALUE of Pi:
    if ( fabs(pi - bigPi) <= BigFloat::exp2(- prec +1))
       std::cout << " >> Internal value of Pi verified up to " << prec << " bits\n";
    else
       std::cout << " !! Internal value of Pi INCORRECT to " << prec << " bits! \n";

  };

  /* ***************************************************************************
  COMPUTING THE SQUARE ROOT of Pi
	to (eps - 2) bits of absolute precision
  *************************************************************************** */
  if (DOsqrt == 0) return 0;	// do not compute sqrt(Pi)

  // Here is sqrt(Pi) to 100 digits from Knuth:
  Expr SQRTPI="1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665";

  double sPi = sqrt(pi);
  sPi.approx(extLong::CORE_posInftyLong, eps + 2);
  std::cout << " sqrt(Pi) = " << std::setprecision(outputPrec-1) << sPi << std::endl;

  /* ***************************************************************************
  AUTOMATIC CHECK that the internal value of computed sqrt(Pi) is correct
  up to the first 100 digits.
  *************************************************************************** */
  prec = core_min(eps-1, 332);	// 332 bits == 100 digits
  double d3 = fabs(sPi - SQRTPI);
  double tmp = pow(Expr(10), (int) - prec * log(2)/log(10));
  if ( d3 <= tmp )
     std::cout << " >> Internal value of sqrt(Pi) verified to " << prec << " bits\n";
  else
     std::cout << " !! Internal value of sqrt(Pi) INCORRECT to " << prec << " bits! \n";
  
  return 0;
}

