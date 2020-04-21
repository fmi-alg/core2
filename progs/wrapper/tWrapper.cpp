/* **************************************
   File: tWrapper.cpp

   Purpose: This is a unit test for Wrappers in Level 1. 
	to control output precision of four aritmetic operations
        - addition, subtraction, multiplication, division

        set_wbf_mode(bool b) turns on and off the weak bigfloat mode.
        set_wbf_prec(prec p) specifies the desired output precision.

        Example:

        set_wbf_prec(100);
        A = B*C;

        In weak bigfloat mode, the precision of A will be set to 100, such that
        the accuracy of multiplication is guaranteed to 100-bit relative precision.
	This is the standard IEEE model of accuracy.

   TODO:
   1.  There is some ambiguity about input.  Below, we have:

  		BigFloat x(1.23456789), y(2.98765432);

	The question is: what the internal value of x and y (which
	must be rounded from decimal to binary)?  This has to be
	checked...
   2.  If the user gives you the precision and the arguments to test,
	we need to use Expr to generate the correct answer to test.
   3.  We should test the full gamut of Core 2 operators, including
	transcendentals.

   Since CORE Library Version 2.0
   $Id: tWrapper.cpp,v 1.1 2010/05/24 13:25:47 exact Exp $
************************************** */
#define CORE_LEVEL 1

#include "CORE/CORE.h"



