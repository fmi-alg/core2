/* **************************************
   File: SoftSubdivisionSearch.h

   Description: The Main module of the SSS Framework

   HISTORY: July, 2013: Surin Ahn
   Update: July, 2017: Ching-Hsiang (Tom) Hsu

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once

#include <iostream>

#include "sss/UnionFind.h"
#include "strategy/PriorityQueue.h"
#include "subdivision/ConfBox3d.h"
#include "predicate/ConfBox3dPredicate.h"
#include "path/Path.h"

#include "display/MainWindow.h"

using namespace std;

class SoftSubdivisionSearch {
 private:
  UnionFind* pSets;
  double epsilon;
  int searchType;
  int ct;
  BoxQueue* PQ;
  vector<ConfBox3d*> toReset;

  Point3d start;
  Point3d goal;
  Point3d startRot;
  Point3d goalRot;

  ConfBox3dPredicate* predicate;

  BoxQueue* getPriorityQueue();

  void insertNode(ConfBox3d* b);

  bool expand(ConfBox3d* b);

  ConfBox3d* findEnclosingFreeBox(Point3d p, Point3d pRot);

  void unionAdjacent(ConfBox3d* b);

  // find path using simple heuristic:
  // use distance to goal as key in PQ, see dijkstraQueue
  template<typename Cmp>
  bool findPath(ConfBox3d* boxA, ConfBox3d* boxB, int& ct);
  template<typename Cmp>
  ConfBox3d* findVorBox(ConfBox3d* freeBox, int& ct);

 public:
  ConfBox3d* pRoot;
  int freeCount;
  int stuckCount;
  int mixCount;
  int mixSmallCount;

  SoftSubdivisionSearch(ConfBox3d* root, double e, int searchType, Point3d start, Point3d goal, Point3d startRot, Point3d goalRot);

  int getNumSubdivisions() {
    return ct;
  }

  vector<ConfBox3d*> softSubdivisionSearch();

  ~SoftSubdivisionSearch(void) { }
};
