/* **************************************
   File: Edge.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include "math.h"

class Corner;

class Edge
{
public:
	Corner* src;
	Corner* dst;

	Edge(Corner* s, Corner* d);

	double distance(double x, double y, double z);

};
