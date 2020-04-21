#pragma once

#include <unordered_set>

#include "subdivision/ConfBox3d.h"
#include "strategy/PriorityQueue.h"

class Path {
 public:
  static bool isNeighbor(ConfBox3d* a, ConfBox3d* b);

  static vector<ConfBox3d*> bfsShortestPath(ConfBox3d* a, ConfBox3d* b);
  static vector<ConfBox3d*> dijkstraSafePath(ConfBox3d* a, ConfBox3d* b);
  static vector<ConfBox3d*> dijkstraShortestPath(ConfBox3d* a, ConfBox3d* b);

  static void printPath(vector<ConfBox3d*> path);
};
