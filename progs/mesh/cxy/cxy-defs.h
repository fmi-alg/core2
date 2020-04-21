/* **************************************
   File: cxy-defs.h

   Description: 
	Core Levels, factored out from cxy.cpp.
   NOTE: for level 1, we need to define
  
   	CORE_LEVEL_1_USE_WRAPPERS
  
   in order to invoke the thin wrappers of Core Level 102.
	(Level 102 is really "Level 1.2", but we cannot use ".")

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: cxy-defs.h,v 1.4 2010/06/22 16:55:10 exact Exp $
 ************************************** */

// This file must be included before any other header in
// all files in the new CXY implementation. It defines a 
// consistent CORE_LEVEL for the whole package. Any other
// globals that are required by the whole package can also
// go in here.

#ifndef CXY2_CXY_DEFS_H_
#define CXY2_CXY_DEFS_H_

#ifndef CORE_LEVEL
  #define CORE_LEVEL 1
#endif

#define CORE_LEVEL_1_USE_WRAPPERS

#include "CORE/CORE.h"
#include "CORE/IntervalT.h"

typedef IntervalT<double> Interval;

#endif // #ifdef CXY2_CXY_DEFS_H_
