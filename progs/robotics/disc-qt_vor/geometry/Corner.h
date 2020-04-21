#pragma once

#include "CORE/geom2d/point2d.h"

#include <math.h>

// Class Forward Declaration
class Box;
class Wall;

class Corner : public Point2d
{
public:
  Point2d p;
  Wall* preWall;
  Wall* nextWall;

  Corner(Point2d pp):p(pp){}

  bool isConvex();
  bool intersectZone(Box *b);
};
