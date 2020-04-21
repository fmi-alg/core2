#ifndef KRAWCZYK_KRAWCZYK_DEFS_H_
#define KRAWCZYK_KRAWCZYK_DEFS_H_
#define CXY2_CXY_DEFS_H_

// This file must be included before any other header in
// all files in this implementation. It defines a
// consistent CORE_LEVEL for the whole package. Any other
// globals that are required by the whole package can also
// go in here.

// Always operate in CORE_LEVEL 4. Runners of this program
// can specify a starting CORE_LEVEL to be either 1 , 2
// or 3.
//
#define CORE_LEVEL 4

#include "CORE/CORE.h"
#include "CORE/IntervalT.h"
#include "CORE/poly/Curves.h"
#include "CORE/linearAlgebraT.h"

enum OPERATOR_DECISION {
  OD_EXCLUDE = 0,
  OD_INCLUDE,
  OD_SUBDIVIDE
};

const bool LOG_DEBUG = false;

#include "box.h"
#include "rounded-ia.h"

//#define M_DIV(a, b) ((a)/(b))
//#define DIV_NS interval_ext
#define M_DIV(a, b) (rounded::Divide((a), (b)))
#define DIV_NS rounded


#endif // #ifdef KRAWCZYK_KRAWCZYK_H_
