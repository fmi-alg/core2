#include <gmp.h>
#include <mpfr.h>
#include <iostream>

void t_signed_zero() {
  mpfr_t pos_inf, neg_inf, pos_zero, neg_zero;
  mpfr_init(pos_inf); mpfr_set_inf(pos_inf, 1);
  mpfr_init(neg_inf); mpfr_set_inf(neg_inf, -1);
  mpfr_init(pos_zero); mpfr_ui_div(pos_zero, 1, pos_inf, GMP_RNDN); 
  mpfr_init(neg_zero); mpfr_ui_div(neg_zero, 1, neg_inf, GMP_RNDN); 

  std::cout << "is_zero(pos_zero)=" << (mpfr_zero_p(pos_zero)!=0) << std::endl;
  std::cout << "is_zero(neg_zero)=" << (mpfr_zero_p(neg_zero)!=0) << std::endl;
  std::cout << "sgn(pos_zero)=" << (mpfr_sgn(pos_zero)) << std::endl;
  std::cout << "sgn(neg_zero)=" << (mpfr_sgn(neg_zero)) << std::endl;

  mpfr_clear(pos_inf);
  mpfr_clear(neg_inf);
  mpfr_clear(pos_zero);
  mpfr_clear(neg_zero);
}
/// test return value of mpfr_get_exp
/// result: f = (-1)^s z 2^e where 0.5 <= z < 1
void t_exp() {
  mpfr_t f;
  mpfr_init(f);
  mpfr_set_prec(f, 64);
  mpfr_set_si(f, 128, GMP_RNDN);
  mp_exp_t e = mpfr_get_exp(f);
  std::cout << "prec=" << mpfr_get_prec(f) << std::endl; 
  std::cout << "e=" << e << std::endl; 

  mpz_t z;
  mpz_init(z);
  e = mpfr_get_z_exp(z, f);
  std::cout << "e=" << e << std::endl; 
  std::cout << "z=" << mpz_get_str(0, 10, z) << std::endl; 

  mpz_clear(z);
  mpfr_clear(f);
}
void test(mpz_t mp) {
  mpz_neg(mp, mp);
}

int main() {
  mpz_t z;
  mpz_init_set_si(z, 10);
  std::cout << "z=" << mpz_get_str(NULL, 10, z) << std::endl;
  t_signed_zero();
  mpz_clear(z);
  return 0;
}
