#ifndef __AGM_H__
#define __AGM_H__

#define CORE_LEVEL 4

#include <CORE/CORE.h>


typedef CORE::BigFloat NT;
typedef unsigned long ulong;

/// convert digits to bits
///
inline int digit2bit(int digits) {
    return (int)(digits*3.3219281);
}

/// common agm procedure for computing pi
///
void agm_pi(NT& A, NT& T, int prec);

/// compute \pi to prec relative precision
///
inline NT pi(int prec) {
  NT A, T; agm_pi(A, T, prec + 6);
  return A*A.div(T, prec + 5);
}

/// compute \sqrt{\pi} to prec relative precision
///
inline NT sqrtpi(int prec) {
  NT A, T; agm_pi(A, T, prec + 5);
  return div(A, sqrt(T, prec + 5), prec + 4);
}

/// newton evaluation function pointer
typedef NT (*eval_fun)(const NT&, int);
typedef NT (*eval_fun2)(const NT&, const NT&, int);

/// discrete newton iteration to solve f(x) = c
/// eval_fun -- evaluation function
/// c        -- constant
/// x0       -- initial approximation of x
/// prec     -- precision 
NT newton(eval_fun f, const NT& c, const NT& x0, int prec);
NT newton2(eval_fun2 f, const NT& c, const NT& x0, int prec, const NT& const_val);

// U(m), T(m), arctan(m)
NT eval_U(const NT& bfPi, const NT& m, int prec);
NT eval_T(const NT& m, int prec);
NT eval_A(const NT& bfLog4, const NT& m, int prec);

const int n_inits = 7;
extern double u_inits[];
extern double t_inits[];
extern double m_inits[];

/// compute exp(x) for x \in [1, 2]
inline NT prim_exp(const NT& x, int prec) {
  NT x0 = m_inits[n_inits-1];
  for (int i=0; i<n_inits; i++) {
    if (x < u_inits[i]) {
      x0 = m_inits[i];
      break;
    }
  }
  NT m = newton2(eval_U, x, x0, prec + 4, pi(prec+8));
  return eval_T(m, prec+1);
}

/// compute log(x) for x \in [3, 9]
inline NT prim_log(const NT& x, int prec) {
  NT x0 = m_inits[n_inits-1];
  for (int i=0; i<n_inits; i++) {
    if (x < t_inits[i]) {
      x0 = m_inits[i];
      break;
    }
  }
  NT m = newton(eval_T, x, x0, prec);
  return eval_U(pi(prec+8), m, prec+1);
}

// compute \tan(x/2)
inline NT half_tan(const NT& x, int prec) {
  return newton2(eval_A, div2(x), div2(x), prec, prim_log(NT(4.0), prec*2));
}

/// compute \sin(x) using \sin(x) = 2*m/(1+m*m) where m = tan(x/2)
inline NT prim_sin(const NT& x, int prec) {
  NT m = half_tan(x, prec+2);
  return 2*m.div(1+m*m, prec+1);
}

/// compute \cos(x) using \cos(x) = (1-m*m)/(1+m*m) = 2/(1+m*m) - 1
///  where m = tan(x/2)
inline NT prim_cos(const NT& x, int prec) {
  NT m = half_tan(x, prec+3);
  return NT(2.0).div(1+m*m, prec+1) - NT(1.0);
}

/// compute \tan(x) using \tan(x) = 2*m/(1-m*m) where m = tan(x/2)
inline NT prim_tan(const NT& x, int prec) {
  NT m = half_tan(x, prec+4);
  return 2*m.div(1-m*m, prec+1);
}

/// compute arctan(x)
inline NT prim_arctan(const NT& x, int prec) {
    return eval_A(prim_log(NT(4.0), prec*2), x, prec);
}

/// compute arcsin(x)
inline NT prim_arcsin(const NT& x, int prec) {
    return newton(prim_sin, x, NT(0.2), prec);
}

/// compute arccos(x)
inline NT prim_arccos(const NT& x, int prec) {
    return newton(prim_cos, x, NT(0.2), prec);
}

/// elementary functions
NT exp(const NT& x, int prec);
NT log(const NT& x, int prec);
NT sin(const NT& x, int prec);
NT cos(const NT& x, int prec);
NT tan(const NT& x, int prec);
NT arcsin(const NT& x, int prec);
NT arccos(const NT& x, int prec);
NT arctan(const NT& x, int prec);

#endif // __AGM_H__
