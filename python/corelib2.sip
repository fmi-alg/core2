/****************************************************************************
 * core2.sip -- Python Interface for Core Library 2
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
 *
 * $Id: corelib2.sip,v 1.1.1.1 2006/02/09 09:18:13 exact Exp $
 ***************************************************************************/

%Module corelib2 2

%ModuleHeaderCode
#define CORE_LEVEL 4
#include <CORE/CORE.h>
%End

%Include string.sip
%Include BigInt.sip
%Include BigRat.sip
%Include BigFloat.sip
