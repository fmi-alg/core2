#pragma once
#include <math.h>
#include "Feature.h"
#include <stdlib.h>

class Wall;
class Box;

class Corner : public Feature
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

    virtual bool inZone(Box * box);

    virtual bool inZone(double x2, double y2);

    virtual bool inZone_star(Box * box);

    virtual bool inZone_star(double x2, double y2);

    bool isIsolated(){ return preWall==NULL && nextWall==NULL; }

    bool isDangling(){ return preWall==NULL || nextWall==NULL; }
};
