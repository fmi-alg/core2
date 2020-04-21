/* **************************************
   File: Wall.h

   Description: A wall is a triangle defined by vertices (corners) a, b, c.

   HISTORY: July, 2013: Surin Ahn

   ************************************** */

#pragma once
#include "./Obstacle.h"

class Corner;
class Edge;

class Wall : public Obstacle {
 public:
  Corner* a;
  Corner* b;
  Corner* c;

  Wall(Corner* aa, Corner* bb, Corner* cc);
  Wall(Corner aa, Corner bb, Corner cc);

  double distance(double x, double y , double z);

  bool isRight(double x, double y, double z);
};
