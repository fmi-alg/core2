#pragma once
#include <math.h>
class Wall;

class Corner
{
public:
	double x, y;
	Wall* preWall;
	Wall* nextWall;

	Corner(double xx, double yy):x(xx), y(yy)
	{

	}

	double distance(double x2, double y2)
	{
		return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) );
	}

	bool isConvex();
	bool intersectZone();
};
