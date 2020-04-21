#include "many.h"

int
main (int argc, char *argv[])
{
  unsigned long N = atoi (argv[1]), M;
  mp_prec_t p;
  mpfr_t i, j;
  char *lo;
  mp_exp_t exp_lo;
  int st, st0;

  fprintf (stderr, "Using GMP %s and MPFR %s\n", gmp_version, mpfr_version);
  st = cputime ();

  mpfr_init (i);
  mpfr_init (j);

  M = N;

  do
    {
      M += 10;
      mpfr_set_prec (i, 32);
      mpfr_set_d (i, LOG2_10, GMP_RNDU);
      mpfr_mul_ui (i, i, M, GMP_RNDU);
      mpfr_add_ui (i, i, 25, GMP_RNDU);
      p = mpfr_get_ui (i, GMP_RNDU);
      fprintf (stderr, "Setting precision to %lu\n", p);


      mpfr_set_prec (i, p);
      mpfr_set_prec (j, 2);

      mpfr_set_d (j, 0.5, GMP_RNDN);
      mpfr_exp (i, j, GMP_RNDN);
      mpfr_exp (i, i, GMP_RNDN);
      mpfr_exp (i, i, GMP_RNDN);

      st0 = cputime ();
      lo = mpfr_get_str (NULL, &exp_lo, 10, M + 3, i, GMP_RNDN);
      st0 = cputime () - st0;
    }
  while (can_round (lo + 3, N, M) == 0);

  lo += 3; /* skip 181 */
  lo[N] = '\0';
  printf ("%s\n", lo);

  mpfr_clear (i);
  mpfr_clear (j);

  fprintf (stderr, "Cputime: %dms (output %dms)\n", cputime () - st, st0);
  return 0;
}
