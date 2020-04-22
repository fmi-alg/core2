/****************************************************************************
 * CORE_TWO.h -- The main inclusion file for the Core Library
 *
 * 	$Id: CORE_TWO.h,v 1.25 2010/07/21 16:18:08 exact Exp $
 *
 * Core Library Version 2.0, March 2006
 * Copyright (c) 1995-2006 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of Core Library (http://cs.nyu.edu/exact/core); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with Core Library.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: http://cs.nyu.edu/exact/core
 * Email: exact@cs.nyu.edu
 ***************************************************************************/
#ifndef __CORE_TWO_CORE_TWO_H__
#define __CORE_TWO_CORE_TWO_H__

#ifndef CORE_TWO_LEVEL
  #define CORE_TWO_LEVEL 3
#endif

#include <CORE_TWO/Config.h>
#include <CORE_TWO/CoreDefs.h>

// level 1 with Wrappers. 
// If Wrappers are to be disabled the
// CORE_TWO_LEVEL_1_NO_WRAPPERS must be defined.
#ifndef CORE_TWO_LEVEL_1_NO_WRAPPERS
#include <CORE_TWO/Wrappers.h>
#endif

// REMARK: even in Level 1, we may access the basic BigNumbers
// level 1
#include <CORE_TWO/BigInt.h> 
#include <CORE_TWO/BigRat.h>
#include <CORE_TWO/BigFloat.h>
#include <CORE_TWO/BFInterval.h>
// level 2
#include <CORE_TWO/BigFloat2.h>
// level 3
#include <CORE_TWO/Expr.h>

// include inline functions for ExprRep
#include <CORE_TWO/ExprRep.inl>

// timer
#include <CORE_TWO/Timer.h>

typedef double machine_double;
typedef long machine_long;

#if CORE_TWO_LEVEL == 1
  // LEVEL_1 with wrappers provides wrappers over fundamental
  // data types, and exposes the commonly used part of the  API
  // that the BigInt / BigFloat and BigRat classes expose. Define
  // this quantity if for some reason you would not like to use
  // wrappers at Level 1.
  #ifndef CORE_TWO_LEVEL_1_NO_WRAPPERS
    // LongWrappers are disabled for now because the implementation
    // is incomplete. So BigInts become longs.
    //
    // NOTE (Narayan): On many platforms, sizeof(long) == sizeof(int)
    // so we need "long long", available as int64_t on most unix
    // variants.
    //
    // #define BigInt LongWrapper
    // #define long LongWrapper

    #define BigInt long
    #define BigFloat DoubleWrapper
    #define BigRat DoubleWrapper
    #define Expr DoubleWrapper
    #define double DoubleWrapper
  #else
    // Use plain simple datatypes.
    #define BigInt long
    #define BigFloat double
    #define BigRat double
    #define Expr double
  #endif
#elif CORE_TWO_LEVEL == 2
  #undef double
  #define double BigFloat
  #undef long
  #define long BigInt
  #define Expr BigFloat
#elif CORE_TWO_LEVEL == 3
  #undef double
  #define double Expr
  #undef long
  #define long BigInt
#elif CORE_TWO_LEVEL == 4	// This allows all the types to mix
			// but type conversions are not fully defined
#elif CORE_TWO_LEVEL == 5	// this is a hack to allow users (Willi Mann)
			// to get "Level 3 doubles" (i.e., Expr), and
			// at the same time use "unsigned long" with no error.
  #undef double
  #define double Expr
#endif

#ifndef CORE_TWO_NO_AUTOMATIC_NAMESPACE
using namespace CORE_TWO;
#endif

#endif /*__CORE_TWO_CORE_TWO_H__*/
