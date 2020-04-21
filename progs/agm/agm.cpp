#include "agm.h"

using namespace CORE;
typedef CORE::BigInt INT;

double u_inits[] = {1.1549, 1.2972, 1.4322, 1.5708, 1.7228, 1.9021, 2.1364};
double t_inits[] = {3.1738, 3.6591, 4.1878, 4.8105, 5.6004, 6.6999, 8.4688};
double m_inits[] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

// compute 2^{-n}
NT neg_power_2(int n) {
  return NT::exp2(-n);
}

//
// common agm procedure for computing pi
// (note: for now we do in full precision)
//
void agm_pi(NT& A, NT& T, int prec) {
  A = 1;
  NT B(2);
  B = div2(sqrt(B, prec)); // sqrt(2)/2
  T = 0.25;
  INT X(1);
  NT Y;

  while (-(A-B).uMSB() < prec) {
    Y = A;
    A = div2(A+B);
    B = sqrt(B*Y, prec);
    T -= X*(A-Y)*(A-Y);
    X <<= 1;
  }
}

//
// Discrete Newton iteration
//
// TODO: need check, if x+h is out of range, then should use
//
//      ff = (f - fun(x-h, p).div(h, p)
//
NT newton(eval_fun fun, const NT& c, const NT& x0, int prec) {
  int p = prec + 1;
  NT x(x0);
  NT del(1);
  NT f(0), ff(0);
  NT h = neg_power_2(p/2); // has to be > p/2
#ifdef AGM_DEBUG
  int counter = 0;
#endif
  do {
    f = fun(x, p);
    ff = (fun(x+h, p) - f).div(h, p);
    del = f - c;
    x -= del.div(ff, p);
#ifdef AGM_DEBUG
    std::cout << "del.uMSB()=" << del.uMSB() << std::endl;
    counter ++;
#endif
  } while (sgn(del) != 0 && -del.uMSB() < p);
#ifdef AGM_DEBUG
  std::cout << "counter=" << counter << std::endl;
#endif

  return x;
}

//
// Discrete Newton iteration
//
NT newton2(eval_fun2 fun, const NT& c, const NT& x0, int prec, const NT& val) {
  int p = prec + 1;
  NT x(x0);
  NT del(1);
  NT delx(1);
  NT f(0), ff(0);
  NT h = neg_power_2(p/2); // has to be > p/2

#ifdef AGM_DEBUG
  int counter = 0;
#endif
  do {
    f = fun(val, x, p);
    ff = (fun(val, x+h, p) - f).div(h, p);
    del = f - c;
    x -= del.div(ff, p);
#ifdef AGM_DEBUG
    //std::cout << "del.uMSB()=" << del.uMSB() << std::endl;
    counter ++;
#endif
  } while (del.sgn() != 0 && -del.uMSB() < p);
#ifdef AGM_DEBUG
  std::cout << "counter=" << counter << std::endl;
#endif

  return x;
}

//
// compute U_k(m)
//
NT eval_U_k(NT& bfPi, const NT& m, int k, int prec) {
  // compute F(\alpha_0) using AGM
  // precision needed for approximate \lim a_n
  int p = prec + 7;
  // iteration needed for approximate \lim a_n
  int kk = ceillg(ulong(p + 3));
  // precision needed for approximate b_0
  int pp = p + kk;
  NT A(1);
  NT B = sqrt(1-m, pp+4);
  NT C;

  for (int i=0; i<kk; i++) {
    C = div2(A+B);
    B = sqrt(A*B, pp+5);
    A = C;
  }
  // precisions needed for approximate \pi and division are p
  A = bfPi.div(A+B, p);

  // compute U_k(m)
  // precision needed for approximate s_0
  p = prec + 5 + ceillg(ulong(k));
  NT S = sqrt(m, p);
  for (int i=0; i<k; i++) {
    C = div2(1+S);
    A *= C;
    S = div(sqrt(S, p+8), C , p+4);
  }
  
  return div2(A*(1+S));
}

//
// compute U(m)
//
NT eval_U(NT& bfPi, const NT& m, int prec) {
  int k = ceillg(ulong(prec+7));
  return eval_U_k(bfPi, m, k, prec+1);
}

//
// compute T_k(m)
//
NT eval_T_k(const NT& m, int k, int prec) {
  // need compute v_k to relative (prec+18) bits
  int p = prec + 18 + 18*k;
  NT V(1);
  NT S = sqrt(m, p);
  NT W;

  for (int i=0; i<k; i++) {
/* THIS WAS the Core1 interface:
    W = 2*S*V.div(1+V*V, p-2);
    W = W.div(1 + (1-W*W).sqrt(p-6), p-7);
    W = (V+W).div(1-V*W, p-11);
    V = W.div(1 + (1+W*W).sqrt(p-15), p-16);
    S = 2*S.sqrt(p+4).div(1+S, p+4);
    p -= 18;
*/
// Here is the Core2 interface:
NT tmp;
    tmp.div(1+V*V, p-2);
    W = 2*S*tmp;
    tmp.sqrt(1-W*W, p-6);
    W = W.div(1 + (1-W*W).sqrt(p-6), p-7);
    W = (V+W).div(1-V*W, p-11);
    V = W.div(1 + (1+W*W).sqrt(p-15), p-16);
    S = 2*S.sqrt(p+4).div(1+S, p+4);
    p -= 18;
  }

  return (1+V).div(1-V, prec+18);
}

//
// compute T(m)
//
NT eval_T(const NT& m, int prec) {
  int k = ceillg(ulong(prec+23));
  return eval_T_k(m, k, prec+1);
}

//
// compute arctan(x)
//
NT eval_A_k(const NT& log4, const NT& x, int k, int prec) {
  // need compute v_k to relative (prec+18) bits
  int p = prec + 18 + 18*k;
  NT S = neg_power_2((prec-1)/2);
  NT V = x.div(1+(1+x*x).sqrt(p), p);
  NT Q(1);
  NT W;

  for (int i=0; i<k; i++) {
    Q = 2*Q.div(1+S, p);
    W = 2*S*V.div(1+V*V, p-2);
    W = W.div(1 + (1-W*W).sqrt(p-6), p-7);
    W = (V+W).div(1-V*W, p-11);
    V = W.div(1 + (1+W*W).sqrt(p-15), p-16);
    S = 2*S.sqrt(p+4).div(1+S, p+4);
    p -= 18;
  }

  V = (1+V).div(1-V, prec+18);

  return Q*(prim_log(4.0*V, prec) - log4);
}

//
// compute arctan(x)
//
NT eval_A(const NT& log4, const NT& m, int prec) {
  int k = 2*ceillg(ulong(prec+23));
  return eval_A_k(log4, m, k, prec+1);
}

