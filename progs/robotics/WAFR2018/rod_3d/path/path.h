/*
 * path.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef PATH_H
#define PATH_H

#include <vector>

#include "strategy/box_priority_queue.h"
#include "subdivision/configuration_box3d.h"

class Path {
 public:
  static bool isNeighbor(const ConfBox3d& a, const ConfBox3d& b);

  static std::vector<ConfBox3d*> bfsShortestPath(ConfBox3d* a, ConfBox3d* b);
  static std::vector<ConfBox3d*> dijkstraSafePath(ConfBox3d* a, ConfBox3d* b);
  static std::vector<ConfBox3d*> dijkstraShortestPath(ConfBox3d* a,
                                                      ConfBox3d* b);

  static void printPath(std::vector<ConfBox3d*> path);
};

#endif  // PATH_H
