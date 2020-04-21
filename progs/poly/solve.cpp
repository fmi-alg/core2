/* **************************************
   File: solve.cpp

   Description: 
	Solving for real algebraic roots
	    It is based on the Polynomial class

   Usage:
	% alge [opt-prec=54] [start=0] [end=start+10]

	where [opt-prec] is the optional guaranteed precision 
	and [start], [end] define the interval in which to search
	for root. 

   Author:  Chee Yap, Sylvain Pion
   Date:    July 20, 2002

   Since Core Library  v1.4
   $Id: solve.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#ifndef Level
#  define Level 3
#endif

#include <vector>
#include "Sturm.h"
#include "Newton.h"

// ==================================================
// 
// ==================================================


// ==================================================
// Main
// ==================================================

// typedef int NT
typedef BigInt NT;
typedef Polynomial<NT> Poly;

    // Sqrt 2
    NT c_root2[] = {-2, 0, 1};
    Poly root2(2, c_root2);

    // Legendre : 4 real roots between [-1;1]
    NT c_legendre4[] = {3, 0, -30, 0, 35};
    Poly legendre4(4, c_legendre4);

    // wilk.pol Wilkinson polynomial (x-1)(x-2)...(x-n) n=20
    // dri (dense, real?, i?)
    // 0 (exact coeff)
    // 20 (degree)

    NT c_wilk[] = {
	"2432902008176640000",
	"-8752948036761600000",
	"13803759753640704000",
	"-12870931245150988800",
	"8037811822645051776",
	"-3599979517947607200",
	"1206647803780373360",
	"-311333643161390640",
	"63030812099294896",
	"-10142299865511450",
	"1307535010540395",
	"-135585182899530",
	"11310276995381",
	"-756111184500",
	"40171771630",
	"-1672280820",
	"53327946",
	"-1256850",
	"20615",
	-210,
	1};

  Poly wilk(20, c_wilk);
  Sturm<NT> SS;

  void debug(std::string m) {
    std::cout << "DEBUG: calling isolateRoots\n";
  }

  void testSturm( Poly P) {
    SS = * new Sturm<NT>(P);
    // SS.dump(std::string("Test Sturm Poly, "));

    std::vector<std::pair<Expr, Expr> > v;
    SS.isolateRoots(v);
    std::cout << " Number of roots : " << v.size() << std::endl;
    for (std::vector<std::pair<Expr, Expr> >::const_iterator it = v.begin();
         it != v.end(); ++it) {
      std::cout << " Root in [" << it->first << " ; "
                                << it->second << "]" << std::endl;
    }
  }//sturmtest

void processCommandLine(int argc, char ** argv ) {
    // SET INTERNAL PRECISION
    if (argc >= 2) {
  	setDefaultRelPrecision(atoi(argv[1]));	// if user specifies precision
    } else
    	setDefaultRelPrecision(80); 	// otherwise, defRelPrec = 54
  					// i.e., as in machine arithmetic 
  
    setDefaultAbsPrecision(CORE_posInfty); // This puts defRelPrec in control
    int prec = 100;			// Do Newton iteration till delta
					// is at most 2^{-prec}
  
    // SET OUTPUT PRECISION
    std::cout.setf(std::ios::scientific, std::ios::floatfield); // choose
    					// scientific notation
    std::cout.precision(11); 		// default Print Digits = 11
    
    std::cout << "-------------------------------------------------------\n";
    std::cout << "Relative Precision is " << defRelPrec << std::endl;
    std::cout << "Print Digits is " << std::cout.precision() << std::endl;
    std::cout << "Iteration Stops if delta < "
			<< BigFloat::exp2(prec) << std::endl;
    //  std::cout << setprecision(defPrtDgt.asLong()); // already set above.
    std::cout << "-------------------------------------------------------\n";

}

typedef Sturm<NT>::Interval Interval;

  int main(int argc, char** argv){

    processCommandLine(argc, argv);
  
    // sturm test of Wilk -- slow (about 3-4 min)!
    testSturm(legendre4);

    SS = * new Sturm<NT> (legendre4);
    Interval v = SS.isolateRoot(3);
    std::cout << "3rd Root = " << v.first << ", " << v.second << std::endl; 
    v = SS.isolateRoot(1);
    std::cout << "1rd Root = " << v.first << ", " << v.second << std::endl; 
    v = SS.isolateRoot(5);
    std::cout << "5rd Root = " << v.first << ", " << v.second << std::endl; 

    v = SS.firstRootBelow(0);
    std::cout << "1st below 0 = " << v.first << ", " << v.second << std::endl; 
    v = SS.firstRootBelow(100);
    std::cout << "1st below 100 = "<< v.first << ", " << v.second << std::endl; 
    v = SS.firstRootAbove(0);
    std::cout << "1st above 0 = " << v.first << ", " << v.second << std::endl; 
    v = SS.firstRootAbove(-100);
    std::cout << "1st above -100 = "<<v.first << ", " << v.second << std::endl; 

    testSturm(wilk);

    return 0;
  }// main
