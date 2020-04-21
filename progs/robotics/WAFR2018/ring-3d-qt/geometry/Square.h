/* **************************************
   File: Square.h

   Description: A Square is a face of S^2 hat.

   HISTORY: Nov. 28, 2016 Ching-Hsiang Hsu

   ************************************** */

#pragma once

#include "CORE/geom3d/point3d.h"

#include <string>

using namespace std;

class Corner;
class Edge;
class Wall;

class Square {
 public:
  string name;
  int id;

  Corner* cnr[4];
  Edge* edge[4];
  Wall* wall[2];

  Square(Corner* m_cnr[4]);
  Square(Corner cnr[4]);

  double distance(Point3d p);
};
