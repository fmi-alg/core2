/*
 * ceval-defs.h
 *
 *  Created on: Jun 22, 2010
 *      Author: narayan
 */

#ifndef CEVAL_DEFS_H_
#define CEVAL_DEFS_H_

#ifndef CORE_LEVEL
 #define CORE_LEVEL 1
#endif

// Include the default CORE header files
#include "CORE/CORE.h"
// And the complex number package that is used throughout.
#include "CORE/ComplexT.h"

// The standard typedefs. NT, and Complex, the complex type
// derived from NT.
typedef ComplexT<double> Complex;

// Use this to control the type of coefficients
// of the polynomial, either real or complex coefficients.
//typedef ComplexT<double> PolyType;
typedef double PolyType;

using namespace std;

#endif /* CEVALDEFS_H_ */
