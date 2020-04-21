#pragma once

#include "../subdivision/Box.h"
#include "../sss/Graph.h"

#include <unordered_set>

class Path{
 public:
  static vector<Box*> bfsShortestPath(Box* a, Box* b);
  static vector<Box*> safeShortestPath(Box* a, Box* b);
  static vector<Box*> dijkstraShortestPath(Box* a, Box* b);

  static void printPath(vector<Box*> path);
};
