/*
 * benchmark.h
 *
 *  Created on: Jun 25, 2010
 *      Author: narayan
 *
 *  NOTE(Chee): ConvertPoly should be exposed here.
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include "CORE/poly/Curves.h"
#include "CORE/poly/Poly.h"

namespace benchmark {

// This header file defines the functions found in
// benchmark.cpp as per the core level being operated
// at.
void GetPoly(const char *file_name,
             Polynomial<double> *d);
void GetBiPoly(const char *filename,
               BiPoly<double> *u,
               BiPoly<double> *v);
void GetRootBounds(const Polynomial<double> &poly,
                   double *lb,
                   double *ub);

// Chee added this to do complexCurves
template <typename NT>
void ConvertPoly(const Polynomial<NT> &p,
                 BiPoly<NT> *real,
                 BiPoly<NT> *im);

// Generate random polynomials.
void GenerateRandom(Polynomial<double> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed);
void GenerateRandom(BiPoly<double> *u,
                    BiPoly<double> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed);

// NOTE(narayan):
// Functions for polynomials with complex coefficients.
// Not currently used. They cannot be used unless changes
// made to CORE to resolve ambiguous operators, and to
// support their output. Most errors stem from the BigFloat2
// class.
#ifdef ENABLE_COMPLEX_POLYS
void GetPoly(const char *file_name,
              Polynomial<Complex> *d);

void GetRootBounds(const Polynomial<Complex> &poly,
                   double *lb,
                   double *ub);
void GenerateRandom(Polynomial<Complex> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed);
#endif

}


#endif /* BENCHMARK_H_ */
