/*****************************************************************************/
/*                                                                           */
/*           F I S T :  Fast, Industrial-Strength Triangulation              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* (C)                            Martin Held                                */
/* (C)            Universitaet Salzburg, Salzburg, Austria                   */
/*                                                                           */
/* This code is not in the public domain. All rights reserved! Please make   */
/* sure to read the full copyright statement contained in api_functions.cpp. */
/*                                                                           */
/*****************************************************************************/

#ifndef _MARTIN_H_
#define _MARTIN_H_

/*                                                                           */
/* defines my boolean data type                                              */
/*                                                                           */
#ifdef BOOL_DEFINED
typedef bool boolean;
#else
#define false 0
#define true  (!false)
typedef unsigned char  boolean;
#endif


/*                                                                           */
/* some useful constants; some of them might be defined in  math.h...        */
/* we initialize them differently depending on whether or not this code is   */
/* to be used with the NYU's Core Library "CORE".                            */
/*                                                                           */
#ifdef DOUBLE_OVERRIDE

#ifdef M_PI
#undef M_PI
#endif
#ifdef M_2PI
#undef M_2PI
#endif
#ifdef M_PI_180
#undef M_PI_180
#endif
#if (defined(WITH_COREBACKEND) || defined(WITH_RABACKEND)) && !defined(WITH_MPFRBACKEND)
const double M_PI     = STRING_TO_REAL("3.14159265358979323846");
const double M_2PI    = STRING_TO_REAL("6.28318530717958647693");
const double M_PI_180 = STRING_TO_REAL("0.01745329251994329576");
#else
extern double M_PI;
extern double M_2PI;
extern double M_PI_180;
#endif

#else

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
#ifndef M_2PI
#define M_2PI           6.28318530717958647693
#endif
#ifndef M_PI_180
#define M_PI_180        0.01745329251994329576
#endif

#endif

/*                                                                           */
/* Some constants are used in expressions that need pure machine_double      */
/* constants. So above constants cannot be used in case                      */
/* defined(WITH_COREBACKEND). The following constants can be used instead:   */
/*                                                                           */
#ifndef MD_2PI
#define MD_2PI           6.28318530717958647693
#endif
#ifndef MD_PI
#define MD_PI            3.14159265358979323846
#endif


/*                                                                           */
/* these macros are convenient...                                            */
/*                                                                           */
#define Min(a, b)     ((a) < (b) ? (a) : (b))
#define Max(a, b)     ((b) < (a) ? (a) : (b))
#define Min3(a, b, c) (((a) < (b)) ? \
                       (((a) < (c)) ? \
                        (a) : (c)) \
                       : \
                       (((b) < (c)) ? \
                        (b) : (c)))
#define Max3(a, b, c) (((a) < (b)) ? \
                       (((b) < (c)) ? \
                        (c) : (b))  \
                       :  \
                       (((a) < (c)) ? \
                        (c) : (a)))
#define Min4(a, b, c, d) (((a) < (b)) ? \
                          (((a) < (c)) ? \
                           (((a) < (d)) ?\
                            (a) : (d)) \
                           : \
                           (((c) < (d)) ? \
                            (c) : (d))) \
                          : \
                          (((b) < (c)) ? \
                           (((b) < (d)) ? \
                            (b) : (d)) \
                           : \
                           (((c) < (d)) ? \
                            (c) : (d))))
#define Max4(a, b, c, d) (((a) < (b)) ? \
                          (((b) < (c)) ? \
                           (((c) < (d)) ?\
                            (d) : (c)) \
                           : \
                           (((b) < (d)) ? \
                            (d) : (b))) \
                          : \
                          (((a) < (c)) ? \
                           (((c) < (d)) ? \
                            (d) : (c)) \
                           : \
                           (((a) < (d)) ? \
                            (d) : (a))))

#define Sign(var,x) \
(var = x, \
 ((var > 0.0) ? 1 : ((var < 0.0) ? -1 : 0)))
#define Abs(x)        (((x) >= 0.0) ? (x) : -(x))
#define Square(x)     ((x) * (x))
/* #define xor(a, b)     (!(a) ^ !(b)) */
#define Odd(x)        ((x) % 2 == 1)
#define Even(x)       (! Odd(x))
#define Swap(i1, i2, i) \
{i  = i1; \
 i1 = i2; \
 i2 = i; }
#define Ceiling(x) \
(((x) == ((double) ((int) (x)))) ? ((int) (x)) : (1 + (int) (x)))

/*                                                                           */
/* some macros for epsilon-based comparisons with respect to zero...         */
/*                                                                           */
#define lt_(a, eps) (  ((a) <  -eps) )
#define ge(a, eps) (! ((a) <  -eps) )
#define le(a, eps) (  ((a) <=  eps) )
#define gt(a, eps) (! ((a) <=  eps) )
#define eq_(a, eps) ( (((a) <= eps) && !((a) < -eps)) )
#define ne_(a, eps) ( !((a) <= eps) ||  ((a) < -eps) )

#endif
