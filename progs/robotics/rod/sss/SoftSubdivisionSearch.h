/* **************************************
   File: SoftSubdivisionSearch.h

   Description: The Main module of the SSS Framework

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once
#include <iostream>
#include "./UnionFind.h"
#include "../strategy/PriorityQueue.h"
#include "../subdivision/ConfBox3d.h"
#include "../predicate/ConfBox3dPredicate.h"
#include "../path/Path.h"

using namespace std;

class SoftSubdivisionSearch {
 private:
  UnionFind* pSets;
  double epsilon;
  int QType;
  int ct;
  BoxQueue* PQ;
  vector<ConfBox3d*> toReset;
  // TODO: change this to use a ConfBox
  double alpha[3];
  double beta[3];
  ConfBox3dPredicate* predicate;

  BoxQueue* getPriorityQueue();

  void insertNode(ConfBox3d* b);

  bool expand(ConfBox3d* b);

  void unionAdjacent(ConfBox3d* b);

  ConfBox3d* findEnclosingFreeBox(double coordinate[3]);

  vector<ConfBox3d*> getCanonicalPath(ConfBox3d* boxA, ConfBox3d* boxB);

  // find path using simple heuristic:
  // use distance to beta as key in PQ, see dijkstraQueue
  void findPath(ConfBox3d* boxA, ConfBox3d* boxB);

 public:
  ConfBox3d* pRoot;
  int freeCount;
  int stuckCount;
  int mixCount;
  int mixSmallCount;

  SoftSubdivisionSearch(ConfBox3d* root, double e, int qType, unsigned int s, double alpha[3], double beta[3]);

  int getNumSubdivisions() {
    return ct;
  }

  vector<ConfBox3d*> softSubdivisionSearch();

  ~SoftSubdivisionSearch(void) { }
};
