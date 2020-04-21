/* **************************************
   File: Wall.cpp

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "Wall.h"
#include "Corner.h"

Wall::Wall(Corner* s, Corner* d):src(s), dst(d)
{
	src->nextWall = this;
	dst->preWall = this;
}

//	distance to line segment,
//	follows http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
double Wall::distance(double x, double y)
{
	double x1 = src->x;
	double x2 = dst->x;
	double y1 = src->y;
	double y2 = dst->y;
	double u = ( (x-x1)*(x2-x1) + (y-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	u = u < 0 ? 0 : u;
	u = u > 1 ? 1 : u;
	double x0 = u*x2 + (1-u)*x1;
	double y0 = u*y2 + (1-u)*y1;
	return sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
}

bool Wall::isRight(double x, double y)
{
	double x1 = src->x;
	double x2 = dst->x;
	double y1 = src->y;
	double y2 = dst->y;
	return (x2-x1)*(y-y1) - (y2-y1)*(x-x1) < 0;
}
