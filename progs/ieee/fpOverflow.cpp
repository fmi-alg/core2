/* fpOverflow.cpp
 *
 * 	Taken from:
 *	https://stackoverflow.com/questions/15655070/how-to-detect-double-precision-floating-point-overflow-and-underflow
 *
 * 	SYNOPSIS:	> fpOverflow [n]
 *
 *		will run a for-loop for n times.  The overflag and
 *		underflow flag is initially turned off.  
 *
 *		If n=7, the overflow flag will be turned on, but the
 *			underflow flag will not be turned on.
 *		If n>7, both the
 *			overflow and underflow flags will be turned on.
 *		If n<7, both the
 *			overflow and underflow flags will not be turned on.
 *		
 *	This program relies on the library <fenv> which has 
 *		functions to test the IEEE754 exceptions flags.
 *
 *	Note that
 *		POSIX, C99, C++11 have <fenv.h>
 *			and C++11 has <cfenv>)
 *
 *	When compiling, be sure to give "-std=c++11" flag to g++.
 *
 *	-- Chee (Jan 2018)
 *
 *	Optimization BUG: if you compile this program
 *		using "make fpOverflow", the
 *		resulting executable is actually wrong: it will never
 *		show the overflow/underflow flags turned on!  Reason?
 *		because the default flags include
 *		the optimization flag (-O3).  Hence, we
 *		provide a target ("make fgOverflowX") which compiles without
 *		the optimization flags.  Now the overflow/underflow flags
 *		are properly turned on.
 *		Try this?
 *			#pragma fenv_access (on)  
 ****************************************************/
#include <cfenv>
#include <iostream>


int main(int argc, char* argv[]) {

    int n= 7;
    if (argc > 1) n = atoi(argv[1]);

    std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);

    double overflowing_var = 1000;
    double underflowing_var = 0.01;

    std::cout << "Overflow flag before: "
		<< (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag before: "
		<< (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;

    for(int i = 0; i < n; ++i) {
        overflowing_var *= overflowing_var;
        underflowing_var *= underflowing_var;
    }

    std::cout << "Overflow flag after: "
		<< (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag after: "
		<< (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;
}
