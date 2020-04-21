/* **************************************
   File: Wall.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once

#include "CORE/geom2d/point2d.h"
#include "CORE/geom2d/line2d.h"
#include "CORE/geom2d/segment2d.h"

#include <math.h>

// Class Forward Declaration
class Box;
class Corner;

class Wall : public Segment2d
{
public:
  Segment2d seg;
  Corner* src;
  Corner* dst;

  Wall(Corner* s, Corner* d);

  short distance_sign(Point2d p);
  bool pointInZone(Point2d p);
  bool intersectZone(Box *b);
};
