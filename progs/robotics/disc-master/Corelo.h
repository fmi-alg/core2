/****************************************************************************
 * Core Library Version 1.7, August 2004
 * Copyright (c) 1995-2004 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of CORE (http://cs.nyu.edu/exact/core/); you may
 * redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with CORE.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * File: CoreIO.h
 *
 * 	Most of these code are to convert from mpfr's number IO.
 * 	Also, include some useful routines found in CoreIo.cpp for
 * 	processing text input files.
 *
 * Written by 
 *       Zilin Du <zilin@cs.nyu.edu>
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $Source: /home/exact/cvsroot/exact/corelib2/src/CoreIo.cpp,v $
 * $Revision: 1.1 $ $Date: 2006/11/10 21:09:28 $
 ***************************************************************************/


#ifndef __CORE_COREIO_H__
#define __CORE_COREIO_H__

#include <CORE/Config.h>
#include <CORE/CoreDefs.h>
#include <CORE/CoreAux.h>
#include <iostream>
#include <iomanip>

CORE_BEGIN_NAMESPACE


////////////////////////////////////////
// from CoreIo.cpp

// skip blanks, tabs, line breaks and comment lines,
// 	leaving us at the beginning of a token (or EOF):
int skip_comment_line (std::istream & in);

// skips an immediate '\' followed by '\n'
// 	NOTE: this assumes a very special file format (e.g., our BigInt File format)
// 	in which the '\' is immediately followed by '\n'.
int skip_backslash_new_line (std::istream & in);

// read the next "token" from the input stream into the buffer, while:
// 	(1) skipping over initial white spaces and comment lines
// 	(2) terminating at the first white space or comment char!
void read_next_token(std::istream& in, char* &buffer, int sz);

#endif
