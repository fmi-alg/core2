/* **************************************
   File: Wall.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include <math.h>
#include "Feature.h"

class Corner;
class Box;

class Wall : public Feature
{
public:
	Corner* src;
	Corner* dst;

	Wall(Corner* s, Corner* d);

	//distance functions
	virtual double distance(const Point2d& p);
	double distance_star(const Point2d& p);
	short distance_sign(const Point2d& p);

    //
	virtual bool inZone(const Point2d& p);
	virtual bool inZone_star(const Point2d& p);

	//
	virtual bool inZone(Box * b);
	virtual bool inZone_star(Box * b);

	bool isRight(const Point2d& p);

};
