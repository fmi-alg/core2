/* **************************************
   File: Wall.cpp

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */
// Custom
#include "subdivision/Box.h"
#include "geometry/Corner.h"
#include "geometry/Wall.h"

Wall::Wall(Corner* s, Corner* d):src(s), dst(d){
  src->nextWall = this;
  dst->preWall = this;
  seg = Segment2d(s->p, d->p);
}

// Chee: I am guessing that
//   this returns 1 if (x,y) does not project on to the line segment represented by the wall,
//   then this return 1 or -1.  Otherwise it returns 0.
//
short Wall::distance_sign(Point2d p){
  double u = seg.toLine().projectionLambda(p);
  if(u>=1) return 1;
  if(u<=0) return -1;
  return 0;  // Guess: this is when the point (x,y) projects onto the wall.
}

// Check point p(x, y) is in the zone of the wall
// return true if it is in the zone
bool Wall::pointInZone(Point2d p){
  if(distance_sign(p) != 0) return false;
  return (seg.orientation(p) > 0);
}

bool Wall::intersectZone(Box *b){

  //check if the end points of this wall are in the box
  if(b->in(src->p) || b->in(dst->p))
    return true;

  double w2=b->width/2;  //half of width
  double h2=b->height/2; //half of height
  double dw[4] = {-1, 1, 1, -1};
  double dh[4] = {-1, -1, 1, 1};

  Point2d corner[5];
  for(int i=0;i<4;++i){
    corner[i] = Point2d(b->mB.X()+dw[i]*w2, b->mB.Y()+dh[i]*h2);
    if(pointInZone(corner[i])) return true;
  }
  corner[4] = corner[0];

  int ds[5];
  for(int i=0;i<4;++i){
    ds[i]=distance_sign(corner[i]);
  }
  ds[4] = ds[0];

  for(int i=0;i<4;++i){
    if((ds[i] != ds[i+1] &&
       (seg.orientation(corner[i]) > 0 || seg.orientation(corner[i+1]) > 0)))
      return true;
  }

  return false;
}
