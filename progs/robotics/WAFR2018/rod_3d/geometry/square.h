/*
 * square.h
 *
 * Created on: June. 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 * Description:
 *  a square is a face of S^2 hat.
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#pragma once


#include "CORE/geom3d/point3d.h"

class Corner;
class Edge;
class Wall;

class Square : public Feature {
 public:
  Corner* corner[4];
  Edge* edge[4];
  Wall* wall[2];

  Square(Corner* cnr[4]);
  Square(Corner cnr[4]);

  double distance(Point3d p);
};
