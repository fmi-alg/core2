// Custom
#include "subdivision/Box.h"
#include "geometry/Corner.h"
#include "geometry/Wall.h"


bool Corner::isConvex(){
  Corner* c = this->nextWall->dst;
  return (this->preWall->seg.orientation(c->p) < 0);
}


bool Corner::intersectZone(Box *b){
  if(b->in(this->p))  return true;
  if(preWall == NULL) return true;

  double w2=b->width/2;  //half of width
  double h2=b->height/2; //half of height
  double dw[4] = {-1, 1, 1, -1};
  double dh[4] = {-1, -1, 1, 1};

  Point2d corner[4];
  for(int i=0;i<4;++i){
    corner[i] = Point2d(b->mB.X()+dw[i]*w2, b->mB.Y()+dh[i]*h2);
  }

  for(int i=0;i<4;++i){
    if(preWall->seg.toLine().projectionLambda(corner[i]) >= 1 &&
       nextWall->seg.toLine().projectionLambda(corner[i]) <= 0){
      return true;
    }
  }

  return false;
}
