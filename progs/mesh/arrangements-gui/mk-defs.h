/**
 * mk_defs.h
 *
 * July 15, 2011
 */

#ifndef MK_MK_DEFS_H_
#define MK_MK_DEFS_H_

#include <stdlib.h>

#define CORE_LEVEL 4

#include "CORE/CORE.h"
#include "CORE/IntervalT.h"
#include "CORE/poly/Curves.h"
#include "CORE/BigFloat.h"
#include "CORE/linearAlgebraT.h"


//typedef DoubleWrapper DT;
//typedef DoubleWrapper NT;
typedef BigFloat NT;
typedef BigFloat DT;

// use DT for arithmetics
typedef IntervalT<DT> IntervalDT;
typedef IntervalT<NT> IntervalNT;

//
#include "Point.h"
#include "Vector.h"
typedef Point<DT,2> Point2d;
typedef Vector<DT,2> Vector2d;

//
#include "box.h"
typedef BoxT<DT> Box;
typedef BiPoly<DT> poly;


#endif
