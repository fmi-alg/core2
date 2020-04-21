/***************************************************
 *	file: tFinite.cpp
 *		from http://en.cppreference.com/w/cpp/numeric/math/isfinite
 *
 * 	The method is
 * 		std::isfinite( ... ) takes a floating point
 * 			argument and returns a boolean
 *
 *	These are defined in header <cmath>
 *		bool isfinite( float arg ); (1)	(since C++11)
 *		bool isfinite( double arg ); (2)	(since C++11)
 *		bool isfinite( long double arg ); (3)	(since C++11)
 *		bool isfinite( Integral arg ); (4)	(since C++11)
 *
 *	(1-3) Determines if the given floating point number arg
 *		has finite value i.e. it is normal, subnormal or zero,
 *		but not infinite or NaN.
 *	(4) A set of overloads or a function template accepting the
 *		from argument of any integral type. Equivalent to (2)
 *		(the argument is cast to double).
 *
 *
 *	This program performs 5 tests and they should have these answers:  
 *
 *		isfinite(NaN) = false
 *		isfinite(Inf) = false
 *		isfinite(0.0) = true
 *		isfinite(exp(800)) = false
 *		isfinite(DBL_MIN/2.0) = true
 *
 *	--Chee Yap (Sep 2016)
 ***************************************************/

#include <iostream>
#include <cmath>
#include <cfloat>
 
int main()
{
    std::cout << std::boolalpha
              << "isfinite(NaN) = " << std::isfinite(NAN) << '\n'
              << "isfinite(Inf) = " << std::isfinite(INFINITY) << '\n'
              << "isfinite(0.0) = " << std::isfinite(0.0) << '\n'
              << "isfinite(exp(800)) = " << std::isfinite(std::exp(800)) << '\n'
              << "isfinite(DBL_MIN/2.0) = " << std::isfinite(DBL_MIN/2.0) << '\n';
}
