/* **************************************
   File: Wall.cpp

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

// Header
#include "Box.h"
#include "Wall.h"
// Custom
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
// Chee: I am guessing that this returns 1 if (x,y) does not project on to the line segment represented by the wall,
//   then this return 1 or -1.  Otherwise it returns 0.
//
short Wall::distance_sign(double x, double y)
{
    double x1 = src->x;
    double x2 = dst->x;
    double y1 = src->y;
    double y2 = dst->y;
    double u = ( (x-x1)*(x2-x1) + (y-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

    if(u>=1) return 1;
    if(u<=0) return -1;
    return 0;  // Guess: this is when the point (x,y) projects onto the wall.
}

// point p(x, y) is projected to the wall
// return a real value t such that
//    when t equals 0, p projects onto the source
//    when t equals 1, p projects onto the destination
double Wall::projection(double x, double y)
{
    double x1 = src->x;
    double x2 = dst->x;
    double y1 = src->y;
    double y2 = dst->y;
    double t = ( (x-x1)*(x2-x1) + (y-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

    return t;
}

// Check point (x, y) is in the zone of the wall
// return true if it is in the zone
bool Wall::pointInZone(double x, double y)
{
    double x1 = src->x;
    double x2 = dst->x;
    double y1 = src->y;
    double y2 = dst->y;
    double u = ( (x-x1)*(x2-x1) + (y-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

    if(u>=1 || u<=0) return false;
    return isRight(x, y);
}

bool Wall::intersectZone(Box *b)
{
    //check if the end points of this wall are in the box
    if( b->in(src->x,src->y) || b->in(dst->x,dst->y) )
        return true;

    //center x,y
    double x=b->x;
    double y=b->y;
    double w2=b->width/2;  //half of width
    double h2=b->height/2; //half of height

    double corner1[2]={x-w2,y-h2};
    double corner2[2]={x+w2,y-h2};
    double corner3[2]={x+w2,y+h2};
    double corner4[2]={x-w2,y+h2};

    //check with the Zone of the wall
    short s1=distance_sign(corner1[0],corner1[1]);
    short s2=distance_sign(corner2[0],corner2[1]);
    short s3=distance_sign(corner3[0],corner3[1]);
    short s4=distance_sign(corner4[0],corner4[1]);

    if(s1==0 || s2==0 || s3==0 || s4==0) // on the wall
        return true;
    if(s1!=s2 || s2!=s3 || s3!=s4 || s4!=s1)
        return true;

    return false;
}
