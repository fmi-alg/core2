/*
  file: gmptest.cpp

  Purpose: Run this program before installing the Core Library
  		to see if you already have an installed gmp library
		that can be detected by your GCC compiler.
	   If you pass this test, then you can skip Stage One of
	   building the Core Library (which is simply to install gmp).
 */

#include <cstddef>	// Chee, Jan 2018: you need this for MacOS
#include <stdio.h>
#include <gmp.h>

int main (int argc, char **argv) 
{
  mpz_t a, b, p;

  mpz_init (a);
  mpz_init (b);
  mpz_init (p);

  // test 1
  mpz_set_str(a, "12345", 10);
  mpz_set_str(b, "67890", 10);

  mpz_mul (p, a, b);

  mpz_out_str( stdout, 10, a);
  fputs (" * ", stdout);
  mpz_out_str( stdout, 10, b);
  fputs (" = ", stdout);
  mpz_out_str( stdout, 10, p);
  fputc ('\n', stdout);

  // test 2
  mpz_set_str(a, "111111111", 10);
  mpz_set_str(b, "111111111", 10);

  mpz_mul (p, a, b);

  mpz_out_str( stdout, 10, a);
  fputs (" * ", stdout);
  mpz_out_str( stdout, 10, b);
  fputs (" = ", stdout);
  mpz_out_str( stdout, 10, p);
  fputc ('\n', stdout);

  fputs (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", stdout);
  fputs (">>>   If the first answer on your screen is 838102050, ", stdout);
  fputs ("\n>>>   and the second answer on your screen is 12,345,678,987,654,321,", stdout);
  fputs ("\n>>>   you have passed the gmp test\n", stdout);
  fputs (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", stdout);

  mpz_clear (a);
  mpz_clear (b);
  mpz_clear (p);

  return 0;
}
