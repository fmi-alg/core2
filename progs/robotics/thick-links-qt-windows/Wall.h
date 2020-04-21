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

class Wall : public Feature
{
public:
    Corner* src;
    Corner* dst;

    Wall(Corner* s, Corner* d);

    //distance functions
    virtual double distance(double x, double y);
    double distance_inf_line(double x, double y, Corner *p);
    bool distance_at_end(double x, double y);
    double distance_star(double x, double y);
    short distance_sign(double x, double y);

    //
    virtual bool inZone(double x, double y);
    virtual bool inZone_star(double x, double y);

    //
    virtual bool inZone(Box * b);
    virtual bool inZone_star(Box * b);

    bool isRight(double x, double y);

    ~Wall(){};

};
