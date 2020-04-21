/* **************************************
   File: Wall.h

   Description:

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include "math.h"
#include "Feature.h"

class Corner;
class Box;

class pt2line {
public:
    double u, dist;
};

class Wall : public Feature
{
public:
    Corner* src;
    Corner* dst;

    Wall(Corner* s, Corner* d);
    Wall(Corner& s, Corner& d): Wall(&s, &d) {}


    //distance functions
    virtual double distance(double x, double y);
    pt2line distance2(double x, double y);
    double distance_star(double x, double y);
    short distance_sign(double x, double y);

    //
    virtual bool inZone(double x, double y);
    virtual bool inZone_star(double x, double y);

    //
    virtual bool inZone(Box * b);
    virtual bool inZone_star(Box * b);

    bool isRight(double x, double y);

    bool isOnSegment(Corner& c);

};
