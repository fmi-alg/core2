/* **************************************
   File: Edge.cpp

   Description: 

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "Edge.h"
#include "Corner.h"

Edge::Edge(Corner* s, Corner* d):src(s), dst(d)
{
	src->nextEdge = this;
	dst->preEdge = this;
}

double Edge::distance(double x, double y, double z)
{
	double x1 = src->x;
	double x2 = dst->x;
	double y1 = src->y;
	double y2 = dst->y;
	double z1 = src->z;
	double z2 = dst->z; 

	double t_num = (x-x1)*(x2-x1)+(y-y1)*(y2-y1)+(z-z1)*(z2-z1); // <q-a, b-a>
	double t_denom = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1); // |b-a|^2 
	double t = t_num / t_denom;
	t = t < 0 ? 0 : t;
	t = t > 1 ? 1 : t;
	double x0 = t*x2 + (1-t)*x1;
	double y0 = t*y2 + (1-t)*y1;
	double z0 = t*z2 + (1-t)*z1;
	return sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) + (z-z0)*(z-z0) );
}

