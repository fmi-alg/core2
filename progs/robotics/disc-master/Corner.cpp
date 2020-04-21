// Header
#include "Box.h"
#include "Corner.h"
// Custom
#include "Wall.h"


bool Corner::isConvex()
{
	Corner* c = this->nextWall->dst;
	return !this->preWall->isRight(c->x, c->y);
}


bool Corner::intersectZone(Box *b)
{
    if(b->in(this->x,this->y)) return true;
    if(preWall == NULL) return true;

    double x=b->x;
    double y=b->y;

    double w2=b->width/2;  //half of width
    double h2=b->height/2; //half of height
    double dw[4] = {-1, 1, 1, -1};
    double dh[4] = {-1, -1, 1, 1};

    double corner[4][2];
    for(int i=0;i<4;++i){
        corner[i][0] = x+dw[i]*w2;
        corner[i][1] = y+dh[i]*h2;
    }

    for(int i=0;i<4;++i){
        if(preWall->projection(corner[i][0],corner[i][1]) > 1 &&
           nextWall->projection(corner[i][0],corner[i][1]) < 0)
        {
            return true;
        }
    }

    for(int i=0;i<3;++i){
        if(preWall->projection(corner[i][0],corner[i][1]) < 1 &&
           nextWall->projection(corner[i+1][0],corner[i+1][1]) > 0)
        {
            return true;
        }
    }

    return false;
}
