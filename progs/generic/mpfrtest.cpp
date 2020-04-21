/*
  file: mpfrtest.cpp

  Purpose: Run this program before installing the Core Library
  		to see if you already have an installed gmp library
		that can be detected by your GCC compiler.
	   If you pass this test, then you can skip Stage One of
	   building the Core Library (which is simply to install gmp).
 */

#include <cstddef>	// Chee, Jan 2018: you need this for MacOS

#include <stdio.h>
#include <mpfr.h>

int main (int argc, char **argv) 
{
  mpfr_t a, b, p;

  mpfr_init2(a, 32);
  mpfr_init2(b, 32);
  mpfr_init2(p, 32);

  // test 1
  mpfr_set_str(a, "12345", 10, GMP_RNDN);
  mpfr_set_str(b, "67890", 10, GMP_RNDN);

  mpfr_mul (p, a, b, GMP_RNDN);

  mpfr_out_str( stdout, 10, 0, a, GMP_RNDN);
  fputs (" * ", stdout);
  mpfr_out_str( stdout, 10, 0, b, GMP_RNDN);
  fputs (" = ", stdout);
  mpfr_out_str( stdout, 10, 0, p, GMP_RNDN);
  fputc ('\n', stdout);

  // test 2 (pi)
  mpfr_set_str(a, "355", 10, GMP_RNDN);
  mpfr_set_str(b, "113", 10, GMP_RNDN);

  mpfr_div (p, a, b, GMP_RNDN);

  mpfr_out_str( stdout, 10, 0, a, GMP_RNDN);
  fputs (" / ", stdout);
  mpfr_out_str( stdout, 10, 0, b, GMP_RNDN);
  fputs (" = ", stdout);
  mpfr_out_str( stdout, 10, 0, p, GMP_RNDN);
  fputc ('\n', stdout);

  fputs (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", stdout);
  fputs (">>>    If the first answer on your screen is 8.3810205000e8\n",
		  	stdout);
  fputs (">>>    and the second answer on your screen is 3.1415929208\n",
		  	stdout);
  fputs ("\n>>>    you have passed the mpfr test\n", stdout);
  fputs (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", stdout);

  mpfr_clear (a);
  mpfr_clear (b);
  mpfr_clear (p);

  return 0;
}
