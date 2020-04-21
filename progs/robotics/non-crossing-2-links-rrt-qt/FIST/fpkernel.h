#ifndef FPKERNEL_H
#define FPKERNEL_H

#include<cfloat>
namespace {
    const double fpkernel_DBL_MAX = DBL_MAX;
}

#ifdef WITH_EXPR_WRAPPER
#define DOUBLE_OVERRIDE
typedef double machine_double;
#include <ExprWrapper.h>
#include <formattedIO.h>

#define double ExprWrapper::ExprWrapper
#define double_global ExprWrapper::ExprWrapper

#define REAL_TO_INT(x) (x).intValue()
#define TO_MDOUBLE(x) (x).doubleValue()
#define REAL_TO_FLOAT(x) (x).floatValue()

#define DOUBLE_TO_IOMDOUBLE(x) (x).doubleValue()
#define MDOUBLE_TO_IOMDOUBLE(x) (x)

#define SET_PRECISION(var, prec) var.set_k2precision(prec)
#define SET_CORE_MPFR_KERNEL(var, valk1, valk2, name) var.set_k1k2val(valk1, valk2, name)
#define SET_MPFR_KERNEL_PI(var) var.set_k2pi()

//atof
#define FP_atof double

#define shift  a68fe7dac8c32f30ad04d52fab4aa46be1068d82
#elif defined(WITH_COREBACKEND)
/* Set CORE Level to 4. Otherwise all unsigned long no longer work */
#define CORE_LEVEL 4

#include<CORE/CORE.h>
#include <formattedIO.h>

#undef double
#define double Expr
#define double_global Expr

#define double_arg const double &

//This is used to indicate that double gets a different meaning!
#define DOUBLE_OVERRIDE

#define TO_MDOUBLE(x) (x).doubleValue()

inline CORE::BigInt cut_comma(const Expr & x) {
	CORE::BigInt v = floor(x);
	if(v < 0) {
		return v + 1;
	} else {
		return v;
	}
}

#include "coreatan2.h"

inline int intfloor(const Expr & e) {
	return floor(e).intValue();
}

inline unsigned int uintceil(const Expr & e) {
	return (unsigned)ceil(e).intValue();
}

//REAL_TO_INT behaves like (int)(x)
#define REAL_TO_INT(x) cut_comma(x).intValue()
#define REAL_TO_GLINT(x) cut_comma(x).intValue()
#define REAL_TO_FLOAT(x) (x).floatValue()
#define BIGINT_TO_INT(x) (x).intValue()
#define TO_REAL(x) double(x)
#define STRING_TO_REAL(x) double(x)


#define DOUBLE_TO_IOMDOUBLE(x) (x).doubleValue()
#define MDOUBLE_TO_IOMDOUBLE(x) (x)

//atof
#define FP_atof double

//Definitions for functions undefined in CORE
#define isnan(x) 0
//#define atan2(y,x) atan2local((y),(x))
#define signbit(x) (x < 0 ? 1 : 0)

#define CORE_EXPR_OPS(name, x) { \
	std::map<std::string, unsigned int > m; \
	(x).rep()->rekCollectOp(m); \
	for(std::map<std::string, unsigned int>::iterator it = m.begin(); it != m.end(); ++it) { \
		std::cout << it->second << "\t: " << it->first << std::endl; \
	}}

#ifdef  WITH_CORE_EXPR_WRAPPER
inline void CORE_EXPR_SEQ(std::string name, const Expr & x) {
	std::map<unsigned int, std::string > m;
	(x).rep()->rekSequen(m);
	for (std::map<unsigned int, std::string>::iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << it->second << std::endl;
	}
}

inline void CORE_EXPR_DEBUG1(std::string name, const Expr & x) {
 	std::cout << name << " depth: " << (x).rep()->getDepth() << std::endl;
	std::cout << name << " elementCount: " << (x).rep()->getElementCount() << std::endl;
	CORE_EXPR_SEQ(name, (x));
	std::cout << name << " value: " << (x) << std::endl;
}
#endif


// The second argument is the SHA1sum of "shift\n". This avoids a 
// conflict with a shift function in CORE - VRONI uses some 
// variables named shift
#define shift  a68fe7dac8c32f30ad04d52fab4aa46be1068d82

#elif defined(WITH_RABACKEND)

#include<Default_real_algebraic.hpp>
#include <formattedIO.h>

typedef double machine_double;
#undef double
#define double Default_real_algebraic
#define double_global Default_real_algebraic

#define double_arg const double &

//This is used to indicate that double gets a different meaning!
#define DOUBLE_OVERRIDE

inline machine_double TO_MDOUBLE(const double & x) {
	x.guarantee_relative_error_two_to(-53);
	return x.get_interval().get_median();
}

inline int cut_comma(const double & x) {
	return (int)TO_MDOUBLE(x);
}


inline int intfloor(const double & e) {
	int cut = cut_comma(e);
	if ( e < 0) {
		return cut - 1;
	} else {
		return cut;
	}
}

inline double string_to_ra(const char * str) {
	machine_double md = atof(str);
	return double(md);
}

//inline unsigned int uintceil(const Expr & e) {
//	return (unsigned)ceil(e).intValue();
//}

//REAL_TO_INT behaves like (int)(x)
#define REAL_TO_INT(x) cut_comma(x)
#define REAL_TO_GLINT(x) cut_comma(x)
#define REAL_TO_FLOAT(x) TO_MDOUBLE(x)
//#define BIGINT_TO_INT(x) (x).intValue()
#define TO_REAL(x) double(machine_double(x))
#define STRING_TO_REAL(x) string_to_ra(x)

#define DOUBLE_TO_IOMDOUBLE(x) TO_MDOUBLE(x)
#define MDOUBLE_TO_IOMDOUBLE(x) (x)

//atof
#define FP_atof STRING_TO_REAL

//Definitions for functions undefined in CORE
#define isnan(x) 0
//#define atan2(y,x) atan2local((y),(x))
#define signbit(x) (x < 0 ? 1 : 0)


// The second argument is the SHA1sum of "shift\n". This avoids a 
// conflict with a shift function in CORE - VRONI uses some 
// variables named shift
#define shift  a68fe7dac8c32f30ad04d52fab4aa46be1068d82

#elif defined(WITH_MPFRBACKEND)

#include <mpfr_class.h>


typedef double machine_double;
#undef double

#ifdef WITH_EXPR_WRAPPER
#define WITH_MPFR_DOUBLE
#include<ExprWrapper.h>
#define double ExprWrapper::ExprWrapper
#define double_global ExprWrapper::ExprWrapper
#else
#define double Mpfr_class
#define double_global Mpfr_class_global
#endif /* WITH_EXPR_WRAPPER */


#define double_arg const double &

//This is used to indicate that double gets a different meaning!
#define DOUBLE_OVERRIDE


#define TO_MDOUBLE(x) (x).doubleValue()
#define REAL_TO_INT(x) (x).intValue()
#define REAL_TO_GLINT(x) (x).intValue()
#define REAL_TO_FLOAT(x) (x).floatValue()
#define TO_REAL(x) double(x)
#define STRING_TO_REAL(x) double(x)

#ifdef EXPR_WRAPPER
#define DOUBLE_TO_IOMDOUBLE(x) (x).odoubleValue()
#define MDOUBLE_TO_IOMDOUBLE(x) (x).doubleValue()
#else
#define DOUBLE_TO_IOMDOUBLE(x) (x).doubleValue()
#define MDOUBLE_TO_IOMDOUBLE(x) (x)
#endif

//atof
#define FP_atof double

#define SET_PRECISION(var, prec) var.set_precision(prec)
#define SET_CORE_MPFR_KERNEL(var, valk1, valk2, name) var = (valk2)
#define SET_MPFR_KERNEL_PI(var) var.set_pi()
#define SET_CORE_KERNEL(var,val)

inline void CORE_EXPR_DEBUG1(std::string , const double & ) {}

inline long intfloor(const double & e) {
#ifdef WITH_EXPR_WRAPPER
   return (int)floor(e);
#else
	return floor(e).intValue();
#endif
}

inline unsigned long uintceil(const double & e) {
#ifdef WITH_EXPR_WRAPPER
   return (int)floor(e);
#else
	return ceil(e).intValue();
#endif
}

//Definitions for functions undefined in CORE
#if WITH_EXPR_WRAPPER
#define is_nan(x) 0
#define sign_bit(x) (x < 0 ? 1 : 0)
#endif

#include <formattedIO.h>

#else /* no object-based numeric backend */

#include <math.h>

#define TO_MDOUBLE(x) (x)
#define DOUBLE_TO_IOMDOUBLE(x) (x)
#define MDOUBLE_TO_IOMDOUBLE(x) (x)
#define TO_MFLOAT(x) (x)
#define REAL_TO_INT(x) (int)(x)
#define REAL_TO_GLINT(x) (GLint)(x)
#define REAL_TO_FLOAT(x) (float)(x)
#define BIGINT_TO_INT(x) (x)
#define TO_REAL(x) (x)

//atof
#define FP_atof atof

#define machine_double double
#define double_global double

#define double_arg double

#ifdef __cplusplus
inline int intfloor(const double e) {
	return (int)(floor(e));
}

inline unsigned int uintceil(const double e) {
	return (unsigned)ceil(e);
}
#endif

#define CORE_EXPR_DEBUG1(x,y)

#define is_nan(x) isnan(x)
#define sign_bit(x) signbit(x)

#ifdef DOUBLE_OVERRIDE
	#error "DOUBLE_OVERRIDE should not be defined here!"
#endif


#endif /* WITH_COREBACKEND */


/* Definitions that are equal among all numeric object backends */
#ifdef DOUBLE_OVERRIDE

//formattedIO
#define FP_printf formattedIO::fmc_printf
#define FP_fprintf formattedIO::fmc_fprintf

inline const double * FP_PRNTARG(const double & x) { return &x; }

//[sf]scanf methods
#define FP_fscanf formattedIO::cfscanf
#define FP_sscanf formattedIO::csscanf

#else /* DOUBLE_OVERRIDE */

//IO
#define FP_printf printf
#define FP_fprintf fprintf

#define FP_PRNTARG(x) (x)

//[sf]scanf methods
#define FP_fscanf fscanf
#define FP_sscanf sscanf


#endif

#endif /* FPKERNEL_H */
