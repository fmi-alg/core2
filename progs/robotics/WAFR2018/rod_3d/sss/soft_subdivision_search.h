/*
 * soft_subdivision_search.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef SOFT_SUBDIVISION_SEARCH_H
#define SOFT_SUBDIVISION_SEARCH_H

#include <iostream>

#include "path/path.h"
#include "predicate/configuration_box3d_predicate.h"
#include "sss/union_find.h"
#include "strategy/box_priority_queue.h"
#include "subdivision/configuration_box3d.h"

#include "display/main_window.h"

class SoftSubdivisionSearch {
 private:
  UnionFind* p_sets_;
  double epsilon_;
  int search_type_;
  int ct_;
  BoxQueue* PQ_;
  // TODO: change this to use a ConfBox
  Point3d start_;
  Point3d goal_;
  Point3d start_rotation_;
  Point3d goal_rotation_;

  ConfBox3dPredicate* predicate_ = nullptr;

  BoxQueue* getPriorityQueue();

  void insertNode(ConfBox3d* box);

  bool expand(ConfBox3d* box);

  ConfBox3d* findEnclosingFreeBox(Point3d point, Point3d point_rotation);

  void unionAdjacent(ConfBox3d* box);

  std::vector<ConfBox3d*> getCanonicalPath(ConfBox3d* boxA, ConfBox3d* boxB);

  // find path using simple heuristic:
  // use distance to goal as key in PQ, see dijkstraQueue
  template <typename Cmp>
  bool findPath(ConfBox3d* boxA, ConfBox3d* boxB);
  template <typename Cmp>
  ConfBox3d* findVorBox(ConfBox3d* free_box);

 public:
  ConfBox3d* root;
  int free_count;
  int stuck_count;
  int mix_count;

  SoftSubdivisionSearch(ConfBox3d* root, double e, int search_type,
                        Point3d start, Point3d goal, Point3d start_rotation,
                        Point3d goal_rotation);

  int getNumSubdivisions() { return ct_; }

  std::vector<ConfBox3d*> softSubdivisionSearch();

  ~SoftSubdivisionSearch(void) {}
};

#endif  // SOFT_SUBDIVISION_SEARCH_H
