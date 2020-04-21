/**
 * mk_defs.h
 *
 * July 15, 2011
 */

#ifndef MK_MK_DEFS_H_
#define MK_MK_DEFS_H_

#include <stdlib.h>

#define CORE_LEVEL 4

#include "IntervalT.h"
#include "Curves.h"
#include "linearAlgebraT.h"


//typedef DoubleWrapper DT;
//typedef DoubleWrapper NT;

// use DT for arithmetics
typedef CORE::IntervalT<double> IntervalDT;
typedef CORE::IntervalT<double> IntervalNT;

//
#include "Point.h"
#include "Vector.h"
typedef Point<double,2> Point2d;
typedef Vector<double,2> Vector2d;

//
#include "box.h"
typedef BoxT<double> Box;
typedef CORE::BiPoly<double> poly;


#endif
