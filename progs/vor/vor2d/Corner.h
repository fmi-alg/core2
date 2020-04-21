#pragma once
#include <math.h>
#include <stdlib.h>
#include "Feature.h"

class Wall;
class Box;

class Corner : public Feature
{
public:

    //double x, y;
    Point2d pos; //position of the corner

	Wall* preWall;
	Wall* nextWall;

	Corner(const Point2d& p)
	{
	    pos=p;
	    preWall=nextWall=NULL;
	}

	virtual double distance(const Point2d& p)
	{
		return (p-this->pos).norm();
	}

	virtual bool inZone(Box * box);

	virtual bool inZone(const Point2d& p);

	virtual bool inZone_star(Box * box);

	virtual bool inZone_star(const Point2d& p);

	bool isConvex();

	bool isIsolated(){ return preWall==NULL && nextWall==NULL; }

	bool isDangling(){ return preWall==NULL || nextWall==NULL; }

};
