#include "agm.h"

using namespace CORE;
typedef CORE::BigInt	INT;

#define TWO_PI      6.28318530
#define HALF_PI     1.57079632
#define QUAD_PI     0.78539816
#define PI          3.14159265
#define LOG2        0.693147181

NT MathNaN() {
  std::cerr << "MathNaN()" << std::endl;
  return NT(0);
}

NT exp(const NT& x, int prec) {
    if (x < 0.0) {                  // e^{x} = 1/e^{-x}
        return NT(1.0).div(exp(-x, prec+2), prec+2).makeExact();
    } else if (x < 1.0) {           // e^{x} = e^{x+1}/e
        return prim_exp(x+1.0, prec+1).div(prim_exp(1.0, prec+2), prec+4).makeExact();
    } else if (x < 2.0) { 
        return prim_exp(x, prec);
    } else {                        // e^{x} = (e^{r})^{2^k} where r = x/2^k
        long k = x.lMSB().asLong();
        int p = prec + k + 3*(1<<k) - 3;
        NT r = x.div(NT::exp2(k), p + 3).makeExact();
        return pow(exp(r, p+1), 1<<k); 
/*       
        long k = (x/LOG2).longValue();
        NT log2 = log(2.0, prec+k+4);
        std::cout << "x=" << x << std::endl;
        std::cout << "k=" << k << std::endl;
        std::cout << "log2=" << log2 << std::endl;
        std::cout << "x-k*log2=" << (x-k*log2) << std::endl;
        return NT::exp2(k) * exp(x-k*log2, prec+k+1);
*/
    }
}

NT log(const NT& x, int prec) {
	//std::cout << "x=" << x << std::endl;
    if (x <= 0.0) {                  // 
	//    std::cout << "<0?" << std::endl;
        return MathNaN();
    } else if (x < 1.0) {           // log(x) = -log(1/x)
	//std::cout << "<1?" << std::endl;
        return -log(NT(1.0).div(x, prec+1), prec+1).makeExact();
    } else if (x < 3.0) {
	//std::cout << "<3?" << std::endl;
        return prim_log(4.0*x, 2*prec) - prim_log(4.0, 2*prec);
    } else if (x < 9.0) {
	//    std::cout << "<9?" << std::endl;
        return prim_log(x, prec);
    } else {                        // log(x) = k*log(2) + log(x/2^k)
	//    std::cout << ">9?" << std::endl;
        long k = x.lMSB().asLong() - 2;
        NT log2 = log(2.0, prec + clLg(k) + 1);
        NT r = x * NT::exp2(-k);
        return k*log2 + prim_log(r, prec+2);
    }
}

NT reduce(const NT& x, int prec) {
    long p = x.uMSB().asLong();
    if ((unsigned long)p > sizeof(long)*8) {
        INT k = (x.div2().div(pi(p), p+1)).BigIntValue();
	if (p < prec+1)	p = prec+1;
	return (x - (k<<1)*pi(p));
    } else {
	long k = (x.div2().div(pi(p), p+1)).longValue();
	if (p < prec+1)	p = prec+1;
	return (x - (k<<1)*pi(p));
    }
}
	    
NT sin(const NT& x, int prec) {
    if (x < 0.0) {                              // sin(x) = -sin(-x)
        return -sin(-x, prec);
    } else if (x < QUAD_PI) {
        return prim_sin(x, prec);
    } else if (x < HALF_PI) {			// sin(x) = cos(\pi/2 - x)
        return prim_cos(pi(prec+1).div2() - x, prec+1);
    } else if (x < PI) {                        // sin(x) = sin(\pi - x)
        return prim_cos(pi(prec) - x, prec);
    } else if (x < TWO_PI) {
        return -prim_sin(2*pi(prec+1) - x, prec);    // sin(x) = -sin(2\pi - x)
    } else {
        return sin(reduce(x, prec), prec);   	// sin(x) = sin(x - 2\pi);
    } 
}

NT cos(const NT& x, int prec) {
    if (x < 0.0) {                              // cos(x) = cos(-x)
        return cos(-x, prec);
    } else if (x < QUAD_PI) {
        return prim_cos(x, prec);
    } else if (x < HALF_PI) {			// cos(x) = sin(\pi/2 - x)
        return prim_sin(pi(prec+1).div2() - x, prec+1);
    } else if (x < PI) {                        // cos(x) = -cos(\pi - x)
        return -prim_cos(pi(prec) - x, prec);
    } else if (x < TWO_PI) {
        return prim_cos(2*pi(prec+1) - x, prec);     // cos(x) = cos(2\pi - x)
    } else {
        return cos(reduce(x, prec), prec);   // cos(x) = cos(x - 2\pi);
    } 
}

NT tan(const NT& x, int prec) {
    if (x < 0.0) {                              // tan(x) = -tan(-x)
        return -tan(-x, prec);
    } else if (x < HALF_PI) {
        return prim_tan(x, prec);
    } else if (x < PI) {                        // tan(x) = -tan(\pi - x)
        return -prim_tan(pi(prec) - x, prec);
    } else if (x < TWO_PI) {
        return -prim_tan(2*pi(prec+1) - x, prec);    // tan(x) = -tan(2\pi - x)
    } else {
        return tan(reduce(x, prec), prec);   // tan(x) = tan(x - 2\pi);
    } 
}

NT arctan(const NT& x, int prec) {
    if (x < 0.0) {
        return -arctan(-x, prec);               // arctan(x) = -arctan(-x);
    } else if (x < 1.0) {
        return prim_arctan(x, prec);
    } else {                                    // arctan(x) = \pi/2 - arctan(1/x);
        return pi(prec).div2() - prim_arctan(NT(1.0).div(x, prec), prec);
    }
}

NT arcsin(const NT& x, int prec) {
    if (x < 0.0) {
        return -arcsin(-x, prec);               // arcsin(x) = -arcsin(-x);
    } else if (x < 1.0) {
        return prim_arcsin(x, prec);
    } else {
        return MathNaN();
    }
}

NT arccos(const NT& x, int prec) {
    if (x < 0.0) {
        return arccos(-x, prec);               // arccos(x) = -arccos(-x);
    } else if (x < 1.0) {
        return prim_arccos(x, prec);
    } else {
        return MathNaN();
    }
}

