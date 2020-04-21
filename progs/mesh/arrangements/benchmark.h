/*
 * benchmark.h
 *
 * This piece code is originally implemented by
 *    Narayan to serve his benchmark code. The reason that I 
 *    "steal" it is because it has the functionality of parsing
 *    a file that stores a complex uni-variate polynomial with special
 *    format, and is able to separate it into 2 bi-variate polynomials.
 *
 *    For this algorithm we won't deal with above case, but rather,
 *    we will take 2 explicit bi-variate polynomials as parameters.
 *    However, it's good to have such a parser that works for special
 *    requirement. 
 *
 *    initial author: Narayan 
 *    Modified: Shang Wang(Aug, 11)
 * 
 * */


#include <iostream>
#include <vector>

// Ignore what the others say, and always compile
// This file into Level 4.
#define CORE_LEVEL 4

// Note that this does not include "benchmark.h"

#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"
#include "CORE/ComplexT.h"

using namespace std;

namespace benchmark {

// Converts a complex polynomial p(z) into two real
// bivariate polynomials u(x, y) & v(x, y) such that
// p(z) = u(x, y) + iv(x, y)
template <typename NT>
void ConvertPoly(const Polynomial<NT> &p,
                 BiPoly<NT> *real,
                 BiPoly<NT> *im) {
  BiPoly<NT> subst = "x + y";
  // p.eval<BiPoly<NT> >(subst) would also do the
  // trick.
  //
  // composeHorner is defined in composeBiPoly.h. Note that
  // it is not always required.
  BiPoly<NT> subst_result = composeHorner<NT>(p, subst);

  unsigned int degree = subst_result.getYdegree();

  vector<Polynomial<NT> > real_coeff;
  vector<Polynomial<NT> > im_coeff;

  bool negate = true;
  const vector<Polynomial<NT> > &c = subst_result.coeffX;

  for (unsigned int i = 0; i <= degree; ++i) {
    if (i % 2 == 0) {
      negate = negate ? false : true;
      real_coeff.push_back(c[i]);
      im_coeff.push_back(Polynomial<NT>());
      if (negate) {
        real_coeff.back().negate();
      }
    } else {
      im_coeff.push_back(c[i]);
      real_coeff.push_back(Polynomial<NT>());
      if (negate) {
        im_coeff.back().negate();
      }
    }
  }
  *real = BiPoly<NT>(real_coeff);
  *im = BiPoly<NT>(im_coeff);
}

template <typename T>
void read_poly(FILE *instr, Polynomial<T>& p)
{
  int indx, num_coeff, i;

  /* skip blank or comment lines */
  while ((i = fgetc(instr)) == '!' || isspace(i))
    if(i == '!')
      while (fgetc(instr) != '\n')
        /* skip */ ;
  ungetc(i, instr);

  /* read data type */
  char data_type[3];
  fscanf(instr, "%3s", data_type);

  /* read and convert prec_in from base 10 to base 2 */
  long prec_in = -1;
  if (prec_in == -1) {
    fscanf(instr, "%ld", &(prec_in));
    prec_in = (long) (prec_in * log(10.00)/log(2.0));// LOG2_10);
  } else {    /* override input precision */
    fscanf(instr, "%*d");
    prec_in = prec_in; /* load default */
  }

  /* read degree */
  int deg;
  fscanf(instr, "%d", &(deg));

  /* if sparse read num of coeff */
  if (data_type[0] == 's')
    fscanf(instr, "%d", &num_coeff);
  else
    num_coeff = deg + 1;

  /* no need to read coefficients if user polynomial */
  if (data_type[0] == 'u')
    return;

  /* read coefficients */
  T* coeffs = new T[deg+1]; // we always assume dense representation!

  for (int i = 0; i < deg + 1; ++i) {
    coeffs[i] = 0;
  }

  //char new_line;
  for (i = 0; i < num_coeff; i++) {

    if (data_type[0] == 's') {
      fscanf(instr, "%d", &indx);
    } else {
      indx = i;
    }

    if (data_type[1] == 'r') {
      if (data_type[2] == 'i') {
        BigInt temp;
        if (!mpz_inp_str(temp.mp(), instr, 10)) {
          // Exit if something happens.
          assert(false);
        }
        coeffs[indx]=T(temp);
      } else {
        // We support only integer coefficients at this point.
        assert(false);
      }
    } else {
      // We support only polynomials with real coefficients
      // at this point.
      assert(false);
    }
  }

  p = Polynomial<T>(deg, coeffs);
  delete[] coeffs;
}

template <typename T>
bool read_poly(const char* filename, Polynomial<T>& p)
{
  FILE* f;
  if ((f = fopen(filename, "r")) == NULL) {
    std::cerr << "error reading " << filename << std::endl;
    return false;
  }
  read_poly(f, p);
  fclose(f);
  return true;
}

void GetPoly(const char *filename,
             Polynomial<BigInt> *poly) {
  read_poly<BigInt>(filename, *poly);
}

void GetPoly(const char *filename,
             Polynomial<BigFloat> *poly) {
  read_poly<BigFloat>(filename, *poly);
}

void GetPoly(const char *filename,
             Polynomial<DoubleWrapper> *poly) {
  // cout << "********************************************************************" << endl;
  // cout << "WARNING : You might lose precision in the polynomial coefficients " << endl
  //     << "while operating at this core level. The polynomial you are operating " << endl
  //     << "on might not be the polynomial you intended." << endl;
  // cout << "********************************************************************" << endl;

  Polynomial<BigInt> p_bi;
  GetPoly(filename, &p_bi);
  const int deg = p_bi.getDegree();
  DoubleWrapper *dr = new DoubleWrapper[deg + 1];
  for (int i = 0; i <= deg; ++i) {
    dr[i] = p_bi.coeff()[i].doubleValue();
  }
  *poly = Polynomial<DoubleWrapper>(deg, dr);
  delete[] dr;
}

void GetPoly(const char *filename,
             Polynomial<Expr> *poly) {
  read_poly<Expr>(filename, *poly);
}

void GetBiPoly(const char *filename,
               BiPoly<BigInt> *u,
               BiPoly<BigInt> *v) {
  Polynomial<BigInt> poly;
  GetPoly(filename, &poly);
  ConvertPoly<BigInt>(poly, u, v);
}
void GetBiPoly(const char *filename,
               BiPoly<BigFloat> *u,
               BiPoly<BigFloat> *v) {
  Polynomial<BigFloat> poly;
  GetPoly(filename, &poly);
  ConvertPoly<BigFloat>(poly, u, v);
}
void GetBiPoly(const char *filename,
               BiPoly<DoubleWrapper> *u,
               BiPoly<DoubleWrapper> *v) {
  Polynomial<DoubleWrapper> poly;
  GetPoly(filename, &poly);
  ConvertPoly<DoubleWrapper>(poly, u, v);
}
void GetBiPoly(const char *filename,
               BiPoly<Expr> *u,
               BiPoly<Expr> *v) {
  Polynomial<Expr> poly;
  GetPoly(filename, &poly);
  ConvertPoly<Expr>(poly, u, v);
}

void GetRootBounds(const Polynomial<Expr> &poly,
                   Expr *lb,
                   Expr *ub) {
  (*lb) = CORE::CauchyLowerBound<Expr>(poly);
  (*ub) = CORE::CauchyUpperBound<Expr>(poly);
}

void GetRootBounds(const Polynomial<BigFloat> &poly,
                   BigFloat *lb,
                   BigFloat *ub) {
  BigInt temp = CORE::CauchyBound<BigFloat>(poly);
  cout << "Root bound : " << temp << endl;
  (*lb) = temp * -1;//CORE::CauchyLowerBound<BigFloat>(poly);
  (*ub) = temp;//CORE::CauchyUpperBound<BigFloat>(poly);
}

template <typename NT>
void ConvertPoly(Polynomial<BigFloat> *output,
    const Polynomial<NT> &in) {
  vector<BigFloat> coeff_array;
  for (int i = 0; i < in.getDegree() + 1; ++i) {
    coeff_array.push_back(in.coeff()[i]);
  }
  (*output) = Polynomial<BigFloat>(coeff_array);
}

void GetRootBounds(const Polynomial<DoubleWrapper> &poly,
                   DoubleWrapper *lb,
                   DoubleWrapper *ub) {
  Polynomial<BigFloat> poly_2;
  ConvertPoly(&poly_2, poly);

  BigInt temp = CORE::CauchyBound<BigFloat>(poly);

  (*lb) = DoubleWrapper((temp * -1).doubleValue());
  (*ub) = DoubleWrapper(temp.doubleValue());
}

void GenerateRandom(Polynomial<DoubleWrapper> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  srand(seed);
  const unsigned int MAX = 2*max_coeff;
  machine_long max = max_coeff;
  vector<DoubleWrapper> vec;
  for (unsigned int i = 0; i <= degree; ++i) {
    machine_long random = rand() % MAX;
    vec.push_back(random - max);
  }
  (*d) = Polynomial<DoubleWrapper>(vec);
}

void GenerateRandom(Polynomial<BigFloat> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  srand(seed);
  const unsigned int MAX = 2*max_coeff;
  BigFloat max = max_coeff;
  vector<BigFloat> vec;
  for (unsigned int i = 0; i <= degree; ++i) {
    machine_long random = rand() % MAX;
    vec.push_back(BigFloat(random) - max);
  }
  (*d) = Polynomial<BigFloat>(vec);
}

void GenerateRandom(BiPoly<DoubleWrapper> *u,
                    BiPoly<DoubleWrapper> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  Polynomial<DoubleWrapper> b;
  GenerateRandom(&b, degree, max_coeff, seed);
  ConvertPoly(b, u, v);
}

void GenerateRandom(BiPoly<BigFloat> *u,
                    BiPoly<BigFloat> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  Polynomial<BigFloat> b;
  GenerateRandom(&b, degree, max_coeff, seed);
  ConvertPoly(b, u, v);
}

/*
 *
 * Functions for dealing with polynomials with complex coefficients.
 * Not supported for now as this requires changes to the
 * ComplexT library and other parts of core as well.
 *
 **************/
#ifdef ENABLE_COMPLEX_POLYS

void GetPoly(const char *file_name,
             Polynomial<ComplexT<Expr> > *d) {
  cout <<"Not implemented !" << endl;
  assert(false);
}

void GetPoly(const char *file_name,
             Polynomial<ComplexT<BigFloat> > *d) {
  cout <<"Not implemented !" << endl;
  assert(false);
}

void GetPoly(const char *file_name,
             Polynomial<ComplexT<DoubleWrapper> > *d) {
  cout <<"Not implemented !" << endl;
  assert(false);
}

void GetRootBounds(const Polynomial<ComplexT<DoubleWrapper> > &poly,
                   DoubleWrapper *lb,
                   DoubleWrapper *ub) {
  vector<BigFloat> vec;
  for (int i = 0; i < poly.degree(); ++i) {
    vec.push_back(poly.coeff()[i].mod());
  }
  Polynomial<BigFloat> p(vec);
  BigInt temp = CORE::CauchyBound<BigFloat>(p);

  (*lb) = DoubleWrapper((temp * -1).doubleValue());
  (*ub) = DoubleWrapper(temp.doubleValue());
}


void GetRootBounds(const Polynomial<ComplexT<BigFloat> > &poly,
                   BigFloat *lb,
                   BigFloat *ub) {
  vector<BigFloat> vec;
  for (int i = 0; i < poly.degree(); ++i) {
    vec.push_back(poly.coeff()[i].mod());
  }
  Polynomial<BigFloat> p(vec);

  BigInt temp = CORE::CauchyBound<BigFloat>(p);

  (*lb) = temp * -1;//CORE::CauchyLowerBound<BigFloat>(poly);
  (*ub) = temp;//CORE::CauchyUpperBound<BigFloat>(poly);
}

void GenerateRandom(Polynomial<ComplexT<DoubleWrapper> > *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  srand(seed);
  const unsigned int MAX = 2*max_coeff;
  vector<ComplexT<DoubleWrapper> > vec;
  for (unsigned int i = 0; i <= degree; ++i) {
    machine_long random = rand() % MAX - max_coeff;
    machine_long random2 = rand() % MAX - max_coeff;
    vec.push_back(ComplexT<DoubleWrapper>(random, random2));
  }
  (*d) = Polynomial<ComplexT<DoubleWrapper> >(vec);
}

void GenerateMignotte(Polynomial<ComplexT<DoubleWrapper> > *d) {
  vector<ComplexT<DoubleWrapper> > vec(21);
  vec[20] = ComplexT<DoubleWrapper>(1, 0);
  vec[0] = ComplexT<DoubleWrapper>(1, 0);
  vec[1] = ComplexT<DoubleWrapper>(0, 300);
  vec[2] = ComplexT<DoubleWrapper>(-30000,0);
  vec[3] = ComplexT<DoubleWrapper>(0, -1000000);
  (*d) = Polynomial<ComplexT<DoubleWrapper> >(vec);
}

void GenerateRandom(Polynomial<ComplexT<BigFloat> > *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  srand(seed);
  const unsigned int MAX = 2*max_coeff;
  vector<ComplexT<BigFloat> > vec;
  for (unsigned int i = 0; i <= degree; ++i) {
    machine_long random = rand() % MAX - max_coeff;
    machine_long random2 = rand() % MAX - max_coeff;
    vec.push_back(ComplexT<BigFloat>(random, random2));
  }

  (*d) = Polynomial<ComplexT<BigFloat> >(vec);
}

void GenerateMignotte(Polynomial<ComplexT<BigFloat> > *d) {
  vector<ComplexT<BigFloat> > vec(21);
  vec[20] = ComplexT<BigFloat>(1, 0);
  vec[0] = ComplexT<BigFloat>(1, 0);
  vec[1] = ComplexT<BigFloat>(0, 300);
  vec[2] = ComplexT<BigFloat>(-30000,0);
  vec[3] = ComplexT<BigFloat>(0, -1000000);
  (*d) = Polynomial<ComplexT<BigFloat> >(vec);
}

#endif


}  // namespace benchmark.





