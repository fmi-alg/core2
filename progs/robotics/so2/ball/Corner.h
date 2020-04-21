#pragma once
#include <math.h>
class Edge;

class Corner
{
public:
	double x, y, z;
	Edge* preEdge;
	Edge* nextEdge;

	Corner(double xx, double yy, double zz):x(xx), y(yy), z(zz)
	{

	}

	double distance(double x2, double y2, double z2)
	{
		return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) + (z-z2)*(z-z2) );
	}	
};
