#pragma once
#include "../subdivision/ConfBox3d.h"
#include "../sss/Graph.h"
#include <unordered_set>

class Path {
 public:
  static bool isNeighbor(ConfBox3d* a, ConfBox3d* b);

  static vector<ConfBox3d*> bfsShortestPath(ConfBox3d* a, ConfBox3d* b);

  static vector<ConfBox3d*> dijkstraShortestPath(ConfBox3d* a, ConfBox3d* b);

  static void printPath(vector<ConfBox3d*> path);
};
