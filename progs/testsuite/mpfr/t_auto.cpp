#include <iostream>
#include <CORE/Mpfr.h>

/* Define BITS_PER_MP_LIMB
   Can't use sizeof(mp_limb_t) since it should be a preprocessor constant */
#if defined(GMP_NUMB_BITS) /* GMP 4.1.2 or above */
# define BITS_PER_MP_LIMB  (GMP_NUMB_BITS+GMP_NAIL_BITS)
#elif defined (__GMP_BITS_PER_MP_LIMB) /* Older versions 4.x.x */
# define BITS_PER_MP_LIMB  __GMP_BITS_PER_MP_LIMB
# define GMP_NUMB_BITS BITS_PER_MP_LIMB
# ifndef GMP_NAIL_BITS
#  define GMP_NAIL_BITS 0
# endif
#else
# error "Could not detect BITS_PER_MP_LIMB. Try with gmp internal files."
#endif

#define MPFR_MANT(x)      ((x)->_mpfr_d)
#define MPFR_PREC(x)      ((x)->_mpfr_prec)
#define MPFR_LIMB_SIZE(x) ((MPFR_PREC((x))-1)/BITS_PER_MP_LIMB+1)

inline void mpfr_remove_trailing_zeroes(mpfr_t x) {
  unsigned int xn = MPFR_LIMB_SIZE(x); 
  mp_limb_t* xp = MPFR_MANT(x);
  unsigned int i = 0;
  while (i < xn && xp[i] == 0) i++;
  if (i > 0 && i < xn) mpfr_round_prec(x, GMP_RNDN, (xn-i)*BITS_PER_MP_LIMB); 
}

int main(int argc, char* argv[]) {
  Mpfr f1;
  Mpfr f2;

  f1.set_prec(4);
  f1.set_2exp(9, -1);
  f2.set_prec(4);
  f2.set_2exp(9, -4);

  std::cout << "f1.exp=" << f1.get_exp() << std::endl;
  std::cout << "f2.exp=" << f2.get_exp() << std::endl;
  prec_t prec = Mpfr::mul_prec(f1, f2);
  std::cout << "prec=" << prec << std::endl;

  Mpfr f;
  f.set_prec(prec);

  f.mul(f1, f2);

  std::cout << "f1=" << f1.get_str(0, 2) << std::endl;
  std::cout << "f2=" << f2.get_str(0, 2) << std::endl;
  std::cout << "f=" << f.get_str(0, 2) << std::endl;

  mpfr_remove_trailing_zeroes(f.mp());
  std::cout << "f=" << f.get_str(0, 2) << std::endl;
  std::cout << "prec=" << f.get_prec() << std::endl;

  int z = ::ilogb(0);
  std::cout << "z=" << z << std::endl;
  std::cout << "z+10=" << z+10 << std::endl;
  std::cout << "INT_MAX=" << INT_MAX << std::endl;
  std::cout << "-INT_MAX=" << -INT_MAX << std::endl;
  std::cout << "LONG_MIN=" << LONG_MIN << std::endl;

  return 0;
}
