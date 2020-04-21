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
    Corner(): x(0), y(0) {}

    double distance(double x2, double y2)
    {
        return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2) );
    }

    Corner operator +(const Corner & c)
    {
        return Corner(x+c.x, y+c.y);
    }

    bool isConvex();

    virtual bool inZone(Box * box);

    virtual bool inZone(double x2, double y2);

    virtual bool inZone_star(Box * box);

    virtual bool inZone_star(double x2, double y2);

    bool isIsolated(){ return preWall==NULL && nextWall==NULL; }

    bool isDangling(){ return preWall==NULL || nextWall==NULL; }

    void rotate(double angle, double x_pivot, double y_pivot);

    void rotate(double angle, Corner& d);
};

