/*
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#pragma once
#ifndef _MATHTOOL_UTILITY
#define _MATHTOOL_UTILITY

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>   // define C++ stream I/O routines
#include <iomanip>
using namespace std;

/* Miscellaneous Scalar Math */
#define sqr(x)      ((x) * (x))
#define sign(x)     ((x)>=0? 1: -1)
#define applysign(x, y) ((y) >= 0? abs(x): -abs(x))

/* Angle Conversions & Constants */
#ifndef PI
#define PI ((double)3.1415926535897)
#endif

#ifndef PI2
#define PI2 ((double)6.2831853071794)
#endif

#define RAD2DEG (180/PI)
#define DEG2RAD (PI/180)

#define DegToRad(x) ((x)*DEG2RAD)
#define RadToDeg(x) ((x)*RAD2DEG)

#endif //_MATHTOOL_UTILITY
