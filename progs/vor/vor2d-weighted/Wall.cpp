/* **************************************
   File: Wall.cpp

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "Wall.h"
#include "Corner.h"
#include "Box.h"

#include <float.h>

Wall::Wall(Corner* s, Corner* d):src(s), dst(d)
{
	src->nextWall = this;
	dst->preWall = this;
}

//	distance to line segment,
//	follows http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
double Wall::distance(const Point2d& p)
{
    const double x1 = src->pos[0];
    const double x2 = dst->pos[0];
    const double y1 = src->pos[1];
    const double y2 = dst->pos[1];
	double u = ( (p[0]-x1)*(x2-x1) + (p[1]-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	u = u < 0 ? 0 : u;
	u = u > 1 ? 1 : u;
	double x0 = u*x2 + (1-u)*x1;
	double y0 = u*y2 + (1-u)*y1;
	return sqrt( sqr(p[0]-x0) + sqr(p[1]-y0) ) / weight;
}

double Wall::distance_star(const Point2d& p)
{
    const double x1 = src->pos[0];
    const double x2 = dst->pos[0];
    const double y1 = src->pos[1];
    const double y2 = dst->pos[1];
    double u = ( (p[0]-x1)*(x2-x1) + (p[1]-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

    if(u>=1) return FLT_MAX;
    if(u<=0) return FLT_MAX;

    double x0 = u*x2 + (1-u)*x1;
    double y0 = u*y2 + (1-u)*y1;

    return sqrt( sqr(p[0]-x0) + sqr(p[1]-y0) ) / weight;
}

short Wall::distance_sign(const Point2d& p)
{
    const double x1 = src->pos[0];
    const double x2 = dst->pos[0];
    const double y1 = src->pos[1];
    const double y2 = dst->pos[1];
    double u = ( (p[0]-x1)*(x2-x1) + (p[1]-y1)*(y2-y1) ) / ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

    if(u>=1) return 1;
    if(u<=0) return -1;
    return 0;
}

//
bool Wall::inZone(const Point2d& p)
{
    short s=distance_sign(p);
    if(s==0) return true;
    return false;
}


bool Wall::inZone_star(const Point2d& p)
{
    if(inZone(p)==false) return false;
    if(isRight(p)) return true;
    return false;
}

bool Wall::inZone(Box * b)
{
    //check if the end points of this wall are in the box
    if( b->in(src->pos) || b->in(dst->pos) )
        return true;

    //center o
    const Point2d& o=b->o;
    double w2=b->width/2;  //half of width
    double h2=b->height/2; //half of height

    Point2d corner1(o[0]-w2,o[1]-h2);
    Point2d corner2(o[0]+w2,o[1]-h2);
    Point2d corner3(o[0]+w2,o[1]+h2);
    Point2d corner4(o[0]-w2,o[1]+h2);

    //check with the Zone of the wall
    short s1=distance_sign(corner1);
    short s2=distance_sign(corner2);
    short s3=distance_sign(corner3);
    short s4=distance_sign(corner4);


    if(s1==0 || s2==0 || s3==0 || s4==0)
        return true;

    if(s1!=s2 || s2!=s3 || s3!=s4 || s4!=s1)
        return true;

    return false;
}

bool Wall::inZone_star(Box * b)
{
    if(inZone(b)==false) return false;

    //center of box
    const Point2d& o=b->o;
    double w2=b->width/2;  //half of width
    double h2=b->height/2; //half of height

    Point2d corner1(o[0]-w2,o[1]-h2);
    Point2d corner2(o[0]+w2,o[1]-h2);
    Point2d corner3(o[0]+w2,o[1]+h2);
    Point2d corner4(o[0]-w2,o[1]+h2);

    //check the side of the wall
    bool r1=isRight(corner1);
    bool r2=isRight(corner2);
    bool r3=isRight(corner3);
    bool r4=isRight(corner4);

    int count=0;
    if(r1) count++;
    if(r2) count++;
    if(r3) count++;
    if(r4) count++;


    if(count==0) return false; //all on the left side

    //check with the Zone of the wall
    short s1=distance_sign(corner1);
    short s2=distance_sign(corner2);
    short s3=distance_sign(corner3);
    short s4=distance_sign(corner4);

    if(r1 && s1==0) return true; //in the zone and on the right
    if(r2 && s2==0) return true;
    if(r3 && s3==0) return true;
    if(r4 && s4==0) return true;

    //
    if( (r1 || r2) && s1!=s2) return true;
    if( (r2 || r3) && s2!=s3) return true;
    if( (r3 || r4) && s3!=s4) return true;
    if( (r4 || r1) && s4!=s1) return true;

    return false;
}

bool Wall::isRight(const Point2d& p)
{
	const double x1 = src->pos[0];
	const double x2 = dst->pos[0];
	const double y1 = src->pos[1];
	const double y2 = dst->pos[1];
	return (x2-x1)*(p[1]-y1) - (y2-y1)*(p[0]-x1) < 0;
}
