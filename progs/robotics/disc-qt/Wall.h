/* **************************************
   File: Wall.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include "math.h"

// Class Forward Declaration
class Box;
class Corner;

class Wall
{
public:
	Corner* src;
	Corner* dst;

	Wall(Corner* s, Corner* d);
    
	double distance(double x, double y);

	bool isRight(double x, double y);
    short distance_sign(double x, double y);
    bool pointInZone(double x, double y);
    double projection(double x, double y);
    bool intersectZone(Box *b);
};
